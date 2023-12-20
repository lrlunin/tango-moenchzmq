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
struct Frame{
    T arr[V];
    // will it work with assigning so?..
    &T operator()(int y, int x){
        return arr[consts::reorder_map[y][x]];
    }
};
template <typename T = unsigned short, unsigned int = 400*400>
struct ProcessingResult{
    T arr[V];
    &T operator()(int y, int x){
        return arr[consts::reorder_map[y][x]];
    }
};
struct Metadata{
    int bitmode;
    int frameIndex;
};
struct FullFrame{
    Metadata m;
    Frame<unsigned short, LENGTH> f;
};
class ComputationBackend{
public:
    std::vector<std::thread> threads;
    ComputationBackend();
    void init_threads();
    void pause();
    void resume();
    void calcPedestal(int (&arr)[LENGTH]);
    void classifyFrame(const Frame<unsigned short, LENGTH> &input, int (&output)[LENGTH]);
    void thread_task();
    void process_frame(FullFrame *ptr);
    typedef boost::singleton_pool<FullFrame, sizeof(FullFrame)> memory_pool;
    unsigned int pedestal_buff_size = 100;
    unsigned int pedestal_counter[LENGTH] = {0};
    int pedestal_frame[LENGTH] = {0};
    int devided_pedestal[LENGTH];
    boost::lockfree::queue<FullFrame*> frame_ptr_queue;
    std::atomic_bool pedestal = true;
    std::atomic_bool sleep = true;
    std::shared_mutex pedestal_share;
};