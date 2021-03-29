project(problem2 C)

include_directories(include)
link_libraries(m)

add_executable(bitmap_demo src/bitmap_demo.c src/bitmap.c)
target_include_directories(bitmap_demo PUBLIC ${CMAKE_CURRENT_LIST_DIR})


if (CMAKE_C_COMPILER_ID STREQUAL GNU)
    add_compile_options(-mavx -fopt-info-vec -fopt-info-loop)
elseif (CMAKE_C_COMPILER_ID STREQUAL Clang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL AppleClang)
    add_compile_options(-Rpass=.*loop.*)
elseif (CMAKE_C_COMPILER_ID STREQUAL Intel)
    add_compile_options(-xAVX -qopt-report-stdout)
endif ()

add_executable(particle_aos src/particle_aos.c src/bitmap.c)
add_executable(particle_soa src/particle_soa.c src/bitmap.c)


