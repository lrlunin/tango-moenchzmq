#include <iostream>
#include <algorithm>
#include <tuple>
#include <syncstream>
#include <condition_variable>
#include <fmt/core.h>
#include <cmath>
#include <numeric>
#include "ComputationBackend.hpp"
#include <tracy/Tracy.hpp>

using namespace std;

ComputationBackend::ComputationBackend():frame_ptr_queue(5000){};
void ComputationBackend::init_threads(){
    for (int x = 0; x < THREAD_AMOIUNT; ++x){
           threads.push_back(move(thread(&ComputationBackend::thread_task, this)));
    }
}
std::filesystem::path ComputationBackend::getFullFilepath(){
    std::filesystem::path full_filepath;
    full_filepath/=filepath;
    full_filepath/=filename;
    return full_filepath.lexically_normal();
};
void ComputationBackend::pause(){
    threads_sleep = true;
}
void ComputationBackend::resume(){
    threads_sleep = false;
}
void ComputationBackend::resetAccumulators(){
    analog_sum.zero();
    thresholded_sum.zero();
    counting_sum.zero();
    processed_frames_amount = 0;
}
void ComputationBackend::resetPedestalAndRMS(){
    pedestal_counter.zero();
    pedestal_sum.zero();
    pedestal_squared_sum.zero();
}
void ComputationBackend::dumpAccumulators(){
    
};
void ComputationBackend::process_frame(FullFrame *ff_ptr){
    ZoneScoped;
    UnorderedFrame<float, consts::LENGTH> pedestal_current;
    UnorderedFrame<float, consts::LENGTH> pedestal_rms_current;
    pedestal_share.lock_shared();
    ComputationBackend::loadPedestalAndRMS(pedestal_current, pedestal_rms_current);
    pedestal_share.unlock_shared();
    OrderedFrame<float, consts::LENGTH> no_bkgd = ComputationBackend::subtractPedestal(ff_ptr->f, pedestal_current);
    // 0 - pedestal pixel, 1 - photon pixel, 2 - max in cluster
    // better to create 3 different masks for easier assigment in sum frames...
    OrderedFrame<char, consts::LENGTH> frame_classes = ComputationBackend::classifyFrame(no_bkgd, pedestal_rms_current);
    
    pedestal_share.lock();
    updatePedestal(ff_ptr->f, frame_classes, isPedestal);
    pedestal_share.unlock();
    // it's very important that the prcoessed_frames amount will be increased at the very last moment
    // to prevent the race conditions
    if (!isPedestal){
        frames_sums.lock();
        // add to analog, threshold, counting
        counting_sum.addClass(frame_classes, 2);
        analog_sum += no_bkgd;
        //threshold later
        processed_frames_amount++;
        frames_sums.unlock();
    } else {
        processed_frames_amount++;
    }
    memory_pool::free(ff_ptr);
}

 OrderedFrame<char, consts::LENGTH> ComputationBackend::classifyFrame(OrderedFrame<float, consts::LENGTH> &input, UnorderedFrame<float, consts::LENGTH> &pedestal_rms){
    int nsigma = 3;
    char cluster_size = 3;
    OrderedFrame<char, consts::LENGTH> class_mask;
    int c2 = (cluster_size + 1 ) / 2;
    int c3 = cluster_size;

    for (int iy = 0; iy < consts::FRAME_HEIGHT; iy++){
        for (int ix = 0; ix < consts::FRAME_WIDTH; ix++){
            float max_value, tl, tr, bl, br, tot;
            max_value = tl = tr = bl = br = tot = 0;
            char pixel_class = 0;
            float rms = pedestal_rms(iy, ix);
            for (int ir = -cluster_size / 2; ir < cluster_size / 2 + 1; ir++){
                for (int ic = -cluster_size / 2; ic < cluster_size / 2 + 1; ic++){
                    const int y_sub = iy + ir;
                    const int x_sub = ix + ix;
                    if (y_sub >= 0 && y_sub < consts::FRAME_HEIGHT && x_sub >= 0 && x_sub < consts::FRAME_WIDTH){
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
            }
            if (input(iy, ix) < -nsigma * rms) {
                pixel_class = 3;
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
                class_mask(iy, ix) = 1;
            }
            else if (std::max({bl, br, tl, tr}) > c2 * nsigma * rms){
                pixel_class = 1;
                class_mask(iy, ix) = 1;
            }
            if (pixel_class == 1 && input(iy, ix) == max_value){
                pixel_class = 2;
                class_mask(iy, ix) = 2;
            }
        }
    }
    return class_mask;
 }

void ComputationBackend::loadPedestalAndRMS(UnorderedFrame<float, consts::LENGTH> &pedestal, UnorderedFrame<float, consts::LENGTH> &pedestal_rms){
    for (int y = 0; y < consts::FRAME_HEIGHT; y++){
        for (int x = 0; x < consts::FRAME_WIDTH; x++){
            const int counter = pedestal_counter(y, x);
            if (counter != 0) {
                pedestal(y, x) = pedestal_sum(y, x) / counter;
                pedestal_rms(y, x) = sqrt(pedestal_squared_sum(y, x) / counter - pow((pedestal_sum(y, x) / counter), 2));
            }
        }
    }
}

OrderedFrame<float, consts::LENGTH> ComputationBackend::subtractPedestal(UnorderedFrame<unsigned short, consts::LENGTH> &raw_frame, UnorderedFrame<float, consts::LENGTH> &pedestal_frame){
    OrderedFrame<float, consts::LENGTH> result_frame = {0};
    for (int y = 0; y < consts::FRAME_HEIGHT; y++){
        for (int x = 0; x < consts::FRAME_WIDTH; x++){
            result_frame(y, x) = raw_frame(y, x) - pedestal_frame(y, x);
        }
    }
    return result_frame;
};

void ComputationBackend::updatePedestal(UnorderedFrame<unsigned short, consts::LENGTH> &raw_frame, OrderedFrame<char, consts::LENGTH> &frame_classes, bool isPedestal = false){
    for (int y = 0; y < consts::FRAME_HEIGHT; y++){
        for (int x = 0; x < consts::FRAME_WIDTH; x++){
            //if (!isPedestal && frame_classes(y, x) != 0) continue;
            if (pedestal_counter(y, x) < consts::PEDESTAL_BUFFER_SIZE){
                pedestal_counter(y, x)++;
                pedestal_sum(y, x) = pedestal_sum(y, x) + raw_frame(y, x);
                pedestal_squared_sum(y, x) = pedestal_squared_sum(y, x) + pow(raw_frame(y, x), 2);
            } 
            else {
                pedestal_sum(y, x) = pedestal_sum(y, x) + raw_frame(y, x) - pedestal_sum(y, x) / consts::PEDESTAL_BUFFER_SIZE;
                pedestal_squared_sum(y, x) = pedestal_squared_sum(y,x) + pow(raw_frame(y, x), 2) - pedestal_squared_sum(y, x) / consts::PEDESTAL_BUFFER_SIZE;
            }
        }
    }
};

void ComputationBackend::thread_task(){
        FullFrame* ff_ptr;
        while (true){
            while (!threads_sleep && frame_ptr_queue.pop(ff_ptr)){
                ComputationBackend::process_frame(ff_ptr);
            }
            //printf("either queue is empty or set to sleep\n");
            this_thread::sleep_for(0.03s);
        }
        printf("thead %d died\n", this_thread::get_id());
}
