#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <thread>
#include <shared_mutex>
#include <vector>


namespace consts{
    constexpr int FRAME_WIDTH = 400;
    constexpr int FRAME_HEIGHT = 400;
    constexpr int LENGTH = FRAME_HEIGHT * FRAME_WIDTH;
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
    T arr[V] = {0}; 
    T& operator()(int y, int x) {
        return arr[consts::reorder_map[y][x]];
    }
};
template <typename T = unsigned short, unsigned int V = 400*400>
struct OrderedFrame{
    T arr[V] = {0};
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
    UnorderedFrame<unsigned short, consts::LENGTH> f;
};

class ComputationBackend{
public:
    typedef boost::singleton_pool<FullFrame, sizeof(FullFrame)> memory_pool;
    ComputationBackend();
    boost::lockfree::queue<FullFrame*> frame_ptr_queue;
    std::vector<std::thread> threads;
    std::shared_mutex pedestal_share;
    std::mutex frames_sums;
    int processed_frames_amount = 0;

    void init_threads();
    void pause();
    void resume();
    void loadPedestalAndRMS(UnorderedFrame<float, consts::LENGTH> &pedestal, UnorderedFrame<float, consts::LENGTH> &pedestal_rms);
    OrderedFrame<char, consts::LENGTH> classifyFrame(OrderedFrame<float, consts::LENGTH> &input, UnorderedFrame<float, consts::LENGTH> &pedestal_rms);
    OrderedFrame<float, consts::LENGTH> subtractPedestal(UnorderedFrame<unsigned short, consts::LENGTH> &raw_frame, UnorderedFrame<float, consts::LENGTH> &pedestal);
    void updatePedestal(UnorderedFrame<unsigned short, consts::LENGTH> &raw_frame, OrderedFrame<char, consts::LENGTH> &frame_classes, bool isPedestal);
    void thread_task();
    void process_frame(FullFrame *ptr);
    
    const int pedestal_buff_size = 300;
    UnorderedFrame<float, consts::LENGTH> pedestal_counter = {0};
    UnorderedFrame<float, consts::LENGTH> pedestal_sum = {0};
    UnorderedFrame<float, consts::LENGTH> pedestal_squared_sum = {0};
    OrderedFrame<float, consts::LENGTH> analog_sum = {0};
    OrderedFrame<int, consts::LENGTH> counting_sum = {0};

    std::atomic_bool isPedestal = true;
    std::atomic_bool sleep = true;
   
};