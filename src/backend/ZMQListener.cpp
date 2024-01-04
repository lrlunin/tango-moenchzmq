#include "ZMQListener.hpp"
#include <iostream>
ZMQListener::ZMQListener(std::string socket_addr){
    context = zmq::context_t(1);
    socket = zmq::socket_t(context, ZMQ_SUB);
    socket.connect(socket_addr.c_str());
    socket.set(zmq::sockopt::subscribe, "");
    comp_backend_ptr = std::make_unique<ComputationBackend>();
    receive_data = false;

    zmq_listener_thread = std::thread(&ZMQListener::listen_socket, this);
}
void ZMQListener::listen_socket(){
    zmq::message_t zmq_msg_1, zmq_msg_2;
    rapidjson::Document d;
    while (true){
        if (socket.recv(zmq_msg_1)){
            if (d.Parse(static_cast<char*>(zmq_msg_1.data()), zmq_msg_1.size()).HasParseError()){
            continue;
            };
            std::cout << "Received json, frameindex: " << d["frameIndex"].GetUint64() << " nextData " << (d["data"].GetUint() == 1) << std::endl;
        }
        if (d["data"].GetUint() == 1){
                std::cout << "Received data" << std::endl;
                if (socket.recv(zmq_msg_2) && receive_data){
                    FullFrame *ff_ptr = static_cast<FullFrame*>(ComputationBackend::memory_pool::malloc());
                    ff_ptr->m.frameIndex = d["frameIndex"].GetUint64();
                    ff_ptr->m.bitmode = d["bitmode"].GetUint();
                    std::memcpy(ff_ptr->f.arr, zmq_msg_2.data(), std::min(zmq_msg_2.size(), static_cast<size_t>(sizeof(FullFrame::f.arr))));
                    comp_backend_ptr->frame_ptr_queue.push(ff_ptr);
            }
        };
    }
}