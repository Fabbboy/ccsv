cmake_minimum_required(VERSION 3.10)
project(ccsv)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(tests)

file(GLOB_RECURSE SOURCES "./src/*.c")

include_directories(include)

add_library(ccsv STATIC ${SOURCES})
target_include_directories(ccsv PUBLIC include)

