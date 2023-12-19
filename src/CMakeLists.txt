# source files
set(TANGO_SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ClassFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDeviceLunin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDeviceLuninClass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDeviceLuninStateMachine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    PARENT_SCOPE)

set(NEXUS_SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ExampleNexus.cpp
    PARENT_SCOPE)

set(MOENCH_SRC_FILES ${SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ZmqReceiverTest.cpp
    PARENT_SCOPE)