project(problem1 C)

include_directories(include)

if (CMAKE_C_COMPILER_ID STREQUAL GNU)
    add_compile_options(-mavx -fopt-info-vec -fopt-info-loop)
elseif (CMAKE_C_COMPILER_ID STREQUAL Clang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL AppleClang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL Intel)
    add_compile_options(-xAVX -qopt-report-stdout)
endif ()
set (CMAKE_BUILD_TYPE Release)

add_executable(mul_test src/mul_test.c src/matvec_mul.c)
add_executable(mul_bench src/mul_bench.c src/matvec_mul.c)
