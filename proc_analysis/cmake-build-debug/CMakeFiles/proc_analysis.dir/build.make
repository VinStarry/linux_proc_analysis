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
CMAKE_SOURCE_DIR = /home/vinstarry/projects/osdesign/proc_analysis

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/proc_analysis.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/proc_analysis.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/proc_analysis.dir/flags.make

CMakeFiles/proc_analysis.dir/main.c.o: CMakeFiles/proc_analysis.dir/flags.make
CMakeFiles/proc_analysis.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/proc_analysis.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proc_analysis.dir/main.c.o   -c /home/vinstarry/projects/osdesign/proc_analysis/main.c

CMakeFiles/proc_analysis.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proc_analysis.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vinstarry/projects/osdesign/proc_analysis/main.c > CMakeFiles/proc_analysis.dir/main.c.i

CMakeFiles/proc_analysis.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proc_analysis.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vinstarry/projects/osdesign/proc_analysis/main.c -o CMakeFiles/proc_analysis.dir/main.c.s

CMakeFiles/proc_analysis.dir/mem_page.c.o: CMakeFiles/proc_analysis.dir/flags.make
CMakeFiles/proc_analysis.dir/mem_page.c.o: ../mem_page.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/proc_analysis.dir/mem_page.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proc_analysis.dir/mem_page.c.o   -c /home/vinstarry/projects/osdesign/proc_analysis/mem_page.c

CMakeFiles/proc_analysis.dir/mem_page.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proc_analysis.dir/mem_page.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vinstarry/projects/osdesign/proc_analysis/mem_page.c > CMakeFiles/proc_analysis.dir/mem_page.c.i

CMakeFiles/proc_analysis.dir/mem_page.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proc_analysis.dir/mem_page.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vinstarry/projects/osdesign/proc_analysis/mem_page.c -o CMakeFiles/proc_analysis.dir/mem_page.c.s

CMakeFiles/proc_analysis.dir/ps_page.c.o: CMakeFiles/proc_analysis.dir/flags.make
CMakeFiles/proc_analysis.dir/ps_page.c.o: ../ps_page.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/proc_analysis.dir/ps_page.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proc_analysis.dir/ps_page.c.o   -c /home/vinstarry/projects/osdesign/proc_analysis/ps_page.c

CMakeFiles/proc_analysis.dir/ps_page.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proc_analysis.dir/ps_page.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vinstarry/projects/osdesign/proc_analysis/ps_page.c > CMakeFiles/proc_analysis.dir/ps_page.c.i

CMakeFiles/proc_analysis.dir/ps_page.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proc_analysis.dir/ps_page.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vinstarry/projects/osdesign/proc_analysis/ps_page.c -o CMakeFiles/proc_analysis.dir/ps_page.c.s

CMakeFiles/proc_analysis.dir/sys_page.c.o: CMakeFiles/proc_analysis.dir/flags.make
CMakeFiles/proc_analysis.dir/sys_page.c.o: ../sys_page.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/proc_analysis.dir/sys_page.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/proc_analysis.dir/sys_page.c.o   -c /home/vinstarry/projects/osdesign/proc_analysis/sys_page.c

CMakeFiles/proc_analysis.dir/sys_page.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/proc_analysis.dir/sys_page.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/vinstarry/projects/osdesign/proc_analysis/sys_page.c > CMakeFiles/proc_analysis.dir/sys_page.c.i

CMakeFiles/proc_analysis.dir/sys_page.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/proc_analysis.dir/sys_page.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/vinstarry/projects/osdesign/proc_analysis/sys_page.c -o CMakeFiles/proc_analysis.dir/sys_page.c.s

# Object files for target proc_analysis
proc_analysis_OBJECTS = \
"CMakeFiles/proc_analysis.dir/main.c.o" \
"CMakeFiles/proc_analysis.dir/mem_page.c.o" \
"CMakeFiles/proc_analysis.dir/ps_page.c.o" \
"CMakeFiles/proc_analysis.dir/sys_page.c.o"

# External object files for target proc_analysis
proc_analysis_EXTERNAL_OBJECTS =

proc_analysis: CMakeFiles/proc_analysis.dir/main.c.o
proc_analysis: CMakeFiles/proc_analysis.dir/mem_page.c.o
proc_analysis: CMakeFiles/proc_analysis.dir/ps_page.c.o
proc_analysis: CMakeFiles/proc_analysis.dir/sys_page.c.o
proc_analysis: CMakeFiles/proc_analysis.dir/build.make
proc_analysis: CMakeFiles/proc_analysis.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable proc_analysis"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proc_analysis.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/proc_analysis.dir/build: proc_analysis

.PHONY : CMakeFiles/proc_analysis.dir/build

CMakeFiles/proc_analysis.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/proc_analysis.dir/cmake_clean.cmake
.PHONY : CMakeFiles/proc_analysis.dir/clean

CMakeFiles/proc_analysis.dir/depend:
	cd /home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vinstarry/projects/osdesign/proc_analysis /home/vinstarry/projects/osdesign/proc_analysis /home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug /home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug /home/vinstarry/projects/osdesign/proc_analysis/cmake-build-debug/CMakeFiles/proc_analysis.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/proc_analysis.dir/depend

