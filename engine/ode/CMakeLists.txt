cmake_minimum_required(VERSION 3.11)
project(ode)

file(GLOB ODE_SRC
	../../external/ode/ode/src/*.c
	../../external/ode/ode/src/*.cpp
	../../external/ode/ode/src/joints/*.cpp
	../../external/ode/ou/src/ou/*.cpp
	../../external/ode/GIMPACT/src/*.cpp
	../../external/ode/libccd/src/*.c
)
add_library(ode ${ODE_SRC})
target_include_directories(ode PRIVATE
	../../external/ode/include
	../../external/ode/ode/src
	../../external/ode/ode/src/joints
	../../external/ode/OPCODE
	../../external/ode/ou/include
	../../external/ode/GIMPACT/include
	../../external/ode/libccd/src/custom
	../../external/ode/libccd/src
	${CMAKE_CURRENT_BINARY_DIR}
)

list(REMOVE_ITEM ODE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../external/ode/ode/src/collision_trimesh_trimesh_old.cpp)
list(REMOVE_ITEM ODE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../external/ode/ode/src/collision_trimesh_opcode.cpp)
list(REMOVE_ITEM ODE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../external/ode/ode/src/collision_trimesh_disabled.cpp)

target_compile_definitions(ode PRIVATE
	_OU_NAMESPACE=odeou
	_OU_FEATURE_SET=_OU_FEATURE_SET_TLS
	dLIBCCD_ENABLED
	dLIBCCD_INTERNAL
	dLIBCCD_BOX_CYL
	dLIBCCD_CYL_CYL
	dLIBCCD_CAP_CYL
	dLIBCCD_CONVEX_BOX
	dLIBCCD_CONVEX_CAP
	dLIBCCD_CONVEX_CYL
	dLIBCCD_CONVEX_SPHERE
	dLIBCCD_CONVEX_CONVEX
)

set(ODE_PRECISION "dDOUBLE")
set(CCD_PRECISION "CCD_DOUBLE")
set(ODE_VERSION "Custom-ODE")

configure_file(../../external/ode/include/ode/version.h.in ode/version.h)
configure_file(../../external/ode/include/ode/precision.h.in ode/precision.h)
configure_file(../../external/ode/libccd/src/ccd/precision.h.in ccd/precision.h)
configure_file(config.h.in config.h)
