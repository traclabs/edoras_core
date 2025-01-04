cmake_minimum_required(VERSION 3.16)
project(edoras_core LANGUAGES CXX)

find_package(ament_cmake REQUIRED)

set(dependencies
 rosidl_typesupport_introspection_c
 rosidl_runtime_c
 rcutils
 rmw
 rmw_implementation
 rosidl_dynamic_typesupport
 rcpputils
  rosidl_typesupport_fastrtps_c
  rosidl_dynamic_typesupport_fastrtps
  fastcdr
  rmw_fastrtps_cpp
  rmw_fastrtps_shared_cpp
  rosidl_typesupport_fastrtps_cpp
  rmw_dds_common
  rosidl_typesupport_introspection_cpp
  rosidl_typesupport_cpp
  ament_index_cpp
  fastrtps
  rosidl_typesupport_interface ## ADDED
  # rcl
  # rmw_fastrtps_shared_cpp
)

foreach(dep ${dependencies})
  find_package(${dep} REQUIRED)
endforeach()

# *******************************************************
# Code that actually does the conversion
add_library(${PROJECT_NAME}_private SHARED
            src/conversion_private.cpp
           src/debug_helpers.cpp
)
ament_target_dependencies(${PROJECT_NAME}_private
  ${dependencies}
)

target_include_directories(${PROJECT_NAME}_private
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  INTERFACE
  ${rosidl_typesupport_introspection_c_INCLUDE_DIRS}
  ${rcutils_INCLUDE_DIRS}
)


# *******************************************************
# Interface that is used by apps like cFS and bridges
add_library(${PROJECT_NAME} SHARED
  src/interface.cpp
)
target_link_libraries(${PROJECT_NAME} 
 PUBLIC
 ${PROJECT_NAME}_private
)

# Test
add_executable(test_reading tests/test_reading.cpp)
target_link_libraries(test_reading ${PROJECT_NAME})

add_executable(test_setting_joint_state tests/test_setting_joint_state.cpp)
target_link_libraries(test_setting_joint_state ${PROJECT_NAME})

add_executable(test_setting_header tests/test_setting_header.cpp)
target_link_libraries(test_setting_header ${PROJECT_NAME})

# ********************************************************
# Install
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_private
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib        	
)
set_target_properties(${PROJECT_NAME} PROPERTIES
 INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
 BUILD_WITH_INSTALL_RPATH ON
) 

install(DIRECTORY include/${PROJECT_NAME} 
        DESTINATION include)

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION lib/cmake/${PROJECT_NAME}
)

# Create Config.Cmake
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/edoras_core
    CACHE PATH "Location of header files" )
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/edoras_core.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
  PATH_VARS INCLUDE_INSTALL_DIR
)  

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
#  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
  )

# https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20Export%20Configuration.html
# At this point, we have generated a relocatable CMake Configuration for our project 
# that can be used after the project has been installed or packaged. If we want our 
# project to also be used from a build directory we only have to add the following to 
# the bottom of the top level CMakeLists.txt:
#export(EXPORT MathFunctionsTargets
#  FILE "${CMAKE_CURRENT_BINARY_DIR}/MathFunctionsTargets.cmake"
#)
