#include <iostream>
#include <algorithm>
#include <tuple>
#include <syncstream>
#include <condition_variable>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <cmath>
#include <numeric>
#include "CPUComputationBackend.hpp"
#ifdef NDEBUG
#include <tracy/Tracy.hpp>
#endif
#include <chrono>
#include "FileWriter.hpp"

using namespace std;

CPUComputationBackend::CPUComputationBackend(FileWriter* fileWriter):frame_ptr_queue(5000), fileWriter(fileWriter){
    initThreads();
    resetAccumulators();
};

CPUComputationBackend::~CPUComputationBackend(){
    delete[] individual_analog_storage_ptr;
    #ifdef SINGLE_FRAMES_DEBUG
    delete[] pedestal_storage_ptr;
    delete[] pedestal_rms_storage_ptr;
    delete[] frame_classes_storage_ptr;
    #endif
};

void CPUComputationBackend::initThreads(){
    for (int x = 0; x < THREAD_AMOIUNT; ++x){
           threads.push_back(thread(&CPUComputationBackend::threadTask, this));
    }
}
void CPUComputationBackend::pause(){
    threads_sleep = true;
}
void CPUComputationBackend::resume(){
    allocateIndividualStorage();
    threads_sleep = false;
}

void CPUComputationBackend::allocateIndividualStorage(){
    delete[] individual_analog_storage_ptr;
    individual_analog_storage_ptr = new float[individual_frame_buffer_capacity*consts::LENGTH];
    #ifdef SINGLE_FRAMES_DEBUG
    delete[] pedestal_storage_ptr;
    pedestal_storage_ptr = new float[individual_frame_buffer_capacity*consts::LENGTH];
    delete[] pedestal_rms_storage_ptr;
    pedestal_rms_storage_ptr = new float[individual_frame_buffer_capacity*consts::LENGTH];
    delete[] frame_classes_storage_ptr;
    frame_classes_storage_ptr = new char[individual_frame_buffer_capacity*consts::LENGTH];
    #endif
}

void CPUComputationBackend::resetAccumulators(){
    memory_pool::release_memory();
    analog_sum.zero();
    thresholded_sum.zero();
    counting_sum.zero();
    processed_frames_amount = 0;
}
void CPUComputationBackend::resetPedestalAndRMS(){
    pedestal_counter_counting.zero();
    pedestal_sum_counting.zero();
    pedestal_squared_sum_counting.zero();
}
void CPUComputationBackend::dumpAccumulators(){
    fileWriter->openFile();
    fileWriter->writeFrame("images_sum", "analog_sum", analog_sum);
    fileWriter->writeFrame("images_sum", "counting_sum", counting_sum);
    if (saveIndividualFrames){
        fileWriter->writeFrameStack("individual_frames", "analog", individual_analog_storage_ptr, individual_frame_buffer_capacity);
        #ifdef SINGLE_FRAMES_DEBUG
        fileWriter->writeFrameStack("individual_frames", "pedestal", pedestal_storage_ptr, individual_frame_buffer_capacity);
        fileWriter->writeFrameStack("individual_frames", "pedestal_rms", pedestal_rms_storage_ptr, individual_frame_buffer_capacity);
        fileWriter->writeFrameStack("individual_frames", "frame_classes", frame_classes_storage_ptr, individual_frame_buffer_capacity);
        #endif
    }
    fileWriter->closeFile();
};
void CPUComputationBackend::processFrame(FullFrame *ff_ptr){
    #ifdef NDEBUG
    ZoneScoped;
    #endif
    OrderedFrame<float, consts::LENGTH> pedestal_current;
    OrderedFrame<float, consts::LENGTH> pedestal_rms_current;
    pedestal_share.lock_shared();
    CPUComputationBackend::loadPedestalAndRMS(pedestal_current, pedestal_rms_current);
    pedestal_share.unlock_shared();
    OrderedFrame<float, consts::LENGTH> no_bkgd = ff_ptr->f - pedestal_current;
    // 0 - pedestal pixel, 1 - photon pixel, 2 - max in cluster
    // better to create 3 different masks for easier assigment in sum frames...
    OrderedFrame<char, consts::LENGTH> frame_classes = CPUComputationBackend::classifyFrame(no_bkgd, pedestal_rms_current);
    if (updatePedestal){
        pedestal_share.lock();
        updatePedestalMovingAverage(ff_ptr->f, frame_classes, isPedestal);
        pedestal_share.unlock();
    }
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
    if (saveIndividualFrames){
        int frameindex = ff_ptr->m.frameIndex;
        /*
        safe comparasion of signed int and unsigned size_t would be
        std::cmp_less(frameindex, individual_frame_buffer_capacity)
        but we assume that frameindex is never negative
        */
        if (frameindex < individual_frame_buffer_capacity){
            float* frame_ptr = individual_analog_storage_ptr+frameindex*consts::LENGTH;
            no_bkgd.copy_to_buffer<float*>(frame_ptr, true);
            #ifdef SINGLE_FRAMES_DEBUG
            std::memcpy(pedestal_storage_ptr+frameindex*consts::LENGTH, pedestal_current.arr, sizeof(OrderedFrame<float, consts::LENGTH>::arr));
            std::memcpy(pedestal_rms_storage_ptr+frameindex*consts::LENGTH, pedestal_rms_current.arr, sizeof(OrderedFrame<float, consts::LENGTH>::arr));
            std::memcpy(frame_classes_storage_ptr+frameindex*consts::LENGTH, frame_classes.arr, sizeof(OrderedFrame<char, consts::LENGTH>::arr));
            #endif
            //std::memcpy(individual_analog_storage_ptr+frameindex*consts::LENGTH, no_bkgd.arr, sizeof(OrderedFrame<float, consts::LENGTH>::arr));
        }
    }
    processed_frames_amount++;
    memory_pool::free(ff_ptr);
}

 OrderedFrame<char, consts::LENGTH> CPUComputationBackend::classifyFrame(OrderedFrame<float, consts::LENGTH> &input, OrderedFrame<float, consts::LENGTH> &pedestal_rms){
    constexpr char cluster_size = 3;
    OrderedFrame<char, consts::LENGTH> class_mask;
    constexpr int c2 = (cluster_size + 1 ) / 2;
    constexpr int c3 = cluster_size;

    for (int iy = 0; iy < consts::FRAME_HEIGHT; iy++){
        for (int ix = 0; ix < consts::FRAME_WIDTH; ix++){
            float max_value, tl, tr, bl, br, tot;
            max_value = tl = tr = bl = br = tot = 0;
            float rms = pedestal_rms(iy, ix);
            float main_pixel_value = input(iy, ix);
            for (int ir = -cluster_size / 2; ir < cluster_size / 2 + 1; ir++){
                for (int ic = -cluster_size / 2; ic < cluster_size / 2 + 1; ic++){
                    const int y_sub = iy + ir;
                    const int x_sub = ix + ic;
                    if (y_sub >= 0 && y_sub < consts::FRAME_HEIGHT && x_sub >= 0 && x_sub < consts::FRAME_WIDTH){
                        const float value = input(y_sub, x_sub);
                        tot += value;
                        if (ir <= 0 && ic <=0) bl+= value;
                        if (ir <= 0 && ic >=0) br+= value;
                        if (ir >= 0 && ic <=0) tl+= value;
                        if (ir >= 0 && ic >=0) tr+= value;
                        // or
                        max_value = std::max(max_value, value);
                        //if (value > max_value) max_value = value;
                    }
                }
            }
            if (main_pixel_value < -counting_sigma * rms) {
                class_mask(iy, ix) = 3;
            } else if (max_value > counting_sigma * rms || 
                       std::max({bl, br, tl, tr}) > c2 * counting_sigma * rms ||
                       tot > c3 * counting_sigma * rms){
                class_mask(iy, ix) = 1;
                if (main_pixel_value == max_value){
                    class_mask(iy, ix) = 2;
                }
            } else {
                class_mask(iy, ix) = 0;
            }
        }
    }
    return class_mask;
 }

