cmake_minimum_required(VERSION 3.29)
project(jdwp C)

set(CMAKE_C_STANDARD 99)
set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --error-exitcode=1 --track-origins=yes")

include(CTest)

add_library(jdwp SHARED)
target_include_directories(jdwp PRIVATE include)

add_subdirectory(src)
add_subdirectory(test)