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
CMAKE_SOURCE_DIR = /mnt/g/test/shmipc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/g/test/shmipc/build

# Include any dependencies generated for this target.
include CMakeFiles/shmtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/shmtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shmtest.dir/flags.make

CMakeFiles/shmtest.dir/main.cpp.o: CMakeFiles/shmtest.dir/flags.make
CMakeFiles/shmtest.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/g/test/shmipc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/shmtest.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/shmtest.dir/main.cpp.o -c /mnt/g/test/shmipc/main.cpp

CMakeFiles/shmtest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shmtest.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/g/test/shmipc/main.cpp > CMakeFiles/shmtest.dir/main.cpp.i

CMakeFiles/shmtest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shmtest.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/g/test/shmipc/main.cpp -o CMakeFiles/shmtest.dir/main.cpp.s

# Object files for target shmtest
shmtest_OBJECTS = \
"CMakeFiles/shmtest.dir/main.cpp.o"

# External object files for target shmtest
shmtest_EXTERNAL_OBJECTS =

shmtest: CMakeFiles/shmtest.dir/main.cpp.o
shmtest: CMakeFiles/shmtest.dir/build.make
shmtest: CMakeFiles/shmtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/g/test/shmipc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable shmtest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shmtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shmtest.dir/build: shmtest

.PHONY : CMakeFiles/shmtest.dir/build

CMakeFiles/shmtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shmtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shmtest.dir/clean

CMakeFiles/shmtest.dir/depend:
	cd /mnt/g/test/shmipc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/g/test/shmipc /mnt/g/test/shmipc /mnt/g/test/shmipc/build /mnt/g/test/shmipc/build /mnt/g/test/shmipc/build/CMakeFiles/shmtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/shmtest.dir/depend
