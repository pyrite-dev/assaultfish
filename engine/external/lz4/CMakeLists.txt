cmake_minimum_required(VERSION 3.11)
project(lz4)

file(GLOB LZ4_SRC *.c)

add_library(lz4 ${LZ4_SRC})
target_include_directories(lz4 PUBLIC .)

set_property(TARGET lz4 PROPERTY POSITION_INDEPENDENT_CODE ON)
