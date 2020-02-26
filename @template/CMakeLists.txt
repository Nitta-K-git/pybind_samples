cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

find_package(pybind11 REQUIRED)

set( EIGEN3_INCLUDE_DIR C:/eigen-3.3.7)
find_package(Eigen3)

if(EIGEN3_FOUND AND pybind11_FOUND)
else()
  message(FATAL_ERROR "ERROR: this program will not be compiled.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

pybind11_add_module(${PROJECT_NAME} SHARED
  main.cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
add_definitions(-DMY_MODULE_NAME=${PROJECT_NAME}) # module name must be same lib name
