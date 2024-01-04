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
#include "ZMQListener.hpp"
#include <chrono>
#include <nexus/NeXusFile.hpp>

using namespace std;
int main(){
    unique_ptr<ZMQListener> zmq_listener = make_unique<ZMQListener>("tcp://127.0.0.1:5555");
    bool flag;
    cin >> flag; 
}
