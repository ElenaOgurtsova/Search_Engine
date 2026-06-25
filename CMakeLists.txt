cmake_minimum_required(VERSION 3.29)
project(Search_Engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_DIR})
set(gtest_disable_pthreadson)

enable_testing()
add_executable(Search_Engine
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
        src/main.cpp
)

add_executable(Search_Engine_test
        src/ConverterJSON.cpp
        test/tests.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
)

target_include_directories(Search_Engine_test PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/nlohmann_json
)

target_include_directories(Search_Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/nlohmann_json
)
target_link_libraries(Search_Engine_test PRIVATE
        nlohmann_json::nlohmann_json
        gtest_main
        gtest
)

target_link_libraries(Search_Engine PRIVATE
        nlohmann_json::nlohmann_json
)
include(GoogleTest)
gtest_discover_tests(Search_Engine_test)