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
add_executable(af_client ${CLIENT_SRC})

target_include_directories(af_server PRIVATE include)
target_include_directories(af_client PRIVATE include)

target_compile_definitions(af_server PRIVATE AF_SERVER)
target_compile_definitions(af_client PRIVATE AF_CLIENT)

target_link_libraries(af_server PRIVATE fishsoup)
target_link_libraries(af_client PRIVATE fishsoup Mw glad)

install(
	TARGETS af_server af_client
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
