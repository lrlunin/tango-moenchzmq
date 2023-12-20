#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <thread>
#include <shared_mutex>
#include <vector>

const int LENGTH = 16000;

struct Metadata{
    int bitmode;
    int frameIndex;
};
struct FullFrame{
    Metadata m;
    unsigned short arr[LENGTH];
};
class ComputationBackend{
public:
    std::vector<std::thread> threads;
    ComputationBackend();
    void init_threads();
    void pause();
    void resume();
    void calcPedestal(int (&arr)[16000]);
    void thread_task();
    typedef boost::singleton_pool<FullFrame, sizeof(FullFrame)> memory_pool;
    unsigned int pedestal_buff_size = 100;
    unsigned int pedestal_counter[16000] = {0};
    int pedestal_frame[16000] = {0};
    int devided_pedestal[16000];
    boost::lockfree::queue<FullFrame*> frame_ptr_queue;
    std::atomic_bool pedestal = true;
    std::atomic_bool sleep = true;
    std::shared_mutex sm;
};