#pragma once
#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <filesystem>
#include "Frames.hpp"

class FileWriter;
class ComputationBackend{
public:
    typedef boost::singleton_pool<FullFrame, sizeof(FullFrame)> memory_pool;
    ComputationBackend(std::string save_root_path);
    ComputationBackend(FileWriter *hdfWriter);
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
    std::unique_ptr<FileWriter> hdfWriter;
};
