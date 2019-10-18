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

ADD_DEFINITIONS(-DQT_NO_KEYWORDS) # Qt signal/slot setting
find_package(pybind11 REQUIRED)

# include Eigen if you need
set( EIGEN_DIR C:/eigen-3.3.7)
include_directories(${EIGEN_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} MODULE
  main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE pybind11::module)
set_target_properties(${PROJECT_NAME} PROPERTIES
  # PREFIX "${PYTHON_MODULE_PREFIX}"
  # SUFFIX "${PYTHON_MODULE_EXTENSION}"
  SUFFIX ".pyd"
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
add_definitions(-DMY_MODULE_NAME=${PROJECT_NAME}) # module name must be same lib name