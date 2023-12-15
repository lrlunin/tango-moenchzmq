find_package(slsDetectorPackage)
find_package(ZeroMQ REQUIRED)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
include_directories(../inc)


#Add your executable
add_executable(example main_sls.cpp archiver.cpp)
add_executable(zmq_receiver zmq_receiver.cpp)
add_executable(napi_test_cpp-hdf5 example_nexus.cpp)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

target_compile_features(example PRIVATE cxx_std_20)

#Link towards slsDetectorShared
target_link_libraries(example slsDetectorShared)
target_link_libraries(zmq_receiver zmq)
target_link_libraries(napi_test_cpp-hdf5 NeXusCPP)

