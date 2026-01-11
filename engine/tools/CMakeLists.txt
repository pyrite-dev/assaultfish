project(gbtools)

include(GNUInstallDirs)

macro(compile_tool name)
	file(GLOB ${name}_SRC ${name}/*.c)
	add_executable(${name} ${${name}_SRC})
	target_link_libraries(${name} PRIVATE GearSrc)
	install_target(${name})
endmacro()

file(GLOB lz4_SRC ../lz4/*.c)

compile_tool(gs_dumpfont)
compile_tool(gs_makepak)

target_link_libraries(gs_makepak PRIVATE lz4)
