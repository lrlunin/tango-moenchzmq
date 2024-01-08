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
    constexpr int LENGTH = FRAME_HEIGHT * FRAME_WIDTH;
    constexpr float PEDESTAL_BUFFER_SIZE = 1000;
    constexpr static std::array<std::array<unsigned int, FRAME_HEIGHT>, FRAME_WIDTH> reorder_map{[]() constexpr {
        std::array<std::array<unsigned int, FRAME_HEIGHT>, FRAME_WIDTH> result;
        int nadc = 32;
        int adc_nr[32] = {300, 325, 350, 375, 300, 325, 350, 375,
        200, 225, 250, 275, 200, 225, 250, 275,
        100, 125, 150, 175, 100, 125, 150, 175,
        0, 25, 50, 75, 0, 25, 50, 75};
        int npackets = 40;
        int sc_width = 25;
        int sc_height = 200;
        int row;
        for (int ip = 0; ip < npackets; ip++) {
            for (int is = 0; is < 128; is++) {
                for (auto iadc = 0; iadc < nadc; iadc++) {
                    int i = 128 * ip + is;
                    int adc4 = (int)iadc / 4;
                    if (i < sc_width * sc_height) {
                        //  for (int i=0; i<sc_width*sc_height; i++) {
                        int col = adc_nr[iadc] + (i % sc_width);
                        if (adc4 % 2 == 0) {
                            row = 199 - i / sc_width;
                        } else {
                            row = 200 + i / sc_width;
                        }
                        result[row][col] = (nadc * i + iadc);
                    }
                }
            }
        }
        return result;
    }()};
}
template <typename T = unsigned short, unsigned int V = 400 * 400>
struct UnorderedFrame{
    T arr[V]; 
    T& operator()(int y, int x) {
        return arr[consts::reorder_map[y][x]];
    }
    void zero(){
        for (auto i = 0; i<V; i++){
            arr[i] = 0;
        }
    }
};
template <typename T = unsigned short, unsigned int V = consts::FRAME_WIDTH*consts::FRAME_HEIGHT>
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
        for (auto i = 0; i < V; i++){
            arr[i] += rhs.arr[i];
        }
        return *this;
    }
    void addClass(OrderedFrame<char, V>& classes, const char class_nr){
        for (auto i = 0; i<V; i++){
            arr[i] = (classes.arr[i] == class_nr);
        }
    }
    void zero(){
        for (auto i = 0; i<V; i++){
            arr[i] = 0;
        }
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

class ComputationBackend{
public:
    typedef boost::singleton_pool<FullFrame, sizeof(FullFrame)> memory_pool;
    ComputationBackend();
    std::string filepath, filename;
    std::atomic<long> fileindex;
    boost::lockfree::queue<FullFrame*> frame_ptr_queue;
    std::vector<std::thread> threads;
    std::shared_mutex pedestal_share;
    std::mutex frames_sums;
    std::atomic<long> processed_frames_amount;

    int THREAD_AMOIUNT = 10;
    void init_threads();
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
    void thread_task();
    void process_frame(FullFrame *ptr);
    
    UnorderedFrame<float, consts::LENGTH> pedestal_counter;
    UnorderedFrame<float, consts::LENGTH> pedestal_sum;
    UnorderedFrame<float, consts::LENGTH> pedestal_squared_sum;
    OrderedFrame<float, consts::LENGTH> analog_sum;
    OrderedFrame<float, consts::LENGTH> thresholded_sum;
    OrderedFrame<int, consts::LENGTH> counting_sum;
    std::atomic_bool isPedestal = true;
    std::atomic_bool threads_sleep = true;
};