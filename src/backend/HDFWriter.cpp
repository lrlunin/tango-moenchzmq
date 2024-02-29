#include <hdf5/serial/H5Cpp.h>
#include <fmt/core.h>
#include <regex>
#include "HDFWriter.hpp"
#include "ComputationBackend.hpp"

HDFWriter::HDFWriter(std::filesystem::path root_path, std::string file_name):root_path(root_path), file_name(file_name){
    int init_file_index = -1;
    for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(root_path)){
        if (entry.is_regular_file()){
            std::smatch match;
            std::string filename = entry.path().filename().string();
            if (std::regex_search(filename, match, file_match)){
                int last_file_index = std::stoi(match[1]);
                init_file_index = std::max(init_file_index, last_file_index);
            }
        }
    }
    init_file_index++;
    file_index = init_file_index;
};

std::string HDFWriter::getFullFilePath(){
    std::filesystem::path full_file_path = root_path;
    full_file_path /= fmt::format("{}_{:06d}.{}", file_name, file_index, file_extension);
    return full_file_path.lexically_normal();
}
template <typename T, unsigned int V>
void HDFWriter::writeFrame(const std::string frame_name, OrderedFrame<T, V> &frame){
    const std::string group_name = "images";
    const H5::DataType image_datatype(H5::PredType::NATIVE_FLOAT);
    const hsize_t image_dimension[2] = {consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    const H5::DataSpace image_dataspace(2, image_dimension);
  
    std::string full_path = getFullFilePath();
    H5::H5File h5_file(getFullFilePath(), H5F_ACC_TRUNC);
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

template void HDFWriter::writeFrame<float, consts::LENGTH>(const std::string frame_name, OrderedFrame<float, consts::LENGTH> &frame);
