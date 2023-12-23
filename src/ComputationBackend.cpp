#include <iostream>
#include <algorithm>
#include <tuple>
#include <syncstream>
#include <condition_variable>

#include <cmath>
#include <numeric>
#include "ComputationBackend.hpp"
#include <tracy/Tracy.hpp>

using namespace std;

ComputationBackend::ComputationBackend():frame_ptr_queue(2000){};
void ComputationBackend::init_threads(){
    for (int x = 0; x<16; ++x){
           threads.push_back(move(thread(&ComputationBackend::thread_task, this)));
    }
}
void ComputationBackend::pause(){
    sleep = true;
}
void ComputationBackend::resume(){
    sleep = false;
}
void ComputationBackend::process_frame(FullFrame *ff_ptr){
    ZoneScoped;
    UnorderedFrame<float, LENGTH> pedestal_current;
    UnorderedFrame<float, LENGTH> pedestal_rms_current;
    pedestal_share.lock_shared();
    ComputationBackend::loadPedestalAndRMS(pedestal_current, pedestal_rms_current);
    pedestal_share.unlock_shared();
    OrderedFrame<float, LENGTH> no_bkgd = ComputationBackend::subtractPedestal(ff_ptr->f, pedestal_current);
    // 0 - pedestal pixel, 1 - photon pixel, 2 - max in cluster
    // better to create 3 different masks for easier assigment in sum frames...
    OrderedFrame<char, LENGTH> frame_classes = ComputationBackend::classifyFrame(no_bkgd, pedestal_rms_current);
    if (ff_ptr->m.frameIndex % 10 == 0){
        pedestal_share.lock();
        updatePedestal(ff_ptr->f, frame_classes, isPedestal);
        pedestal_share.unlock();
    }


    frames_sums.lock();
    // add to analog, threshold, counting
    //counting_sum.addClass(frame_classes, 2);
    //analog_sum += no_bkgd;
    //threshold later
    processed_frames_amount++;
    frames_sums.unlock();
    // if (ff_ptr->m.frameIndex == 1999){
    //     printf("PED: %f, PED^2 %f\n", pedestal_sum(2, 2), pedestal_squared_sum(2, 2));
    //     printf("PED_COUNTER: %f\n", pedestal_counter(2, 2));
    //     printf("RMS: %f, AVG: %f\n", pedestal_rms_current(2, 2), pedestal_current(2, 2));
    // }
    // printf("finish frame %d\n", ff_ptr->m.frameIndex);
    memory_pool::free(ff_ptr);
}

 OrderedFrame<char, LENGTH> ComputationBackend::classifyFrame(OrderedFrame<float, LENGTH> &input, UnorderedFrame<float, LENGTH> &pedestal_rms){
    int nsigma = 3;
    char cluster_size = 3;
    OrderedFrame<char, LENGTH> class_mask;
    int c2 = (cluster_size + 1 ) / 2;
    int c3 = cluster_size;
    
    for (int iy = 0; iy < 400; iy++){
        for (int ix = 0; ix < 400; ix++){
            float sub_clusts[4] = {0};
            float max_value, tl, tr, bl, br, tot = 0;
            char pixel_class = 0;
            float rms = pedestal_rms(iy, ix);
            for (int ir = -cluster_size /2; ir < cluster_size / 2 + 1; ir++){
                for (int ic = -cluster_size/2; ic < cluster_size/2 + 1; ic++){
                    const int y_sub = std::min(std::max(iy + ir, 0), 399);
                    const int x_sub = std::min(std::max(ix + ix, 0), 399);
                    const int value = input(y_sub, x_sub);
                    tot += value;
                    if (ir <= 0 && ic <=0) bl+= value;
                    if (ir <= 0 && ic >=0) br+= value;
                    if (ir >= 0 && ic <=0) tl+= value;
                    if (ir >= 0 && ic >=0) tr+= value;
                    // or
                    // max_value = std::max(max_value, value);
                    if (value > max_value) max_value = value;
                }
            }
            if (input(iy, ix) < -nsigma * rms) {
                pixel_class =3;
                class_mask(iy, ix) = 3;
                continue;
            }
            if (max_value > nsigma*rms){
                pixel_class = 1;
                class_mask(iy, ix) = 1;
                if (input(iy, ix) < max_value) continue;
            }
            else if (tot > c3*nsigma*rms) {
                pixel_class = 1;
                class_mask(iy, ix) =1;
            }
            else if (std::max({bl, br, tl, tr}) > c2 * nsigma * rms){
                pixel_class = 1;
                class_mask(iy, ix) = 1;
            }
            if (pixel_class == 1 && input(iy, ix ) == max_value){
                pixel_class = 2;
                class_mask(iy, ix) = 2;
            }
        }
    }
    return class_mask;
 }

void ComputationBackend::loadPedestalAndRMS(UnorderedFrame<float, LENGTH> &pedestal, UnorderedFrame<float, LENGTH> &pedestal_rms){
    for (int y = 0; y < 400; y++){
        for (int x = 0; x < 400; x++){
            const int counter = pedestal_counter(y, x);
            if (counter != 0) {
            pedestal(y, x) = pedestal_sum(y, x) / counter;
            pedestal_rms(y, x) = sqrt(pedestal_squared_sum(y, x) / counter - (pedestal_sum(y, x) / counter) * (pedestal_sum(y, x) / counter));
            }
        }
    }
}

OrderedFrame<float, LENGTH> ComputationBackend::subtractPedestal(UnorderedFrame<unsigned short, LENGTH> &raw_frame, UnorderedFrame<float, LENGTH> &pedestal_frame){
    OrderedFrame<float, LENGTH> result_frame = {0};
    for (int y = 0; y < 400; y++){
        for (int x = 0; x < 400; x++){
            result_frame(y, x) = raw_frame(y, x) - pedestal_frame(y, x);
        }
    }
    return result_frame;
};

void ComputationBackend::updatePedestal(UnorderedFrame<unsigned short, LENGTH> &raw_frame, OrderedFrame<char, LENGTH> &frame_classes, bool isPedestal = false){
    for (int y = 0; y < 400; y++){
        for (int x = 0; x < 400; x++){
            //if (!isPedestal && frame_classes(y, x) != 0) continue;
            if (pedestal_counter(y, x) < pedestal_buff_size){
                pedestal_counter(y, x)++;
                pedestal_sum(y, x) = pedestal_sum(y, x) + raw_frame(y, x);
                pedestal_squared_sum(y, x) = pedestal_squared_sum(y, x) + raw_frame(y, x)*raw_frame(y, x);
            } 
            else {
                pedestal_sum(y, x) = pedestal_sum(y, x) + raw_frame(y, x) - pedestal_sum(y, x) / pedestal_buff_size;
                pedestal_squared_sum(y, x) = pedestal_squared_sum(y,x) + raw_frame(y, x)*raw_frame(y, x) - pedestal_squared_sum(y, x) / pedestal_buff_size;
            }
        }
    }
};

void ComputationBackend::thread_task(){
        FullFrame* ff_ptr;
        while (true){
            while (!sleep && frame_ptr_queue.pop(ff_ptr)){
                ComputationBackend::process_frame(ff_ptr);
            }
            printf("either queue is empty or set to sleep\n");
            this_thread::sleep_for(0.03s);
        }
        printf("thead %d died\n", this_thread::get_id());
}
