#pragma once
#include <string>
#include <filesystem>
#include "ComputationBackend.hpp"
// template<typename T, unsigned int V>
// class OrderedFrame;
#include <hdf5/serial/H5Cpp.h>
#include <fmt/core.h>

class HDFWriter {
public:
    // file index like 122
    int file_index;
    // directory path like /home/user/20230825_run
    std::filesystem::path root_path;
    // filename like 20230825_run
    std::string file_name;
    // returns path like  /home/user/20230825_run/20230825_run_122.h5 
    std::string getFullFilePath(){
    std::filesystem::path full_file_path = this->root_path;
    full_file_path /= fmt::format("{}_{:06d}.{}", this->file_name, this->file_index, this->file_extension);
    return full_file_path.lexically_normal();
}
    template <typename T, unsigned int V>
    void writeFrame(const std::string frame_name, OrderedFrame<T,V> &frame){
        const std::string group_name = "images";
        const H5::DataType image_datatype(H5::PredType::NATIVE_FLOAT);
        const hsize_t image_dimension[2] = {400, 400};
        const H5::DataSpace image_dataspace(2, image_dimension);
        std::string full_path = getFullFilePath();
        H5::H5File h5_file(getFullFilePath(), H5F_ACC_TRUNC);
        if (!h5_file.exists(group_name)) h5_file.createGroup(group_name);
        std::string image_path = fmt::format("{}/{}", group_name, frame_name);
        H5::DataSet dataset = h5_file.createDataSet(image_path, image_datatype, image_dataspace);
        dataset.write(frame.arr, image_datatype, image_dataspace);
    };
private:
    const std::string file_extension = "h5";
};
