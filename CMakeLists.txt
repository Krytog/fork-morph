cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME fork-morph)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # Just for clangd to work correctly

set(BASE_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(BASE_SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BASE_3RDPARTY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")

add_subdirectory(tests)
add_subdirectory(src)
