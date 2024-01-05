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
#include <h5cpp/hdf5.hpp>

using namespace std;



int main() {
    auto type = hdf5::datatype::TypeTrait<int>::create();
      std::cout<<type.get_class()<<std::endl;
    return 0;

}