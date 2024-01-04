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
#include <hdf5/serial/H5Cpp.h>

using namespace std;

const H5std_string FILE_NAME("your_file.h5");
const H5std_string DATASET_NAME("/your_dataset");

int main() {
    // save an aribatary std::string to a hdf5 file
    string str = "Hello World! - Привет мир!";
    H5::H5File file(FILE_NAME, H5F_ACC_TRUNC);
    H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
    H5::DataSpace dataspace(H5S_SCALAR);
    H5::DataSet dataset = file.createDataSet(DATASET_NAME, datatype, dataspace);
    dataset.write(str, datatype);
    // set the dataset attrbiute "Class" to "NXentry"
    H5::Attribute attribute = dataset.createAttribute("Class", datatype, dataspace);
    string str2 = "NXentry";
    attribute.write(datatype, str2);    
    // read the string back
    // string read_str;
    // dataset.read(read_str, datatype);
    // cout << read_str << endl;
    return 0;

}