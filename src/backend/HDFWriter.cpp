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
    std::filesystem::path fulld_directory_path = buildFullDirectoryPath();
    if (!std::filesystem::exists(fulld_directory_path)) std::filesystem::create_directory(fulld_directory_path);
    int init_file_index = -1;
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(fulld_directory_path)){
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
    return full_directory_path;
};
std::filesystem::path HDFWriter::buildFullFilePath() {
    std::filesystem::path full_file_path = buildFullDirectoryPath();
    full_file_path /= fmt::format(fmt::runtime(file_format), fmt::arg("file_name", file_name), fmt::arg("file_index", file_index));
    return full_file_path.lexically_normal();
};

void HDFWriter::writeFrame(const std::string frame_name, OrderedFrame<float, consts::LENGTH> &frame) {
    const std::string group_name = "images";
    const H5::DataType image_datatype(H5::PredType::NATIVE_FLOAT);
    const hsize_t image_dimension[2] = {consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    const H5::DataSpace image_dataspace(2, image_dimension);

    std::string full_file_path = buildFullFilePath().lexically_normal();
    H5::H5File h5_file(full_file_path, H5F_ACC_TRUNC);
    if (!h5_file.exists(group_name)) h5_file.createGroup(group_name);
    std::string image_path = fmt::format("{}/{}", group_name, frame_name);
    H5::DataSet dataset = h5_file.createDataSet(image_path, image_datatype, image_dataspace);
    dataset.write(frame.arr, image_datatype, image_dataspace);
    
    // H5::DataSet dataset = file.createDataSet(DATASET_NAME, datatype, dataspace);
    // dataset.write(str, datatype);
    // // set the dataset attrbiute "Class" to "NXentry"
    // H5::Attribute attribute = dataset.createAttribute("Class", datatype, dataspace);
    // string str2 = "NXentry";
    // attribute.write(datatype, str2);
};
