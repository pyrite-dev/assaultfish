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

add_subdirectory(net)

file(GLOB GearBox_SRC src/*.c lz4/*.c)

add_library(GearBox SHARED ${GearBox_SRC})
target_include_directories(GearBox PUBLIC include)
target_include_directories(GearBox PRIVATE lz4)
target_compile_definitions(GearBox PRIVATE _GEARBOX)
target_link_libraries(GearBox PRIVATE gbnet)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	target_link_libraries(GearBox PRIVATE m)
endif()

install_target(GearBox)
install(
	DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

add_subdirectory(tools)

install(
	DIRECTORY resource/
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/GearBox
)
