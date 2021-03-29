project(project3 C)

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

add_executable(raytracing src/raytracingArray.c src/bitmap.c)
target_link_libraries(raytracing PUBLIC OpenMP::OpenMP_C)