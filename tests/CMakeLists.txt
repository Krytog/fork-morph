include(${BASE_CMAKE_PATH}/cmake/TestDefs.cmake)

list(APPEND CMAKE_MODULE_PATH ${BASE_CMAKE_PATH}/cmake)
find_package(GTest REQUIRED)

add_gtest(hello_world
    helloworld.cpp
)

add_gtest(manifestdiff
    manifestdiff.cpp
    ${BASE_SRC_PATH}/ManifestDiff.cpp
)

add_gtest(morph
    morph.cpp
    ${BASE_SRC_PATH}/Morph.cpp
    ${BASE_SRC_PATH}/ManifestDiff.cpp
)
