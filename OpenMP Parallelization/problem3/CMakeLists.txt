project(problem3 C)

include_directories(include)
link_libraries(m)


set(CMAKE_BUILD_TYPE Release)

find_package(OpenMP REQUIRED)
if (CMAKE_C_COMPILER_ID STREQUAL GNU)
    add_compile_options(-mavx -fopt-info-vec -fopt-info-loop -fopenmp)
elseif (CMAKE_C_COMPILER_ID STREQUAL Clang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL AppleClang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL Intel)
    add_compile_options(-xAVX -qopt-report-stdout)
endif ()

add_executable(parallel src/mandle.c src/bitmap.c)
target_link_libraries(parallel PUBLIC OpenMP::OpenMP_C)
