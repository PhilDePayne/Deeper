cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Deeper VERSION 0.1)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

# add thirdparties
include(thirdparty/thirdparty.cmake)

# subdirectories
add_subdirectory(lib)
add_subdirectory(src)
