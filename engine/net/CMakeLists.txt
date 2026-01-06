cmake_minimum_required(VERSION 3.11)
project(gbnet)

add_library(gbnet SHARED src/client.c src/server.c src/socket.c src/stb_ds.c)

target_include_directories(gbnet PUBLIC include)

find_package(PkgConfig)
pkg_check_modules(ZLIB REQUIRED zlib)
target_compile_options(gbnet PRIVATE ${ZLIB_CFLAGS_OTHER})
target_include_directories(gbnet PRIVATE ${ZLIB_INCLUDE_DIRS})
target_link_options(gbnet PRIVATE ${ZLIB_LDFLAGS_OTHER})
target_link_directories(gbnet PRIVATE ${ZLIB_LIBRARY_DIRS})
target_link_libraries(gbnet PRIVATE ${ZLIB_LIBRARIES})
