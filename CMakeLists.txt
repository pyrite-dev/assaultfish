cmake_minimum_required(VERSION 3.11)
project(assaultfish)

include(GNUInstallDirs)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_subdirectory(external/libfishsoup)
add_subdirectory(external/milsko)
add_subdirectory(external/glad)

file(GLOB SERVER_SRC server/*.c)
file(GLOB CLIENT_SRC client/*.c)

add_executable(af_server ${SERVER_SRC})
add_executable(af_client ${CLIENT_SRC} external/stb/stb_image.c)
add_executable(af_dumpfont tools/af_dumpfont.c external/stb/stb_image_write.c)

target_include_directories(af_server PRIVATE include external/stb ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(af_client PRIVATE include external/stb ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(af_dumpfont PRIVATE include external/stb ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(af_server PRIVATE AF_SERVER)
target_compile_definitions(af_client PRIVATE AF_CLIENT)

target_link_libraries(af_server PRIVATE fishsoup)
target_link_libraries(af_client PRIVATE fishsoup Mw glad)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(af_client PRIVATE glu32)
else()
	find_package(PkgConfig)
	pkg_check_modules(GLU REQUIRED glu)
	target_compile_options(af_client PRIVATE ${GLU_CFLAGS_OTHER})
	target_include_directories(af_client PRIVATE ${GLU_INCLUDE_DIRS})
	target_link_options(af_client PRIVATE ${GLU_LDFLAGS_OTHER})
	target_link_directories(af_client PRIVATE ${GLU_LIBRARY_DIRS})
	target_link_libraries(af_client PRIVATE ${GLU_LIBRARIES} m)
endif()

configure_file(include/af_config.h.in af_config.h)

install(
	TARGETS af_server af_client af_dumpfont
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
	FILES resource/logo.png resource/font.png
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/assaultfish
)
