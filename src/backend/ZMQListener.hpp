#pragma once
#include <zmq.hpp>
#include <string>
#include <queue>
#include <sls/ZmqSocket.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <algorithm>
#include <vector>
#include <thread>
#include <tuple>
#include <shared_mutex>
#include <syncstream>
#include <condition_variable>
#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>
#include "CPUComputationBackend.hpp"

class ZMQListener {
    public:
        ZMQListener(std::string socket_addr, std::string socket_port);
        void listen_socket();
        void start_receive();
        void stop_receive();
        void abort_receive();
        std::atomic<long> received_frames_amount;
        std::unique_ptr<CPUComputationBackend> comp_backend_ptr;
    private:
        zmq::context_t context;
        zmq::socket_t socket;
        std::thread zmq_listener_thread;
        std::atomic_bool receive_data;
        std::atomic_bool abort_wait;
};
