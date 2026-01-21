cmake_minimum_required(VERSION 3.11)
project(ode)

file(GLOB ODE_SRC
	../../submodule/ode/ode/src/*.c
	../../submodule/ode/ode/src/*.cpp
	../../submodule/ode/ode/src/joints/*.cpp
	../../submodule/ode/ou/src/ou/*.cpp
	../../submodule/ode/GIMPACT/src/*.cpp
	../../submodule/ode/libccd/src/*.c
)
add_library(ode ${ODE_SRC})
target_include_directories(ode PUBLIC
	../../submodule/ode/include
)
target_include_directories(ode PRIVATE
	../../submodule/ode/ode/src
	../../submodule/ode/ode/src/joints
	../../submodule/ode/OPCODE
	../../submodule/ode/ou/include
	../../submodule/ode/GIMPACT/include
	../../submodule/ode/libccd/src/custom
	../../submodule/ode/libccd/src
	${CMAKE_CURRENT_BINARY_DIR}
)

list(REMOVE_ITEM ODE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../submodule/ode/ode/src/collision_trimesh_trimesh_old.cpp)
list(REMOVE_ITEM ODE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../submodule/ode/ode/src/collision_trimesh_opcode.cpp)
list(REMOVE_ITEM ODE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../submodule/ode/ode/src/collision_trimesh_disabled.cpp)

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

set_property(TARGET ode PROPERTY POSITION_INDEPENDENT_CODE ON)

set(ODE_PRECISION "dDOUBLE")
set(CCD_PRECISION "CCD_DOUBLE")
set(ODE_VERSION "Custom-ODE")

configure_file(../../submodule/ode/include/ode/version.h.in ode/version.h)
configure_file(../../submodule/ode/include/ode/precision.h.in ode/precision.h)
configure_file(../../submodule/ode/libccd/src/ccd/precision.h.in ccd/precision.h)
configure_file(config.h.in config.h)
