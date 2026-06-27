cmake_minimum_required(VERSION 3.29)
project(Search_Engine_test VERSION 1.0)

# get GTest from github
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_DIR})
set(gtest_disable_pthreadson)

enable_testing()

add_executable(Search_Engine_test
        ${CMAKE_SOURCE_DIR}/src/ConverterJSON.cpp
        tests.cpp
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
)

target_include_directories(Search_Engine_test PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/nlohmann_json
)

target_link_libraries(Search_Engine_test PRIVATE
        nlohmann_json::nlohmann_json
        gtest_main
        gtest
)


include(GoogleTest)
gtest_discover_tests(Search_Engine_test)