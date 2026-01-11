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

target_include_directories(gs_makepak PRIVATE ../lz4)
target_sources(gs_makepak PRIVATE ${lz4_SRC})
