cmake_minimum_required(VERSION 3.11)
project(assaultfish)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(external/stb)

add_subdirectory(external/milsko)

add_subdirectory(engine)

add_subdirectory(client)

install(
	FILES resource/logo.png
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/assaultfish
)
