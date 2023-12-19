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

#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>

const int LENGTH = 16000;
struct frame{
    unsigned short data[LENGTH];
};

struct metadata{
    int bitmode;
    int frameIndex;
};
struct frames_pool{};
struct full_data{
    metadata m;
    frame f;
};
void increment(int *value){
    std::cout << "Hello from thread" << std::endl;
    (*value)+=10;
};

class Test: public std::thread{

};

int main(){
    boost::lockfree::queue<full_data*> boost_queue(100);
    sls::zmqHeader c;
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_SUB);
    zmq::message_t payload;
    socket.connect("tcp://127.0.0.1:5555");
    socket.set(zmq::sockopt::subscribe, "");
    char counter = 0;
    int value = 0;
    std::thread t1 (increment, &value);
    t1.join();
    std::cout << value << std::endl;

    // avoid memory fragmentation with memory pool instead of call new for each new full_data
    // if even needed?...
    typedef boost::singleton_pool<full_data, sizeof(full_data)> memory_pool;
    std::vector<full_data*> ptr_list;
    for (int i=0; i < 20; ++i){
        full_data *pool_ptr = static_cast<full_data*>(memory_pool::malloc());
        std::cout << "iter " << i << ", malloc on addr " << &pool_ptr << std::endl;
        pool_ptr->m.bitmode = i*i;
        pool_ptr->m.frameIndex = i*i;
        ptr_list.push_back(pool_ptr);
        
    };
    
    // for (std::vector<full_data*>::reverse_iterator ptr = ptr_list.rbegin(); ptr!=ptr_list.rend(); ptr++){
    //     std::cout << "bm: " << (*ptr)->m.bitmode << " fm " << (*ptr)->m.frameIndex << std::endl;
    //     memory_pool::free((*ptr));
    // }
    for (auto ptr : ptr_list){
        std::cout << "ptr " << &ptr << ", bm: " << ptr->m.bitmode << " fm " << ptr->m.frameIndex << std::endl;
        memory_pool::free(ptr);
    }

    // while (counter < 100){
    //     zmq::message_t zmq_msg_1, zmq_msg_2;
    //     //message *msg_ptr = new message();
    //     // expceted to be a json
    //     rapidjson::Document d;
    //     socket.recv(zmq_msg_1);
    //     if (d.Parse(static_cast<char*>(zmq_msg_1.data()), zmq_msg_1.size()).HasParseError()){
    //         std::cout << "Has parsed error" << std::endl;
    //         continue;
    //     };
    //     std::cout << "Received json, counter: " << counter << " frameIndex " << d["frameIndex"].GetUint64() << " nextData " << (d["data"].GetUint() == 1) << std::endl;

    //     if (d["data"].GetUint() == 1){
    //         std::cout << "Received data, counter: " << counter << std::endl;
    //         socket.recv(zmq_msg_2);
    //         full_data *f_data = new full_data();
    //         f_data->m.frameIndex = d["frameIndex"].GetUint64();
    //         f_data->m.bitmode = d["bitmode"].GetUint();
    //         std::memcpy(f_data->f.data, zmq_msg_2.data(), std::min(zmq_msg_2.size(), static_cast<size_t>(sizeof(frame::data))));
    //         boost_queue.push(f_data);
    //         ++counter;
    //     };
    //     full_data *cur_ptr;
    //     if (boost_queue.pop(cur_ptr)){
    //         std::cout << "frame_index:  " << cur_ptr->m.frameIndex << " frame avg: " << std::accumulate(cur_ptr->f.data, cur_ptr->f.data+LENGTH, 0)/LENGTH   << std::endl;
    //         delete cur_ptr;
    //     }
    // }
    // std::tuple b {std::string("Shark"), 1.0, 44};
    // std::cout << std::get<std::string>(b) << std::get<double>(b) << std::get<int>(b) << std::endl;  
    // std::tuple d {std::string("Shark"), -22., 33};
    // std::cout << std::get<std::string>(d) << std::get<1>(d) << std::get<int>(d) << std::endl;  
    // return 0;
}
