#include <hdf5/serial/H5Cpp.h>
#include <fmt/core.h>

#include "NeXuSWriter.hpp"
#include "ComputationBackend.hpp"
std::string NeXuSWriter::getFullFilePath(){
    std::filesystem::path file_path = this->file_path;
    file_path /= ".nxs";
    return file_path.lexically_normal();
}
std::string NeXuSWriter::getBaseEntryPath(){
    return fmt::format("/entry{}", file_index);
}
std::string NeXuSWriter::getDataGroupPath(){
    return fmt::format("{}/data", getBaseEntryPath());
}
std::string NeXuSWriter::getDataSetPath(){
    return fmt::format("{}/signal", getDataGroupPath());
}
void NeXuSWriter::createDatastructure(){
    H5::StrType attr_datatype(H5::PredType::C_S1, H5T_VARIABLE);
    H5::DataSpace attr_dataspace(H5S_SCALAR);

    hsize_t data_dims[] = {3, consts::FRAME_HEIGHT, consts::FRAME_WIDTH};
    H5::DataType data_datatype(H5::PredType::NATIVE_FLOAT);
    H5::DataSpace data_dataspace(3, data_dims);

    H5::H5File file(getFullFilePath(), H5F_ACC_TRUNC);
    if (!file.exists(getBaseEntryPath())) file.createGroup(getBaseEntryPath())
                                              .createAttribute("NX_class", attr_datatype, attr_dataspace)
                                              .write(attr_datatype, "NXentry");
    if (!file.exists(getDataGroupPath())) {
        H5::Group data_group = file.createGroup(getDataGroupPath());
        data_group.createAttribute("NX_class", attr_datatype, attr_dataspace).write(attr_datatype, "NXdata");
        data_group.createAttribute("axes", attr_datatype, attr_dataspace).write(attr_datatype, "['axis1', 'axis2', 'axis3', 'axis4', 'axis5', 'axis6', 'axis7', 'axis8']");
        data_group.createAttribute("signal", attr_datatype, attr_dataspace).write(attr_datatype, "signal");
        data_group.createDataSet("signal", data_datatype, data_dataspace);
    }
}
void NeXuSWriter::writeArray(int axis, OrderedFrame<float, consts::LENGTH> *frame){
    H5::H5File file(getFullFilePath(), H5F_ACC_TRUNC);
    H5::DataSet dataset = file.openDataSet(getDataSetPath());
    // H5::DataSet dataset = file.createDataSet(DATASET_NAME, datatype, dataspace);
    // dataset.write(str, datatype);
    // // set the dataset attrbiute "Class" to "NXentry"
    // H5::Attribute attribute = dataset.createAttribute("Class", datatype, dataspace);
    // string str2 = "NXentry";
    // attribute.write(datatype, str2);
}
