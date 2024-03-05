#include <hdf5/serial/H5Cpp.h>
#include <fmt/core.h>
#include <regex>
#include "ComputationBackend.hpp"
#include "HDFWriter.hpp"
#include <chrono>
#include <fmt/chrono.h>


HDFWriter::HDFWriter(std::filesystem::path root_path):HDFWriter(-1, root_path){};
HDFWriter::HDFWriter(int file_index, std::filesystem::path root_path):FileWriter(file_index, root_path){
    const auto now = std::chrono::system_clock::now();
    file_name = fmt::format("{:%Y%m%d}_run", now);
    session_directory = fmt::format("{:%Y%m%d}_run", now);
    std::filesystem::path full_directory_path = buildFullDirectoryPath();
    int init_file_index = -1;
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(full_directory_path)){
        if (entry.is_regular_file()){
            std::smatch match;
            std::string filename = entry.path().filename().string();
            if (std::regex_search(filename, match, file_match)){
                int last_file_index = std::stoi(match[1]);
                init_file_index = std::max(init_file_index, last_file_index);
            }
        }
    }
    init_file_index = std::max(init_file_index, file_index);
    init_file_index++;
    this->file_index = init_file_index;
};
std::filesystem::path HDFWriter::buildFullDirectoryPath() {
    std::filesystem::path full_directory_path = root_path;
    full_directory_path /= session_directory;
    if (!std::filesystem::exists(full_directory_path)) std::filesystem::create_directory(full_directory_path);
    return full_directory_path;
};
std::filesystem::path HDFWriter::buildFullFilePath() {
    std::filesystem::path full_file_path = buildFullDirectoryPath();
    full_file_path /= fmt::format(fmt::runtime(file_format), fmt::arg("file_name", file_name), fmt::arg("file_index", file_index));
    return full_file_path;
};
void HDFWriter::openFile(){
    current_file.close();
    std::filesystem::path full_file_path = buildFullFilePath();
    unsigned int open_flag = (std::filesystem::exists(full_file_path)) ? H5F_ACC_RDWR : H5F_ACC_TRUNC;
    current_file = H5::H5File(full_file_path, open_flag);
};
void HDFWriter::closeFile(){
    current_file.close();
};
void HDFWriter::writeFrame(const std::string group_name, const std::string frame_name, OrderedFrame<float, consts::LENGTH> &frame) {
    std::unique_ptr<float[]> flipped_buffer(new float[consts::LENGTH]);
    float* buf_ptr = flipped_buffer.get();
    frame.copy_to_buffer<float*>(buf_ptr, true);
    const H5::DataType image_datatype(H5::PredType::NATIVE_FLOAT);
    const hsize_t image_dimension[2] = {consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    const H5::DataSpace image_dataspace(2, image_dimension);

    if (!current_file.exists(group_name)) current_file.createGroup(group_name);
    std::string image_path = fmt::format("{}/{}", group_name, frame_name);
    H5::DataSet dataset = current_file.createDataSet(image_path, image_datatype, image_dataspace);
    dataset.write(buf_ptr, image_datatype, image_dataspace);
};

void HDFWriter::writeFrameStack(const std::string group_name, const std::string frame_stack_name, float* frame_stack_ptr, size_t frame_stack_length){   
    const H5::DataType image_datatype(H5::PredType::NATIVE_FLOAT);
    const hsize_t image_stack_dimension[3] = {static_cast<hsize_t>(frame_stack_length), consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    const H5::DataSpace image_stack_dataspace(3, image_stack_dimension);
    
    const hsize_t image_single_dimension[3] = {1, consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    const H5::DataSpace image_single_dataspace(3, image_single_dimension, NULL);
    if (!current_file.exists(group_name)) current_file.createGroup(group_name);
    std::string image_path = fmt::format("{}/{}", group_name, frame_stack_name);
    H5::DataSet dataset = current_file.createDataSet(image_path, image_datatype, image_stack_dataspace);
    dataset.write(frame_stack_ptr, image_datatype, image_stack_dataspace);
};

void HDFWriter::writeFrameStack(const std::string group_name, const std::string frame_stack_name, char* frame_stack_ptr, size_t frame_stack_length){   
    const H5::DataType image_datatype(H5::PredType::NATIVE_CHAR);
    const hsize_t image_stack_dimension[3] = {static_cast<hsize_t>(frame_stack_length), consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    const H5::DataSpace image_stack_dataspace(3, image_stack_dimension);
    
    const hsize_t image_single_dimension[3] = {1, consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    const H5::DataSpace image_single_dataspace(3, image_single_dimension, NULL);
    H5::H5File h5_file(buildFullFilePath(), H5F_ACC_RDWR);
    if (!h5_file.exists(group_name)) h5_file.createGroup(group_name);
    std::string image_path = fmt::format("{}/{}", group_name, frame_stack_name);
    H5::DataSet dataset = h5_file.createDataSet(image_path, image_datatype, image_stack_dataspace);
    dataset.write(frame_stack_ptr, image_datatype, image_stack_dataspace);
};
