#pragma once
#include <string>
#include <filesystem>
template<typename T, unsigned int V>
class OrderedFrame;

class HDFWriter {
public:
    // file index like 122
    int file_index;
    // directory path like /home/user/20230825_run
    std::filesystem::path root_path;
    // filename like 20230825_run
    std::string file_name;
    // returns path like  /home/user/20230825_run/20230825_run_122.h5 
    std::string getFullFilePath();
    template <typename T, unsigned int V>
    void writeFrame(std::string frame_name, OrderedFrame<T,V> *frame);
private:
    const std::string file_extension = "h5";
};
