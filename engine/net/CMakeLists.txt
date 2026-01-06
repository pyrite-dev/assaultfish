cmake_minimum_required(VERSION 3.11)
project(libfishsoup)

add_library(fishsoup SHARED src/client.c src/server.c src/socket.c src/stb_ds.c)

target_include_directories(fishsoup PUBLIC include)

find_package(PkgConfig)
pkg_check_modules(ZLIB REQUIRED zlib)
target_compile_options(fishsoup PRIVATE ${ZLIB_CFLAGS_OTHER})
target_include_directories(fishsoup PRIVATE ${ZLIB_INCLUDE_DIRS})
target_link_options(fishsoup PRIVATE ${ZLIB_LDFLAGS_OTHER})
target_link_directories(fishsoup PRIVATE ${ZLIB_LIBRARY_DIRS})
target_link_libraries(fishsoup PRIVATE ${ZLIB_LIBRARIES})

include(GNUInstallDirs)
install(
	TARGETS fishsoup
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
