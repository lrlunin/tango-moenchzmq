#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <thread>
#include <shared_mutex>
#include <vector>

constexpr int LENGTH = 160000;
namespace consts{
    constexpr static std::array<std::array<unsigned int, 400>, 400> reorder_map{[]() constexpr {
        std::array<std::array<unsigned int, 400>, 400> result;
        int nadc = 32;
        int adc_nr[32] = {300, 325, 350, 375, 300, 325, 350, 375,
        200, 225, 250, 275, 200, 225, 250, 275,
        100, 125, 150, 175, 100, 125, 150, 175,
        0, 25, 50, 75, 0, 25, 50, 75};
        int npackets = 40;
        int sc_width = 25;
        int sc_height = 200;
        int col, row;
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
};
template <typename T = unsigned short, unsigned int V = 400*400>
struct OrderedFrame{
    T arr[V];
    T& operator()(int y, int x) {
        return arr[y*400 + x];
    }
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
};
struct Metadata{
    int bitmode;
    int frameIndex;
};
struct FullFrame{
    Metadata m;
    UnorderedFrame<unsigned short, LENGTH> f;
};
class ComputationBackend{
public:
    typedef boost::singleton_pool<FullFrame, sizeof(FullFrame)> memory_pool;
    ComputationBackend();
    boost::lockfree::queue<FullFrame*> frame_ptr_queue;
    std::vector<std::thread> threads;
    std::shared_mutex pedestal_share;
    std::shared_mutex frames_sums;
    int processed_frames_amount = 0;

    void init_threads();
    void pause();
    void resume();
    UnorderedFrame<float, LENGTH> getPedestal();
    OrderedFrame<char, LENGTH> classifyFrame(OrderedFrame<float, LENGTH> &input);
    OrderedFrame<float, LENGTH> subtractPedestal(UnorderedFrame<unsigned short, LENGTH> &raw_frame, UnorderedFrame<float, LENGTH> &pedestal);
    void updatePedestal(UnorderedFrame<unsigned short, LENGTH> &raw_frame, OrderedFrame<char, LENGTH> &frame_classes, bool isPedestal);
    void thread_task();
    void process_frame(FullFrame *ptr);
    
    const unsigned int pedestal_buff_size = 100;
    OrderedFrame<unsigned int, 400*400> pedestal_counter = {0};
    OrderedFrame<int, 400*400> pedestal_sum = {0};
    OrderedFrame<float, 400*400> analog_sum = {0};
    OrderedFrame<int, 400*400> counting_sum = {0};

    std::atomic_bool pedestal = true;
    std::atomic_bool sleep = true;
   
};