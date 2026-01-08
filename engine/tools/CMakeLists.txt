project(gbtools)

include(GNUInstallDirs)

macro(compile_tool name)
	file(GLOB ${name}_SRC ${name}/*.c)
	add_executable(${name} ${${name}_SRC})
endmacro()

compile_tool(gb_dumpfont)
