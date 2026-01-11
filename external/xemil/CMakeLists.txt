cmake_minimum_required(VERSION 3.11)
project(xemil)

file(GLOB XEMIL_SRC ../../submodule/xemil/src/*.c)

add_library(xemil ${XEMIL_SRC})
target_include_directories(xemil PUBLIC ../../submodule/xemil/include)

set_property(TARGET xemil PROPERTY POSITION_INDEPENDENT_CODE ON)
