# source files
set(MOENCH_SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ComputationTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ComputationBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ZMQListener.cpp
    PARENT_SCOPE)

set(NEXUS_SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ExampleNexus.cpp
    PARENT_SCOPE)