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
CMAKE_SOURCE_DIR = /home/vinstarry/projects/osdesign/mulproc_display

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/mulproc_display.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mulproc_display.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mulproc_display.dir/flags.make

CMakeFiles/mulproc_display.dir/main.c.o: CMakeFiles/mulproc_display.dir/flags.make
CMakeFiles/mulproc_display.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mulproc_display.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mulproc_display.dir/main.c.o   -c /home/vinstarry/projects/osdesign/mulproc_display/main.c

CMakeFiles/mulproc_display.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mulproc_display.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vinstarry/projects/osdesign/mulproc_display/main.c > CMakeFiles/mulproc_display.dir/main.c.i

CMakeFiles/mulproc_display.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mulproc_display.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vinstarry/projects/osdesign/mulproc_display/main.c -o CMakeFiles/mulproc_display.dir/main.c.s

CMakeFiles/mulproc_display.dir/window_proc.c.o: CMakeFiles/mulproc_display.dir/flags.make
CMakeFiles/mulproc_display.dir/window_proc.c.o: ../window_proc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/mulproc_display.dir/window_proc.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mulproc_display.dir/window_proc.c.o   -c /home/vinstarry/projects/osdesign/mulproc_display/window_proc.c

CMakeFiles/mulproc_display.dir/window_proc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mulproc_display.dir/window_proc.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vinstarry/projects/osdesign/mulproc_display/window_proc.c > CMakeFiles/mulproc_display.dir/window_proc.c.i

CMakeFiles/mulproc_display.dir/window_proc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mulproc_display.dir/window_proc.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vinstarry/projects/osdesign/mulproc_display/window_proc.c -o CMakeFiles/mulproc_display.dir/window_proc.c.s

# Object files for target mulproc_display
mulproc_display_OBJECTS = \
"CMakeFiles/mulproc_display.dir/main.c.o" \
"CMakeFiles/mulproc_display.dir/window_proc.c.o"

# External object files for target mulproc_display
mulproc_display_EXTERNAL_OBJECTS =

mulproc_display: CMakeFiles/mulproc_display.dir/main.c.o
mulproc_display: CMakeFiles/mulproc_display.dir/window_proc.c.o
mulproc_display: CMakeFiles/mulproc_display.dir/build.make
mulproc_display: CMakeFiles/mulproc_display.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable mulproc_display"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mulproc_display.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mulproc_display.dir/build: mulproc_display

.PHONY : CMakeFiles/mulproc_display.dir/build

CMakeFiles/mulproc_display.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mulproc_display.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mulproc_display.dir/clean

CMakeFiles/mulproc_display.dir/depend:
	cd /home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vinstarry/projects/osdesign/mulproc_display /home/vinstarry/projects/osdesign/mulproc_display /home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug /home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug /home/vinstarry/projects/osdesign/mulproc_display/cmake-build-debug/CMakeFiles/mulproc_display.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mulproc_display.dir/depend
