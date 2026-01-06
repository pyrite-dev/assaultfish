include(GNUInstallDirs)

macro(compile_tool name)
	file(GLOB ${name}_SRC ${name}/*.c)
	add_executable(${name} ${${name}_SRC})
	target_include_directories(${name} PRIVATE ../../external/stb)
endmacro()

compile_tool(gb_dumpfont)
