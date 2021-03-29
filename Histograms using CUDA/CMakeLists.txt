project(project4 C)


set(CMAKE_BUILD_TYPE Release)
include_directories(include)
link_libraries(m)

include(CheckLanguage)
check_language(CUDA)

if (CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    add_Compile_options(--resource-usage)
    add_executable(hist_coal ./src/hist_coal.cu)
    add_executable(hist_private ./src/hist_private)
endif ()
