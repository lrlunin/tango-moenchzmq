#pragma once
#include <boost/lockfree/queue.hpp>
#include <boost/pool/singleton_pool.hpp>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <filesystem>

namespace consts{
    constexpr int FRAME_WIDTH = 400;
    constexpr int FRAME_HEIGHT = 400;
    constexpr unsigned int LENGTH = FRAME_HEIGHT * FRAME_WIDTH;
    constexpr float PEDESTAL_BUFFER_SIZE = 1000;
}
template <typename T, unsigned int V>
struct OrderedFrame{
    T arr[V];
    T& operator()(int y, int x) {
        return arr[y * consts::FRAME_WIDTH + x];
    }
    template <typename B>
    void copy_to_buffer(B &buffer, bool flip = true){
        for (int y = 0; y < consts::FRAME_HEIGHT; y++){
            for (int x = 0; x < consts::FRAME_WIDTH; x++){
                /*
                to flip image vertically we need either running y from 0 to 399 or 399 to 0
                if not flipped then flip_arr[y * consts::FRAME_WIDTH + x] = arr[y * consts::FRAME_WIDTH + x]
                if flipped then flip_arr[y * consts::FRAME_WIDTH + x] = arr[(consts::FRAME_HEIGHT - 1 - y) * consts::FRAME_WIDTH + consts::FRAME_WIDTH  + x]
                the resulting formula below is just some math trickery to avoid if coniditions
                */
                buffer[y * consts::FRAME_WIDTH + x] = arr[flip * (consts::FRAME_WIDTH * (consts::FRAME_HEIGHT - 1)) + consts::FRAME_WIDTH * y *(-1 + 2 * (!flip)) + x];
            }
        }
    };
    OrderedFrame& operator+=(const OrderedFrame& rhs){
        for (unsigned int i = 0; i < V; i++){
            arr[i] += rhs.arr[i];
        }
        return *this;
    }
    OrderedFrame<float, consts::LENGTH> operator-(const OrderedFrame<float, consts::LENGTH>& rhs){
        OrderedFrame<float, consts::LENGTH> result;
        for (unsigned int i = 0; i < V; i++){
            result.arr[i] = static_cast<float>(arr[i]) - rhs.arr[i];
        }
        return result;
    }
    OrderedFrame& operator-=(const OrderedFrame& rhs){
        for (unsigned int i = 0; i < V; i++){
            arr[i] -= rhs.arr[i];
        }
        return *this;
    }
    void addClass(OrderedFrame<char, V>& classes, const char class_nr){
        for (unsigned int i = 0; i < V; i++){
            arr[i] += (classes.arr[i] == class_nr);
        }
    }
    void zero(){
        std::fill(arr, arr + V, 0);
    }
};
struct Metadata{
    int bitmode;
    int frameIndex;
};
struct FullFrame{
    Metadata m;
    OrderedFrame<unsigned short, consts::LENGTH> f;
};
