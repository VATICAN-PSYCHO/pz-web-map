cmake_minimum_required(VERSION 3.30)
project(PZWebMap VERSION 0.0.1 LANGUAGES C CXX)

file(GLOB_RECURSE INCLUDE_SOURCES "src/*.cpp")

add_executable(PZWebMap src/main.cpp ${INCLUDE_SOURCES})

set(ENV{CL_TARGET_OPENCL_VERSION} "300")

include_directories(include)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})

target_link_libraries(PZWebMap PRIVATE yaml-cpp ${OpenCV_LIBS})

set_property(TARGET PZWebMap PROPERTY CXX_STANDARD 17)