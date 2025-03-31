add_executable(${PROJECT_NAME} main.cpp
    ManifestReader.cpp
    ManifestDiff.cpp
    Morph.cpp
    FileReader.cpp
    DataProviders.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${BASE_SRC_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${BASE_3RDPARTY_PATH})
