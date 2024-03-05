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
#include "HDFWriter.hpp"
#include <regex>
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
    // H5::H5File file("myfile.h5", H5F_ACC_TRUNC);
    // H5::DataType datatype(H5::PredType::NATIVE_FLOAT);
    // hsize_t dims[3] = {3, 10, 10};
    // hsize_t sub_dims[3] = {1, 10, 10};
    // H5::DataSpace dataspace(3, dims);
    // H5::DataSpace memspace(3, sub_dims, NULL);
    // H5::DataSet dataset = file.createDataSet("signal", datatype, dataspace);
    // std::cout << file.exists("signal") << file.exists("signal2") << std::endl;
    // dataspace = dataset.getSpace();
    // vector<float> data(100);
    // std::fill(data.begin(), data.end(), 20);
    // hsize_t count[3] = {1, 10, 10};
    // hsize_t offset[3] ={0, 0, 0};
    // hsize_t block[3] = {1, 1, 1};
    // dataspace.selectHyperslab(H5S_SELECT_SET, count, offset, NULL, block);
    // dataset.write(data.data(), datatype, memspace, dataspace);

    // std::fill(data.begin(), data.end(), 30);
    // offset[0] = 1;
    // dataspace.selectHyperslab(H5S_SELECT_SET, count, offset, NULL, block);
    // dataset.write(data.data(), datatype, memspace, dataspace);

    // std::fill(data.begin(), data.end(), 40);
    // offset[0] = 2;
    // dataspace.selectHyperslab(H5S_SELECT_SET, count, offset, NULL, block);
    // dataset.write(data.data(), datatype, memspace, dataspace);
    
    // const std::string group_name = "images";
    // const H5::DataType image_datatype(H5::PredType::NATIVE_FLOAT);
    // const hsize_t image_dimension[2] = {400, 400};
    // const H5::DataSpace image_dataspace(2, image_dimension);
  

    // OrderedFrame<float, 400*400>* frame = new OrderedFrame<float, 400*400>();
    // for (int y = 0; y< 400; y++){
    //     for (int x = 0; x<400; x++){
    //         frame->arr[x+400*y]= y+x;
    //     }
    // }
    // std::string full_path = "images_file.h5";
    // H5::H5File h5_file(full_path, H5F_ACC_TRUNC);
    // if (!h5_file.exists(group_name)) h5_file.createGroup(group_name);
    // std::string image_path = fmt::format("{}/{}", group_name, "super_analog");
    // H5::DataSet image_dataset = h5_file.createDataSet(image_path, image_datatype, image_dataspace);
    // image_dataset.write(frame->arr, image_datatype, image_dataspace);
    // delete frame;

    H5::H5File file;
    file.close();
    return 0;
}
