cmake_minimum_required(VERSION 3.11)
project(assaultfish)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

macro(install_target name)
	install(
		TARGETS ${name}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	)
endmacro()

add_subdirectory(external/glad)

add_subdirectory(net)

add_subdirectory(tools)
