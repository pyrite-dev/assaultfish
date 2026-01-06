cmake_minimum_required(VERSION 3.11)
project(assaultfish)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(external/milsko)

add_subdirectory(engine)

install(
	FILES resource/logo.png resource/font.png resource/shadow.vs resource/shadow.fs
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/assaultfish
)
