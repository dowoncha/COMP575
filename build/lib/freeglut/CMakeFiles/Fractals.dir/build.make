# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/dowoncha/Documents/comp575_pa2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dowoncha/Documents/comp575_pa2/build

# Include any dependencies generated for this target.
include lib/freeglut/CMakeFiles/Fractals.dir/depend.make

# Include the progress variables for this target.
include lib/freeglut/CMakeFiles/Fractals.dir/progress.make

# Include the compile flags for this target's objects.
include lib/freeglut/CMakeFiles/Fractals.dir/flags.make

lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o: lib/freeglut/CMakeFiles/Fractals.dir/flags.make
lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o: ../lib/freeglut/progs/demos/Fractals/fractals.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dowoncha/Documents/comp575_pa2/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o   -c /home/dowoncha/Documents/comp575_pa2/lib/freeglut/progs/demos/Fractals/fractals.c

lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.i"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/dowoncha/Documents/comp575_pa2/lib/freeglut/progs/demos/Fractals/fractals.c > CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.i

lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.s"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/dowoncha/Documents/comp575_pa2/lib/freeglut/progs/demos/Fractals/fractals.c -o CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.s

lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.requires:
.PHONY : lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.requires

lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.provides: lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.requires
	$(MAKE) -f lib/freeglut/CMakeFiles/Fractals.dir/build.make lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.provides.build
.PHONY : lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.provides

lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.provides.build: lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o

# Object files for target Fractals
Fractals_OBJECTS = \
"CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o"

# External object files for target Fractals
Fractals_EXTERNAL_OBJECTS =

lib/freeglut/bin/Fractals: lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o
lib/freeglut/bin/Fractals: lib/freeglut/CMakeFiles/Fractals.dir/build.make
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libGLU.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libGL.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libSM.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libICE.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libX11.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXext.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXrandr.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXi.so
lib/freeglut/bin/Fractals: lib/freeglut/lib/libglut.so.3.10.0
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libGL.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libSM.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libICE.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libX11.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXext.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXrandr.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
lib/freeglut/bin/Fractals: /usr/lib/x86_64-linux-gnu/libXi.so
lib/freeglut/bin/Fractals: lib/freeglut/CMakeFiles/Fractals.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable bin/Fractals"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Fractals.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/freeglut/CMakeFiles/Fractals.dir/build: lib/freeglut/bin/Fractals
.PHONY : lib/freeglut/CMakeFiles/Fractals.dir/build

lib/freeglut/CMakeFiles/Fractals.dir/requires: lib/freeglut/CMakeFiles/Fractals.dir/progs/demos/Fractals/fractals.c.o.requires
.PHONY : lib/freeglut/CMakeFiles/Fractals.dir/requires

lib/freeglut/CMakeFiles/Fractals.dir/clean:
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && $(CMAKE_COMMAND) -P CMakeFiles/Fractals.dir/cmake_clean.cmake
.PHONY : lib/freeglut/CMakeFiles/Fractals.dir/clean

lib/freeglut/CMakeFiles/Fractals.dir/depend:
	cd /home/dowoncha/Documents/comp575_pa2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dowoncha/Documents/comp575_pa2 /home/dowoncha/Documents/comp575_pa2/lib/freeglut /home/dowoncha/Documents/comp575_pa2/build /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut/CMakeFiles/Fractals.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/freeglut/CMakeFiles/Fractals.dir/depend

