cmake_minimum_required(VERSION 3.11)
project(assaultfish)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# should this be on?
#set(MW_INSTALL_HEADERS OFF)

add_subdirectory(submodule/milsko) # Since Milsko provides CMake, we just use it

add_subdirectory(engine)

add_subdirectory(client)
