CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

include_directories(
    ${PROJECT_SOURCE_DIR}/linkedlist 
    ${PROJECT_SOURCE_DIR}/plc 
    ${PROJECT_SOURCE_DIR}/serialport)

ADD_EXECUTABLE(${TARGET} main.c)
TARGET_LINK_LIBRARIES(${TARGET} serialport plc)
