cmake_minimum_required(VERSION 3.29)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_executable(Search_Engine
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        main.cpp
)

target_include_directories(Search_Engine PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/nlohmann_json
)

target_link_libraries(Search_Engine PRIVATE
        nlohmann_json::nlohmann_json
)
