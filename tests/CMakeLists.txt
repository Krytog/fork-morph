include(${BASE_CMAKE_PATH}/cmake/TestDefs.cmake)

list(APPEND CMAKE_MODULE_PATH ${BASE_CMAKE_PATH}/cmake)
find_package(GTest REQUIRED)

add_gtest(hello_world
    unit_helloworld.cpp
)
