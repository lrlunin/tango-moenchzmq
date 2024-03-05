#pragma once
#include <string>
#include <filesystem>
#include <hdf5/serial/H5Cpp.h>
#include <fmt/core.h>
#include <regex>
#include "Frames.hpp"

class FileWriter {
protected:
    // file extension like h5
    std::string file_extension;
public:
    // file index like 122
    int file_index;
    // filename like 20230825_run
    std::string file_name;
    // session directory path like 20230825_run
    std::string session_directory;
    // directory path like /home/user/20230825_run
    std::filesystem::path root_path;
    FileWriter(int file_index, std::string session_directory, std::string file_name, std::filesystem::path root_path):file_index(file_index), file_name(file_name), session_directory(session_directory), root_path(root_path){};
    FileWriter(int file_index, std::filesystem::path root_path):file_index(file_index), root_path(root_path){};
    FileWriter(std::filesystem::path root_path):root_path(root_path){};
    // format string like {file_name}_{file_index:06d}.h5 with file_name and file_index as arguments as constexpr
    const std::string file_format;
    // regex to match file names like 20230825_run_122.h5
    const std::regex file_match;
    virtual std::filesystem::path buildFullFilePath() = 0;
    virtual std::filesystem::path buildFullDirectoryPath() = 0;
    virtual ~FileWriter() {};
    virtual void openFile() = 0;
    virtual void closeFile() = 0;
    virtual void writeFrame(const std::string group_name, const std::string frame_name, OrderedFrame<float, consts::LENGTH> &frame) = 0;
    virtual void writeFrameStack(const std::string group_name, const std::string frame_stack_name, float* frame_stack_ptr, size_t frame_stack_length) = 0;
    virtual void writeFrameStack(const std::string group_name, const std::string frame_stack_name, char* frame_stack_ptr, size_t frame_stack_length) = 0;
};
