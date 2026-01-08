cmake_minimum_required(VERSION 3.11)
project(gearbox)

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

file(GLOB GearBox_SRC src/*.c)

add_library(GearBox SHARED ${GearBox_SRC})
target_include_directories(GearBox PUBLIC include)
target_compile_definitions(GearBox PRIVATE _GEARBOX)

install_target(GearBox)

add_subdirectory(tools)

install(
	FILES resource/shadow.vs resource/shadow.fs
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/GearBox
)
