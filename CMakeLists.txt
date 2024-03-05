cmake_minimum_required(VERSION 3.14)
project(TangoMoenchZMQ LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(DEBUG_SRC_FILES "")
find_package(ZeroMQ REQUIRED)
find_package(Boost REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS CXX)
# maybe use h5cpp instead of native HDF5 lib...
# find_package(h5cpp REQUIRED)
if (CUDA_ENABLED)
    enable_language(CUDA)
    find_package(CUDAToolkit)
endif()
if (SINGLE_FRAMES_DEBUG)
    message(STATUS "Single Frames Debug enabled")
endif()
if (OPENMP_ENABLED)
    find_package(OpenMP)
    if (OpenMP_CXX_FOUND)
        message(STATUS "OpenMP CXX found")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
endif()

find_package(Tango)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory ("tracy") # target: TracyClient or alias Tracy :: TracyClient
    option( TRACY_ENABLE "" ON)
    option( TRACY_ON_DEMAND "" ON)
    # set debug src files
    set(DEBUG_SRC_FILES "tracy/public/TracyClient.cpp")
    message(STATUS "Enabling Tracy Profiler")
endif()
include_directories(${HDF5_INCLUDE_DIRS})

add_subdirectory(src/backend)
add_subdirectory(src/tangods)

if (CUDA_ENABLED)
    add_library(cuda_processing STATIC
    CUDA_SRC_FILES
    )
endif()
#Add your executable
add_executable(MoenchZMQ ${MOENCH_SRC_FILES} ${TANGO_SRC_FILES} ${DEBUG_SRC_FILES})
#add_executable(napi_test_cpp-hdf5 ${NEXUS_SRC_FILES})
add_executable(ZmqReceiver ${MOENCH_MAIN} ${MOENCH_SRC_FILES} ${DEBUG_SRC_FILES})


target_link_libraries(MoenchZMQ zmq fmt ${Boost_LIBRARIES} ${HDF5_LIBRARIES} Tango::Tango)
target_link_libraries(MoenchZMQ debug Tracy::TracyClient)

target_link_libraries(ZmqReceiver zmq fmt ${Boost_LIBRARIES} ${HDF5_LIBRARIES})
target_link_libraries(ZmqReceiver debug Tracy::TracyClient)

if (OPENMP_ENABLED AND OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
