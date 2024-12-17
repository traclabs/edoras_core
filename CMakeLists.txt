cmake_minimum_required(VERSION 3.8)
project(edoras_core)

set(include_deps
 rosidl_typesupport_introspection_c
 rosidl_runtime_c
 rcutils
 rosidl_typesupport_interface
 rmw  
)


set(lib_deps
 rosidl_typesupport_introspection_c
 rosidl_runtime_c
 rcutils
 rmw
 rmw_implementation
 fastrtps
)

set(ROS2_DIR /opt/ros/jazzy)
foreach(dep ${include_deps})
  list(APPEND DEP_INCLUDE_DIRS "${ROS2_DIR}/include/${dep}")
endforeach()

foreach(dep ${lib_deps})
  find_library(${dep}_LIB ${dep}  HINTS ${ROS2_DIR}/lib ) 
  list(APPEND DEP_LIBRARIES "${${dep}_LIB}")
endforeach()

message(STATUS "DEP INCLUDE DIRS: ${DEP_INCLUDE_DIRS}")

#include_directories(edoras
#  PUBLIC
#  include
#  ${DEP_INCLUDE_DIRS}
#)


add_library(edoras
  src/interface.cpp
#  src/conversion_private.cpp
#  src/debug_helpers.cpp
)
target_include_directories(edoras
  PUBLIC
  include
  ${DEP_INCLUDE_DIRS}
)

target_link_libraries(edoras
  PUBLIC
  ${DEP_LIBRARIES}
)
