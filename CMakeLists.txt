cmake_minimum_required(VERSION 3.12...3.13 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
project(ProcessAnalyzer)

include(FetchContent)

# Warning flags
if(MSVC)
    # warning level 3 and all warnings as errors
    add_compile_options(/W3 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wpedantic -Werror)
endif()

# Catch2
# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.x
)

# Adds Catch2 testing library
FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_library(catch2 INTERFACE src/process_parser.cpp)
    target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
endif()

# Build
list(APPEND SOURCE_FILES src/process_parser.cpp src/process.cpp src/process_analyzer.cpp)

list(APPEND TEST_FILES tests/test_main.cpp tests/process_parser_tests.cpp)

# ProcessLyzer target build
add_executable(ProcessLyzer app/processlyzer_main.cpp ${SOURCE_FILES})
target_include_directories(ProcessLyzer PRIVATE include)
# To get certain library includes to work (such as the experimental filesystem lib)
target_link_libraries(ProcessLyzer stdc++fs)