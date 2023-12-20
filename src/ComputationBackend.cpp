#include <iostream>
#include <algorithm>
#include <tuple>
#include <syncstream>
#include <condition_variable>

#include <cmath>
#include <numeric>
#include "ComputationBackend.hpp"

using namespace std;

ComputationBackend::ComputationBackend():frame_ptr_queue(2000){};
void ComputationBackend::init_threads(){
    for (int x = 0; x<6; ++x){
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
    pedestal_share.lock_shared();
    auto pedestal = ComputationBackend::getPedestal();
    pedestal_share.unlock_shared();
    auto frame_classes = ComputationBackend::classifyFrame(ff_ptr->f);
    pedestal_share.lock();
    updatePedestal(ff_ptr->f);
    pedestal_share.unlock();
    printf("finish frame %d\n", ff_ptr->m.frameIndex);
    memory_pool::free(ff_ptr);
}

 OrderedFrame<char, LENGTH> ComputationBackend::classifyFrame(UnorderedFrame<unsigned short, LENGTH> &input){
    float sigma = 19;
    char cluster_size = 3;
    OrderedFrame<char, LENGTH> class_mask;
    float c2 = (cluster_size + 1 ) / 2;
    float c3 = cluster_size;

    for (int y = 0; y < 400; y++){
        for (int x = 0; x < 400; x++){
            class_mask(y, x) = input(y, x) % 3;
        }
    }
    this_thread::sleep_for(6ms);
    return class_mask;
 }

UnorderedFrame<float, LENGTH> ComputationBackend::getPedestal(){
    UnorderedFrame<float, LENGTH> output;
    for (int y = 0; y < 400; y++){
        for (int x = 0; x < 400; x++){
            output(y, x) = pedestal_sum(y, x) / std::max(pedestal_counter(y, x), 1u);
        }
    }
    return output;
}

OrderedFrame<float, LENGTH> ComputationBackend::subtractPedestal(UnorderedFrame<unsigned short, LENGTH> &raw_frame, UnorderedFrame<float, LENGTH> &pedestal){
    OrderedFrame<float, LENGTH> result = {0};
    for (int y = 0; y < 400; y++){
        for (int x = 0; x < 400; x++){
            result(y, x) = raw_frame(y, x) - pedestal(y, x);
        }
    }
    return result;
};

void ComputationBackend::updatePedestal(UnorderedFrame<unsigned short, LENGTH> &raw_frame){
    for (int y = 0; y < 400; y++){
        for (int x = 0; x < 400; x++){
            if (pedestal_counter(y, x) < pedestal_buff_size){
                pedestal_sum(y, x) = pedestal_sum(y, x) + raw_frame(y, x);
                pedestal_sum(y, x)++;
            } 
            else {
                pedestal_sum(y, x) = pedestal_sum(y, x) + raw_frame(y, x) - pedestal_sum(y, x)/pedestal_buff_size;
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
