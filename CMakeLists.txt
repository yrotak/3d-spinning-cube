cmake_minimum_required(VERSION 3.5)

project(3dmaths)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

find_package(SDL2 REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)