#include <iostream>
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
#include <cmath>
#include "ComputationBackend.hpp"
#include <chrono>

using namespace std;

template<typename T, int V>
struct Test{
    T arr[V];
    // will it work with assigning so?..
    T& operator()(int y, int x){
        return arr[y*3 + x];
    }
};

int main(){
    
    sls::zmqHeader c;
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_SUB);
    zmq::message_t payload;
    socket.connect("tcp://127.0.0.1:5555");
    socket.set(zmq::sockopt::subscribe, "");
    bool flag = false;
    unique_ptr<ComputationBackend> comp_backend(new ComputationBackend());
    int i = 0;
    while (i < 100){
        zmq::message_t zmq_msg_1, zmq_msg_2;
        //message *msg_ptr = new message();
        // expceted to be a json
        rapidjson::Document d;
        socket.recv(zmq_msg_1);
        if (d.Parse(static_cast<char*>(zmq_msg_1.data()), zmq_msg_1.size()).HasParseError()){
            std::cout << "Has parsed error" << std::endl;
            continue;
        };
        std::cout << "Received json, counter: " << i << " frameIndex " << d["frameIndex"].GetUint64() << " nextData " << (d["data"].GetUint() == 1) << std::endl;

        if (d["data"].GetUint() == 1){
            std::cout << "Received data, counter: " << i << std::endl;
            socket.recv(zmq_msg_2);
            FullFrame *ff_ptr = static_cast<FullFrame*>(ComputationBackend::memory_pool::malloc());
            ff_ptr->m.frameIndex = d["frameIndex"].GetUint64();
            ff_ptr->m.bitmode = d["bitmode"].GetUint();
            std::memcpy(ff_ptr->f.arr, zmq_msg_2.data(), std::min(zmq_msg_2.size(), static_cast<size_t>(sizeof(FullFrame::f.arr))));
            comp_backend->frame_ptr_queue.push(ff_ptr);
            i++;
        };
    }
    comp_backend->sleep = false;
    comp_backend->init_threads();
    cin >> flag;
}