void CPUComputationBackend::loadPedestalAndRMS(OrderedFrame<float, consts::LENGTH> &pedestal, OrderedFrame<float, consts::LENGTH> &pedestal_rms){
    int counter = 0;
    for (unsigned int i = 0; i < consts::LENGTH; i++){
        counter = pedestal_counter_counting.arr[i];
        if (counter != 0) {
            pedestal.arr[i] = pedestal_sum_counting.arr[i] / counter;
            pedestal_rms.arr[i] = sqrt(pedestal_squared_sum_counting.arr[i] / counter - pow((pedestal_sum_counting.arr[i] / counter), 2));
        }
    }
};

void CPUComputationBackend::updatePedestalMovingAverage(OrderedFrame<unsigned short, consts::LENGTH> &raw_frame, OrderedFrame<char, consts::LENGTH> &frame_classes, bool isPedestal = false){
    for (unsigned int i = 0; i < consts::LENGTH; i++){
        if (!isPedestal && frame_classes.arr[i] != 0) continue;
        if (pedestal_counter_counting.arr[i] < consts::PEDESTAL_BUFFER_SIZE){
            pedestal_counter_counting.arr[i]++;
            pedestal_sum_counting.arr[i] = pedestal_sum_counting.arr[i] + raw_frame.arr[i];
            pedestal_squared_sum_counting.arr[i] = pedestal_squared_sum_counting.arr[i] + pow(raw_frame.arr[i], 2);
        } 
        else {
            pedestal_sum_counting.arr[i] = pedestal_sum_counting.arr[i] + raw_frame.arr[i] - pedestal_sum_counting.arr[i] / consts::PEDESTAL_BUFFER_SIZE;
            pedestal_squared_sum_counting.arr[i] = pedestal_squared_sum_counting.arr[i] + pow(raw_frame.arr[i], 2) - pedestal_squared_sum_counting.arr[i] / consts::PEDESTAL_BUFFER_SIZE;
            
        }
    }
};

void CPUComputationBackend::threadTask(){
        FullFrame* ff_ptr;
        while (true){
            while (!threads_sleep && frame_ptr_queue.pop(ff_ptr)){
                CPUComputationBackend::processFrame(ff_ptr);
            }
            //printf("either queue is empty or set to sleep\n");
            this_thread::sleep_for(0.03s);
        }
}
