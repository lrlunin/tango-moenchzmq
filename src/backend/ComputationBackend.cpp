#include <iostream>
#include <algorithm>
#include <tuple>
#include <syncstream>
#include <condition_variable>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <cmath>
#include <numeric>
#include "ComputationBackend.hpp"
#ifdef NDEBUG
#include <tracy/Tracy.hpp>
#endif
#include <chrono>
using namespace std;

ComputationBackend::ComputationBackend():frame_ptr_queue(5000){
    const auto now = chrono::system_clock::now();
    // 20240109_run like folder and file name
    file_path = fmt::format("{:%Y%m%d}_run", now);
    file_name = file_path;
    file_index = 0;
    initThreads();
};
void ComputationBackend::initThreads(){
    for (int x = 0; x < THREAD_AMOIUNT; ++x){
           threads.push_back(thread(&ComputationBackend::threadTask, this));
    }
}
std::filesystem::path ComputationBackend::getFullFilepath(){
    std::filesystem::path full_filepath(save_root_path);
    full_filepath/=file_path;
    full_filepath/=file_name;
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
    pedestal_counter_counting.zero();
    pedestal_sum_counting.zero();
    pedestal_squared_sum_counting.zero();
}
void ComputationBackend::dumpAccumulators(){
    
};
void ComputationBackend::processFrame(FullFrame *ff_ptr){
    #ifdef NDEBUG
    ZoneScoped;
    #endif
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
        frames_sums.unlock();
    }
    processed_frames_amount++;
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
            float rms = pedestal_rms(iy, ix);
            float main_pixel_value = input(iy, ix);
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
            if (main_pixel_value < -nsigma * rms) {
                class_mask(iy, ix) = 3;
            } else if (max_value > nsigma*rms || 
                       std::max({bl, br, tl, tr}) > c2 * nsigma * rms ||
                       tot > c3*nsigma*rms){
                class_mask(iy, ix) = 1;
                if (main_pixel_value == max_value){
                    class_mask(iy, ix) = 2;
                }
            }
        }
    }
    return class_mask;
 }

void ComputationBackend::loadPedestalAndRMS(UnorderedFrame<float, consts::LENGTH> &pedestal, UnorderedFrame<float, consts::LENGTH> &pedestal_rms){
    for (int y = 0; y < consts::FRAME_HEIGHT; y++){
        for (int x = 0; x < consts::FRAME_WIDTH; x++){
            const int counter = pedestal_counter_counting(y, x);
            if (counter != 0) {
                pedestal(y, x) = pedestal_sum_counting(y, x) / counter;
                pedestal_rms(y, x) = sqrt(pedestal_squared_sum_counting(y, x) / counter - pow((pedestal_sum_counting(y, x) / counter), 2));
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
            if (!isPedestal && frame_classes(y, x) != 0) continue;
            if (pedestal_counter_counting(y, x) < consts::PEDESTAL_BUFFER_SIZE){
                pedestal_counter_counting(y, x)++;
                pedestal_sum_counting(y, x) = pedestal_sum_counting(y, x) + raw_frame(y, x);
                pedestal_squared_sum_counting(y, x) = pedestal_squared_sum_counting(y, x) + pow(raw_frame(y, x), 2);
            } 
            else {
                pedestal_sum_counting(y, x) = pedestal_sum_counting(y, x) + raw_frame(y, x) - pedestal_sum_counting(y, x) / consts::PEDESTAL_BUFFER_SIZE;
                pedestal_squared_sum_counting(y, x) = pedestal_squared_sum_counting(y,x) + pow(raw_frame(y, x), 2) - pedestal_squared_sum_counting(y, x) / consts::PEDESTAL_BUFFER_SIZE;
            }
        }
    }
};

void ComputationBackend::threadTask(){
        FullFrame* ff_ptr;
        while (true){
            while (!threads_sleep && frame_ptr_queue.pop(ff_ptr)){
                ComputationBackend::processFrame(ff_ptr);
            }
            //printf("either queue is empty or set to sleep\n");
            this_thread::sleep_for(0.03s);
        }
}
