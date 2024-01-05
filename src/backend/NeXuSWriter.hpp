#pragma once
#include <string>
#include <H5Cpp.h>

class NeXuSWriter {
    void createGroup();
    std::string str = "Hello World! - Привет мир!";
    H5::H5File file(FILE_NAME, H5F_ACC_TRUNC);
    H5::StrType datatype(H5::PredType::C_S1, H5T_VARIABLE);
    H5::DataSpace dataspace(H5S_SCALAR);
    H5::DataSet dataset = file.createDataSet(DATASET_NAME, datatype, dataspace);
    dataset.write(str, datatype);
    // set the dataset attrbiute "Class" to "NXentry"
    H5::Attribute attribute = dataset.createAttribute("Class", datatype, dataspace);
    string str2 = "NXentry";
    attribute.write(datatype, str2);
}