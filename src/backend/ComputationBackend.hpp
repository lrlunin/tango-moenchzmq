#pragma once
#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <filesystem>

namespace consts{
    constexpr int FRAME_WIDTH = 400;
    constexpr int FRAME_HEIGHT = 400;
    constexpr unsigned int LENGTH = FRAME_HEIGHT * FRAME_WIDTH;
    constexpr float PEDESTAL_BUFFER_SIZE = 1000;
    #include "remap_array.hpp"
}
template <typename T, unsigned int V>
struct UnorderedFrame{
    T arr[V];
    T& operator()(int y, int x) {
        return arr[consts::reorder_map[y][x]];
    }
    void zero(){
        std::fill(arr, arr + V, 0);
    }
};
template <typename T, unsigned int V>
struct OrderedFrame{
    T arr[V];
    T& operator()(int y, int x) {
        return arr[y * consts::FRAME_WIDTH + x];
    }
    template <typename B>
    void copy_to_buffer(B &buffer, bool flip = true){
        for (int y = 0; y < consts::FRAME_HEIGHT; y++){
            for (int x = 0; x < consts::FRAME_WIDTH; x++){
                /*
                to flip image vertically we need either running y from 0 to 399 or 399 to 0
                if not flipped then flip_arr[y * consts::FRAME_WIDTH + x] = arr[y * consts::FRAME_WIDTH + x]
                if flipped then flip_arr[y * consts::FRAME_WIDTH + x] = arr[(consts::FRAME_HEIGHT - 1 - y) * consts::FRAME_WIDTH + consts::FRAME_WIDTH  + x]
                the resulting formula below is just some math trickery to avoid if coniditions
                */
                buffer[y * consts::FRAME_WIDTH + x] = arr[flip * (consts::FRAME_WIDTH * (consts::FRAME_HEIGHT - 1)) + consts::FRAME_WIDTH * y *(-1 + 2 * (!flip)) + x];
            }
        }
    };
    OrderedFrame& operator+=(const OrderedFrame& rhs){
        for (unsigned int i = 0; i < V; i++){
            arr[i] += rhs.arr[i];
        }
        return *this;
    }
    void addClass(OrderedFrame<char, V>& classes, const char class_nr){
        for (unsigned int i = 0; i < V; i++){
            arr[i] = (classes.arr[i] == class_nr);
        }
    }
    void zero(){
        std::fill(arr, arr + V, 0);
    }
};
struct Metadata{
    int bitmode;
    int frameIndex;
};
struct FullFrame{
    Metadata m;
    UnorderedFrame<unsigned short, consts::LENGTH> f;
};
class HDFWriter;
class ComputationBackend{
public:
    typedef boost::singleton_pool<FullFrame, sizeof(FullFrame)> memory_pool;
    ComputationBackend(std::string save_root_path);
    ~ComputationBackend();
    std::string save_root_path, file_path, file_name;
    std::atomic<long> file_index;
    boost::lockfree::queue<FullFrame*> frame_ptr_queue;
    std::vector<std::thread> threads;
    std::shared_mutex pedestal_share;
    std::mutex frames_sums;
    std::atomic<long> processed_frames_amount;
    std::atomic<long> live_period;


    int THREAD_AMOIUNT = 10;
    void initThreads();
    std::filesystem::path getFullFilepath();
    void pause();
    void resume();
    void resetAccumulators();
    void resetPedestalAndRMS();
    void dumpAccumulators();
    void loadPedestalAndRMS(UnorderedFrame<float, consts::LENGTH> &pedestal, UnorderedFrame<float, consts::LENGTH> &pedestal_rms);
    OrderedFrame<char, consts::LENGTH> classifyFrame(OrderedFrame<float, consts::LENGTH> &input, UnorderedFrame<float, consts::LENGTH> &pedestal_rms);
    OrderedFrame<float, consts::LENGTH> subtractPedestal(UnorderedFrame<unsigned short, consts::LENGTH> &raw_frame, UnorderedFrame<float, consts::LENGTH> &pedestal);
    void updatePedestal(UnorderedFrame<unsigned short, consts::LENGTH> &raw_frame, OrderedFrame<char, consts::LENGTH> &frame_classes, bool isPedestal);
    void threadTask();
    void processFrame(FullFrame *ptr);
    
    UnorderedFrame<float, consts::LENGTH> pedestal_counter_counting;
    UnorderedFrame<float, consts::LENGTH> pedestal_sum_counting;
    UnorderedFrame<float, consts::LENGTH> pedestal_squared_sum_counting;

    UnorderedFrame<float, consts::LENGTH> pedestal_counter;
    UnorderedFrame<float, consts::LENGTH> pedestal_sum;
    UnorderedFrame<float, consts::LENGTH> pedestal_squared_sum;

    OrderedFrame<float, consts::LENGTH> analog_sum;
    OrderedFrame<float, consts::LENGTH> analog_sum_pumped;
    OrderedFrame<float, consts::LENGTH> thresholded_sum;
    OrderedFrame<float, consts::LENGTH> thresholded_sum_pumped;
    OrderedFrame<int, consts::LENGTH> counting_sum;
    OrderedFrame<int, consts::LENGTH> counting_sum_pumped;
    std::atomic_bool isSplitPumped = false;
    std::atomic_bool isPedestal = true;
    std::atomic_bool threads_sleep = true;

private:
    std::unique_ptr<HDFWriter> hdfWriter;
};
