cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)
message(${ProjectId})

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
# string(REPLACE "/W3" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# include(../Qt.cmake)
# include(../opt.cmake)
# include(../vs.cmake)

# message("---eigen setting---")
# set( EIGEN_DIR C:/eigen-3.3.7)
# include_directories(${EIGEN_DIR})
# message(${EIGEN_DIR})

# message("---pybind11 setting---")
# include_directories(C:/Users/nitta/AppData/Local/Programs/Python/Python35/include)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
# slots,signals,emitの代わりにQ_SLOTS,Q_SIGNALS,Q_EMITを使用すること

find_package(pybind11 REQUIRED)  # or `add_subdirectory(pybind11)`

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

# Add Executable
add_executable(${PROJECT_NAME}
   main.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(${PROJECT_NAME} 
   # C:/Users/nitta/AppData/Local/Programs/Python/Python35/libs/python35.lib
   PRIVATE pybind11::embed
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
