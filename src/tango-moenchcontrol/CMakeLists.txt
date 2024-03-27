cmake_minimum_required(VERSION 3.14)
project(TangoMoenchControl LANGUAGES CXX)

find_package(Tango)
find_package(ZeroMQ REQUIRED)
find_package(Boost REQUIRED)
find_package(slsDetectorPackage 8.0 REQUIRED)


add_subdirectory(tangods)

add_executable(MoenchControl ${MOENCHCONTROL_TANGODS_SRC_FILES})

target_link_libraries(MoenchControl slsDetectorShared slsReceiverStatic Tango::Tango) 
