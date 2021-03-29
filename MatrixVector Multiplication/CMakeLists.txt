cmake_minimum_required(VERSION 3.13)
project(mpcs-hpc-homework-1 C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

if (CMAKE_C_COMPILER_ID STREQUAL GNU)
    add_compile_options(-mavx -fopt-info-vec -fopt-info-loop)
elseif (CMAKE_C_COMPILER_ID STREQUAL Clang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL AppleClang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL Intel)
    add_compile_options(-xAVX -qopt-report-stdout)
endif ()

add_subdirectory(problem1)
add_subdirectory(problem2)

9