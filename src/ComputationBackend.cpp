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
    for (int x = 0; x<4; ++x){
           threads.push_back(move(thread(&ComputationBackend::thread_task, this)));
    }
}
void ComputationBackend::pause(){
    sleep = true;
}
void ComputationBackend::resume(){
    sleep = false;
}
void ComputationBackend::classifyFrame(const unsigned short (&input)[16000], int (&output)[16000]){
    for (auto i = 0; i < 16000; ++i){
        output[i] = 0;
    }
}
void ComputationBackend::process_frame(FullFrame *ff_ptr){
    printf("thread %d, frameIndex %d\n", this_thread::get_id(), ff_ptr->m.frameIndex);
    pedestal_share.lock_shared();
    printf("reader in\n");
    // read pedestal ...
    this_thread::sleep_for(0.2s);
    printf("reader out\n");
    pedestal_share.unlock_shared();

    int pixel_classes[16000] = {0};
    ComputationBackend::classifyFrame(ff_ptr->arr, pixel_classes);

    pedestal_share.lock();
    printf("writer in\n");
    this_thread::sleep_for(0.2s);
    // write pedestal
    printf("writer out\n");
    pedestal_share.unlock();

    int frame_sum = accumulate(ff_ptr->arr, ff_ptr->arr+16000, 0);
    printf("frame sum %d\n", frame_sum);
}
void ComputationBackend::calcPedestal(int (&arr)[16000]){
        for (auto i = 0; i < 16000; ++i){
            arr[i] = pedestal_frame[i] / pedestal_counter[i];
        }
    }
void ComputationBackend::thread_task(){
        FullFrame* ff_ptr;
        while (true){
            while (!sleep && frame_ptr_queue.pop(ff_ptr)){
                ComputationBackend::process_frame(ff_ptr);
                memory_pool::free(ff_ptr);
            }
            printf("either queue is empty or set to sleep\n");
            this_thread::sleep_for(0.03s);
        }
        printf("thead %d died\n", this_thread::get_id());
}
