project(TangoMoenchZMQ LANGUAGES C CXX)
cmake_minimum_required(VERSION 3.14)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(slsDetectorPackage)
#find_package(ZeroMQ REQUIRED)
find_package(Boost REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS CXX)
find_package(h5cpp REQUIRED)

find_package(Tango REQUIRED)
option( TRACY_ENABLE "" ON)
option( TRACY_ON_DEMAND "" ON)
add_subdirectory ("tracy") # target: TracyClient or alias Tracy :: TracyClient
include_directories(${HDF5_INCLUDE_DIRS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -std=c++20 ") # -pg -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ")
add_subdirectory(src/backend)
add_subdirectory(src/tangods)

#Add your executable
add_executable(MoenchZMQ ${TANGO_SRC_FILES})
#add_executable(napi_test_cpp-hdf5 ${NEXUS_SRC_FILES})
add_executable(ZmqReceiver ${MOENCH_SRC_FILES} "tracy/public/TracyClient.cpp")

# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

#Link towards slsDetectorShared
target_link_libraries(MoenchZMQ Tango::Tango)
#target_link_libraries(napi_test_cpp-hdf5 NeXusCPP)
target_link_libraries(ZmqReceiver slsDetectorShared zmq ${Boost_LIBRARIES} Tracy::TracyClient ${HDF5_LIBRARIES} fmt h5cpp::h5cpp)

