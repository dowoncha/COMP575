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
CMAKE_SOURCE_DIR = /home/dowoncha/Github/COMP575/PA3/lib/freeglut

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dowoncha/Github/COMP575/PA3/lib/freeglut

# Include any dependencies generated for this target.
include CMakeFiles/One.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/One.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/One.dir/flags.make

CMakeFiles/One.dir/progs/demos/One/one.c.o: CMakeFiles/One.dir/flags.make
CMakeFiles/One.dir/progs/demos/One/one.c.o: progs/demos/One/one.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dowoncha/Github/COMP575/PA3/lib/freeglut/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/One.dir/progs/demos/One/one.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/One.dir/progs/demos/One/one.c.o   -c /home/dowoncha/Github/COMP575/PA3/lib/freeglut/progs/demos/One/one.c

CMakeFiles/One.dir/progs/demos/One/one.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/One.dir/progs/demos/One/one.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/dowoncha/Github/COMP575/PA3/lib/freeglut/progs/demos/One/one.c > CMakeFiles/One.dir/progs/demos/One/one.c.i

CMakeFiles/One.dir/progs/demos/One/one.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/One.dir/progs/demos/One/one.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/dowoncha/Github/COMP575/PA3/lib/freeglut/progs/demos/One/one.c -o CMakeFiles/One.dir/progs/demos/One/one.c.s

CMakeFiles/One.dir/progs/demos/One/one.c.o.requires:
.PHONY : CMakeFiles/One.dir/progs/demos/One/one.c.o.requires

CMakeFiles/One.dir/progs/demos/One/one.c.o.provides: CMakeFiles/One.dir/progs/demos/One/one.c.o.requires
	$(MAKE) -f CMakeFiles/One.dir/build.make CMakeFiles/One.dir/progs/demos/One/one.c.o.provides.build
.PHONY : CMakeFiles/One.dir/progs/demos/One/one.c.o.provides

CMakeFiles/One.dir/progs/demos/One/one.c.o.provides.build: CMakeFiles/One.dir/progs/demos/One/one.c.o

# Object files for target One
One_OBJECTS = \
"CMakeFiles/One.dir/progs/demos/One/one.c.o"

# External object files for target One
One_EXTERNAL_OBJECTS =

bin/One: CMakeFiles/One.dir/progs/demos/One/one.c.o
bin/One: CMakeFiles/One.dir/build.make
bin/One: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/One: /usr/lib/x86_64-linux-gnu/libGL.so
bin/One: /usr/lib/x86_64-linux-gnu/libSM.so
bin/One: /usr/lib/x86_64-linux-gnu/libICE.so
bin/One: /usr/lib/x86_64-linux-gnu/libX11.so
bin/One: /usr/lib/x86_64-linux-gnu/libXext.so
bin/One: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/One: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
bin/One: /usr/lib/x86_64-linux-gnu/libXi.so
bin/One: lib/libglut.so.3.10.0
bin/One: /usr/lib/x86_64-linux-gnu/libGL.so
bin/One: /usr/lib/x86_64-linux-gnu/libSM.so
bin/One: /usr/lib/x86_64-linux-gnu/libICE.so
bin/One: /usr/lib/x86_64-linux-gnu/libX11.so
bin/One: /usr/lib/x86_64-linux-gnu/libXext.so
bin/One: /usr/lib/x86_64-linux-gnu/libXrandr.so
bin/One: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
bin/One: /usr/lib/x86_64-linux-gnu/libXi.so
bin/One: CMakeFiles/One.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable bin/One"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/One.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/One.dir/build: bin/One
.PHONY : CMakeFiles/One.dir/build

CMakeFiles/One.dir/requires: CMakeFiles/One.dir/progs/demos/One/one.c.o.requires
.PHONY : CMakeFiles/One.dir/requires

CMakeFiles/One.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/One.dir/cmake_clean.cmake
.PHONY : CMakeFiles/One.dir/clean

CMakeFiles/One.dir/depend:
	cd /home/dowoncha/Github/COMP575/PA3/lib/freeglut && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dowoncha/Github/COMP575/PA3/lib/freeglut /home/dowoncha/Github/COMP575/PA3/lib/freeglut /home/dowoncha/Github/COMP575/PA3/lib/freeglut /home/dowoncha/Github/COMP575/PA3/lib/freeglut /home/dowoncha/Github/COMP575/PA3/lib/freeglut/CMakeFiles/One.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/One.dir/depend

