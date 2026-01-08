project(af_client)

file(GLOB af_client_SRC *.c)

add_executable(af_client ${af_client_SRC})
target_link_libraries(af_client PRIVATE GearBox)

install_target(af_client)
