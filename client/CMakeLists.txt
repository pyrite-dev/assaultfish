project(af_client)

file(GLOB af_client_SRC *.c)

add_executable(af_client ${af_client_SRC})
target_link_libraries(af_client PRIVATE GearSrc Mw)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	target_link_libraries(af_client PRIVATE m)
endif()

install_target(af_client)
