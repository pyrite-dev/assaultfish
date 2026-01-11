cmake_minimum_required(VERSION 3.11)
project(assaultfish)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(external/stb)

set(MW_INSTALL_HEADERS OFF)

add_subdirectory(external/lz4)
add_subdirectory(external/ode)
add_subdirectory(submodule/milsko) # Since Milsko provides CMake, we just use it
add_subdirectory(external/xemil)

add_subdirectory(engine)

add_subdirectory(client)
