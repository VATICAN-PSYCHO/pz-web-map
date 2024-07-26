cmake_minimum_required(VERSION 3.30)
project(PZWebMap VERSION 0.0.1 LANGUAGES C CXX)

file(GLOB_RECURSE INCLUDE_SOURCES "src/include/*.cpp" "src/include/*.c")

add_executable(PZWebMap src/main.cpp ${INCLUDE_SOURCES})

add_subdirectory(third-party/yaml-cpp)

target_link_libraries(PZWebMap PRIVATE yaml-cpp)

set_property(TARGET PZWebMap PROPERTY CXX_STANDARD 17)