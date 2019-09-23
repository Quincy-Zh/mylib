CMAKE_MINIMUM_REQUIRED(VERSION 3.0)


#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

SET(TARGET test)

#set(PROJECT_BINARY_DIR bin)

PROJECT(${TARGET})

add_subdirectory(linkedlist)
add_subdirectory(plc)
add_subdirectory(serialport)
add_subdirectory(test)
