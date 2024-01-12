#pragma once
#include <string>
#include "ComputationBackend.hpp"
#include <filesystem>

class NeXuSWriter {
public:
    int file_index;
    void createDatastructure();
    void createGroupIfNotExists();
    void writeArray(int axis, OrderedFrame<float, consts::LENGTH> *frame);
    std::filesystem::path file_path;
private:
    const int axis_amount = 8;
    std::string getFullFilePath();
    std::string getBaseEntryPath();
    std::string getDataGroupPath();
    std::string getDataSetPath();
    std::string getMetaDataPath();
};  
