project(project6 C CXX)


set(CMAKE_BUILD_TYPE Release)
include_directories(include)
link_libraries(m)

add_executable(cpu ./src/spmv_cpu.c)

include(CheckLanguage)
check_language(CUDA)

if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    add_Compile_options(--resource-usage)
    add_executable(gpu ./src/spmv_gpu.cu)
endif ()


