#include <iostream>
#include <zmq.hpp>
#include <string>
#include <queue>
#include <sls/ZmqSocket.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <algorithm>

#include <boost/lockfree/queue.hpp>

const int LENGTH = 16000;
struct frame{
    unsigned short data[LENGTH];
};

struct metadata{
    int bitmode;
    int frameIndex;
};

struct full_data{
    metadata m;
    frame f;
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
    
    
    while (counter < 100){
        zmq::message_t zmq_msg_1, zmq_msg_2;
        //message *msg_ptr = new message();
        // expceted to be a json
        rapidjson::Document d;
        socket.recv(zmq_msg_1);
        if (d.Parse(static_cast<char*>(zmq_msg_1.data()), zmq_msg_1.size()).HasParseError()){
            std::cout << "Has parsed error" << std::endl;
            continue;
        };
        std::cout << "Received json, counter: " << counter << " frameIndex " << d["frameIndex"].GetUint64() << " nextData " << (d["data"].GetUint() == 1) << std::endl;

        if (d["data"].GetUint() == 1){
            std::cout << "Received data, counter: " << counter << std::endl;
            socket.recv(zmq_msg_2);
            full_data *f_data = new full_data();
            f_data->m.frameIndex = d["frameIndex"].GetUint64();
            f_data->m.bitmode = d["bitmode"].GetUint();
            std::memcpy(f_data->f.data, zmq_msg_2.data(), std::min(zmq_msg_2.size(), static_cast<size_t>(sizeof(frame::data))));
            boost_queue.push(f_data);
            ++counter;
        };
        full_data *cur_ptr;
        if (boost_queue.pop(cur_ptr)){
            std::cout << "frame_index:  " << cur_ptr->m.frameIndex << " frame avg: " << std::accumulate(cur_ptr->f.data, cur_ptr->f.data+LENGTH, 0)/LENGTH   << std::endl;
            delete cur_ptr;
        }
    }
    return 0;
}
