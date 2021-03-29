project(project7 C)

include_directories(include)
link_libraries(m)


set(CMAKE_BUILD_TYPE Release)

find_package(MPI REQUIRED)
add_executable(julia_static src/julia_static.c src/bitmap.c)
add_executable(julia_dynamic src/julia_dynamic.c src/bitmap.c)
target_link_libraries(julia_static PUBLIC MPI::MPI_C)
target_link_libraries(julia_dynamic PUBLIC MPI::MPI_C)

add_executable(julia_serial src/julia_serial.c src/bitmap.c)

