add_library(gmock ${BASE_CMAKE_PATH}/tests/contrib/gmock-gtest-all.cc)

target_include_directories(gmock
  SYSTEM PUBLIC ${BASE_CMAKE_PATH}/tests/contrib/gtest ${BASE_CMAKE_PATH}/tests/contrib/gmock ${BASE_CMAKE_PATH}/tests/contrib)

message(${BASE_CMAKE_PATH}/tests/contrib/gtest)

target_link_libraries(gmock pthread)
