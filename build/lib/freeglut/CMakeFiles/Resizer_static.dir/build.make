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
include lib/freeglut/CMakeFiles/Resizer_static.dir/depend.make

# Include the progress variables for this target.
include lib/freeglut/CMakeFiles/Resizer_static.dir/progress.make

# Include the compile flags for this target's objects.
include lib/freeglut/CMakeFiles/Resizer_static.dir/flags.make

lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o: lib/freeglut/CMakeFiles/Resizer_static.dir/flags.make
lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o: ../lib/freeglut/progs/demos/Resizer/Resizer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dowoncha/Documents/comp575_pa2/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && /usr/bin/clang   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o -c /home/dowoncha/Documents/comp575_pa2/lib/freeglut/progs/demos/Resizer/Resizer.cpp

lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.i"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && /usr/bin/clang  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/dowoncha/Documents/comp575_pa2/lib/freeglut/progs/demos/Resizer/Resizer.cpp > CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.i

lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.s"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && /usr/bin/clang  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/dowoncha/Documents/comp575_pa2/lib/freeglut/progs/demos/Resizer/Resizer.cpp -o CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.s

lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.requires:
.PHONY : lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.requires

lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.provides: lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.requires
	$(MAKE) -f lib/freeglut/CMakeFiles/Resizer_static.dir/build.make lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.provides.build
.PHONY : lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.provides

lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.provides.build: lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o

# Object files for target Resizer_static
Resizer_static_OBJECTS = \
"CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o"

# External object files for target Resizer_static
Resizer_static_EXTERNAL_OBJECTS =

lib/freeglut/bin/Resizer_static: lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o
lib/freeglut/bin/Resizer_static: lib/freeglut/CMakeFiles/Resizer_static.dir/build.make
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libGLU.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libGL.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libSM.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libICE.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libX11.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXext.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXrandr.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXi.so
lib/freeglut/bin/Resizer_static: lib/freeglut/lib/libglut.a
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libGL.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libSM.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libICE.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libX11.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXext.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXrandr.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
lib/freeglut/bin/Resizer_static: /usr/lib/x86_64-linux-gnu/libXi.so
lib/freeglut/bin/Resizer_static: lib/freeglut/CMakeFiles/Resizer_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/Resizer_static"
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Resizer_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/freeglut/CMakeFiles/Resizer_static.dir/build: lib/freeglut/bin/Resizer_static
.PHONY : lib/freeglut/CMakeFiles/Resizer_static.dir/build

lib/freeglut/CMakeFiles/Resizer_static.dir/requires: lib/freeglut/CMakeFiles/Resizer_static.dir/progs/demos/Resizer/Resizer.cpp.o.requires
.PHONY : lib/freeglut/CMakeFiles/Resizer_static.dir/requires

lib/freeglut/CMakeFiles/Resizer_static.dir/clean:
	cd /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut && $(CMAKE_COMMAND) -P CMakeFiles/Resizer_static.dir/cmake_clean.cmake
.PHONY : lib/freeglut/CMakeFiles/Resizer_static.dir/clean

lib/freeglut/CMakeFiles/Resizer_static.dir/depend:
	cd /home/dowoncha/Documents/comp575_pa2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dowoncha/Documents/comp575_pa2 /home/dowoncha/Documents/comp575_pa2/lib/freeglut /home/dowoncha/Documents/comp575_pa2/build /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut /home/dowoncha/Documents/comp575_pa2/build/lib/freeglut/CMakeFiles/Resizer_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/freeglut/CMakeFiles/Resizer_static.dir/depend

