# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/vinstarry/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/183.4886.39/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/vinstarry/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/183.4886.39/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vinstarry/projects/osdesign/design1_1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vinstarry/projects/osdesign/design1_1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/design1_1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/design1_1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/design1_1.dir/flags.make

CMakeFiles/design1_1.dir/main.c.o: CMakeFiles/design1_1.dir/flags.make
CMakeFiles/design1_1.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vinstarry/projects/osdesign/design1_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/design1_1.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/design1_1.dir/main.c.o   -c /home/vinstarry/projects/osdesign/design1_1/main.c

CMakeFiles/design1_1.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/design1_1.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vinstarry/projects/osdesign/design1_1/main.c > CMakeFiles/design1_1.dir/main.c.i

CMakeFiles/design1_1.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/design1_1.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vinstarry/projects/osdesign/design1_1/main.c -o CMakeFiles/design1_1.dir/main.c.s

# Object files for target design1_1
design1_1_OBJECTS = \
"CMakeFiles/design1_1.dir/main.c.o"

# External object files for target design1_1
design1_1_EXTERNAL_OBJECTS =

design1_1: CMakeFiles/design1_1.dir/main.c.o
design1_1: CMakeFiles/design1_1.dir/build.make
design1_1: CMakeFiles/design1_1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vinstarry/projects/osdesign/design1_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable design1_1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/design1_1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/design1_1.dir/build: design1_1

.PHONY : CMakeFiles/design1_1.dir/build

CMakeFiles/design1_1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/design1_1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/design1_1.dir/clean

CMakeFiles/design1_1.dir/depend:
	cd /home/vinstarry/projects/osdesign/design1_1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vinstarry/projects/osdesign/design1_1 /home/vinstarry/projects/osdesign/design1_1 /home/vinstarry/projects/osdesign/design1_1/cmake-build-debug /home/vinstarry/projects/osdesign/design1_1/cmake-build-debug /home/vinstarry/projects/osdesign/design1_1/cmake-build-debug/CMakeFiles/design1_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/design1_1.dir/depend
