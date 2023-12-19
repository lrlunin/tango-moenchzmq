project(TangoMoenchZMQ LANGUAGES CXX)
cmake_minimum_required(VERSION 3.14)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(slsDetectorPackage)
#find_package(ZeroMQ REQUIRED)
find_package(Boost REQUIRED)
find_package(Tango REQUIRED)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++20 -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ")
add_subdirectory(src)

#Add your executable
add_executable(TestDeviceLunin ${TANGO_SRC_FILES})
add_executable(napi_test_cpp-hdf5 ${NEXUS_SRC_FILES})
add_executable(ZmqReceiver ${MOENCH_SRC_FILES})

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

#Link towards slsDetectorShared
target_link_libraries(TestDeviceLunin Tango::Tango)
target_link_libraries(napi_test_cpp-hdf5 NeXusCPP)
target_link_libraries(ZmqReceiver slsDetectorShared zmq ${Boost_LIBRARIES})

