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
#include <filesystem>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <hdf5/serial/H5Cpp.h>
#include "NeXuSWriter.hpp"
// #include <hdf5/H

using namespace std;
//using namespace hdf5;


int main() {
    // filesystem::path file_path("h5cpp.h5");
    // hdf5::file::File fl = hdf5::file::create(file_path, hdf5::file::AccessFlags::Truncate);
    // hdf5::node::Group root_group = fl.root();
    // hdf5::dataspace::Simple dspace({3, 10, 10});
    // auto dt = hdf5::datatype::create<float>();
    // hdf5::node::Dataset signal_ds = root_group.create_dataset("signal", dt, dspace);
    // std::vector<float> y(100);
    // std::fill(y.begin(), y.end(), 20);
    // float x = 20;
    // //float y[10] = {20};
    // Dimensions offset{0, 0, 0};
    // Dimensions block{1, 10, 10};

    // dataspace::Hyperslab selection{offset, block};
    // signal_ds.write(y, selection);
    // cout << signal_ds.dataspace().size() << signal_ds.datatype().get_class() << endl;
    // create a 
    H5::H5File file("myfile.h5", H5F_ACC_TRUNC);
    H5::DataType datatype(H5::PredType::NATIVE_FLOAT);
    hsize_t dims[3] = {3, 10, 10};
    hsize_t sub_dims[3] = {1, 10, 10};
    H5::DataSpace dataspace(3, dims);
    H5::DataSpace memspace(3, sub_dims, NULL);
    H5::DataSet dataset = file.createDataSet("signal", datatype, dataspace);
    std::cout << file.exists("signal") << file.exists("signal2") << std::endl;
    dataspace = dataset.getSpace();
    vector<float> data(100);
    std::fill(data.begin(), data.end(), 20);
    hsize_t count[3] = {1, 10, 10};
    hsize_t offset[3] ={0, 0, 0};
    hsize_t block[3] = {1, 1, 1};
    dataspace.selectHyperslab(H5S_SELECT_SET, count, offset, NULL, block);
    dataset.write(data.data(), datatype, memspace, dataspace);

    std::fill(data.begin(), data.end(), 30);
    offset[0] = 1;
    dataspace.selectHyperslab(H5S_SELECT_SET, count, offset, NULL, block);
    dataset.write(data.data(), datatype, memspace, dataspace);

    std::fill(data.begin(), data.end(), 40);
    offset[0] = 2;
    dataspace.selectHyperslab(H5S_SELECT_SET, count, offset, NULL, block);
    dataset.write(data.data(), datatype, memspace, dataspace);
    return 0;
}
