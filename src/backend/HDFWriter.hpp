#pragma once
#include <string>
#include <filesystem>
#include <hdf5/serial/H5Cpp.h>
#include <fmt/core.h>
#include <regex>
#include "FileWriter.hpp"
#include "Frames.hpp"

class HDFWriter : public FileWriter {
protected:
    std::string buildFullFilePath() override;
public:
    HDFWriter(int file_index, std::filesystem::path root_path);
    ~HDFWriter() {};
    void writeFrame(const std::string frame_name, OrderedFrame<float, consts::LENGTH> &frame) override;
private:
    static constexpr char file_extension[] = "h5";
    // format string like {file_name}_{file_index:06d}.h5 with file_name and file_index as arguments as constexpr
    const std::string file_format = std::string("{file_name}_{file_index:06d}.") + file_extension;
    // regex to match file names like 20230825_run_122.h5
    const std::regex file_match = std::regex(std::string("^\\w+_(\\d+)\\.") + file_extension);
};
