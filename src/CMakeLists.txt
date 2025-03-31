add_executable(${PROJECT_NAME} main.cpp
    ManifestReader.cpp
    ManifestDiff.cpp
    Morph.cpp
    FileReader.cpp
    DataProviders.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${BASE_SRC_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${BASE_3RDPARTY_PATH})


# For aws sdk
set(AWS_SDK_CPP_ENABLE_S3 ON)
set(AWS_SDK_CPP_ENABLE_TESTS OFF)
set(AWS_SDK_CPP_ENABLE_TOOLS OFF)
set(BUILD_ONLY "s3")
set(AWS_SDK_CPP_BUILD_ONLY "s3")

set(OPENSSL_ROOT_DIR "/usr")
set(OPENSSL_INCLUDE_DIR "/usr/include")
set(OPENSSL_CRYPTO_LIBRARY "/usr/lib/libcrypto.so")
set(OPENSSL_SSL_LIBRARY "/usr/lib/libssl.so")

find_package(OpenSSL REQUIRED)

configure_file(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-common/include/aws/common/config.h.in ${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-common/include/aws/common/config.h @ONLY)
add_subdirectory(${BASE_CMAKE_PATH}/3rdparty/aws ${CMAKE_BINARY_DIR}/aws)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/include)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-common/include)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-io/include)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-mqtt/include)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-s3/include)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-sdkutils/include)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-auth/include)
include_directories(${BASE_3RDPARTY_PATH}/aws/crt/aws-crt-cpp/crt/aws-c-cal/include)
target_link_libraries(${PROJECT_NAME} PRIVATE AWS::aws-cpp-sdk-core AWS::aws-cpp-sdk-s3)
target_include_directories(${PROJECT_NAME} PRIVATE ${BASE_3RDPARTY_PATH}/aws/src/aws-cpp-sdk-core/include)
