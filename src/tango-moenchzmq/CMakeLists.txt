cmake_minimum_required(VERSION 3.14)
project(TangoMoenchZMQ LANGUAGES CXX)

find_package(Tango)
find_package(ZeroMQ REQUIRED)
find_package(Boost REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS CXX)
include_directories(${HDF5_INCLUDE_DIRS})

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory ("tracy")
    option(TRACY_ENABLE "" ON)
    option(TRACY_ON_DEMAND "" ON)
    # set debug src files
    set(DEBUG_SRC_FILES "tracy/public/TracyClient.cpp")
    message(STATUS "Enabling Tracy Profiler")
endif()

if (CUDA_ENABLED)
    enable_language(CUDA)
    find_package(CUDAToolkit)
endif()
if (SINGLE_FRAMES_DEBUG)
    message(STATUS "Single Frames Debug enabled")
    add_definitions(-DSINGLE_FRAMES_DEBUG=1)
endif()
if (OPENMP_ENABLED)
    find_package(OpenMP)
    if (OpenMP_CXX_FOUND)
        message(STATUS "OpenMP CXX found")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
endif()


add_subdirectory(backend)
add_subdirectory(tangods)
if (CUDA_ENABLED)
    add_library(cuda_processing STATIC
    CUDA_SRC_FILES
    )
endif()

add_executable(MoenchZMQ ${MOENCHZMQ_BACKEND_SRC_FILES} ${MOENCHZMQ_TANGODS_SRC_FILES} ${DEBUG_SRC_FILES})
add_executable(ComputationTest ${MOENCH_TEST} ${MOENCHZMQ_BACKEND_SRC_FILES} ${DEBUG_SRC_FILES})

target_link_libraries(MoenchZMQ zmq fmt ${Boost_LIBRARIES} ${HDF5_LIBRARIES} Tango::Tango)
target_link_libraries(MoenchZMQ debug Tracy::TracyClient)

target_link_libraries(ComputationTest zmq fmt ${Boost_LIBRARIES} ${HDF5_LIBRARIES})
target_link_libraries(ComputationTest debug Tracy::TracyClient)
