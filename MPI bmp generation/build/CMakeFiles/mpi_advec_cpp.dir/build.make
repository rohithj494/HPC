# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build

# Include any dependencies generated for this target.
include CMakeFiles/mpi_advec_cpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mpi_advec_cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mpi_advec_cpp.dir/flags.make

CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.o: CMakeFiles/mpi_advec_cpp.dir/flags.make
CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.o: ../cpp/mpi_advec_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.o -c /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/cpp/mpi_advec_main.cpp

CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/cpp/mpi_advec_main.cpp > CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.i

CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/cpp/mpi_advec_main.cpp -o CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.s

CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.o: CMakeFiles/mpi_advec_cpp.dir/flags.make
CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.o: ../bitmap/bitmap.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.o   -c /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/bitmap/bitmap.c

CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/bitmap/bitmap.c > CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.i

CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/bitmap/bitmap.c -o CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.s

# Object files for target mpi_advec_cpp
mpi_advec_cpp_OBJECTS = \
"CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.o" \
"CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.o"

# External object files for target mpi_advec_cpp
mpi_advec_cpp_EXTERNAL_OBJECTS =

mpi_advec_cpp: CMakeFiles/mpi_advec_cpp.dir/cpp/mpi_advec_main.cpp.o
mpi_advec_cpp: CMakeFiles/mpi_advec_cpp.dir/bitmap/bitmap.c.o
mpi_advec_cpp: CMakeFiles/mpi_advec_cpp.dir/build.make
mpi_advec_cpp: /usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi.so
mpi_advec_cpp: CMakeFiles/mpi_advec_cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable mpi_advec_cpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mpi_advec_cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mpi_advec_cpp.dir/build: mpi_advec_cpp

.PHONY : CMakeFiles/mpi_advec_cpp.dir/build

CMakeFiles/mpi_advec_cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mpi_advec_cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mpi_advec_cpp.dir/clean

CMakeFiles/mpi_advec_cpp.dir/depend:
	cd /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494 /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494 /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build /home/rohithj/Winter2021/HPC/project8/homework-8-rohithj494/build/CMakeFiles/mpi_advec_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mpi_advec_cpp.dir/depend

