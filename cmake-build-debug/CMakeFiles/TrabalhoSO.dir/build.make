# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /home/delfim/Desktop/clion-2017.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/delfim/Desktop/clion-2017.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/delfim/CLionProjects/TrabalhoSO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TrabalhoSO.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TrabalhoSO.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TrabalhoSO.dir/flags.make

CMakeFiles/TrabalhoSO.dir/admin.c.o: CMakeFiles/TrabalhoSO.dir/flags.make
CMakeFiles/TrabalhoSO.dir/admin.c.o: ../admin.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/TrabalhoSO.dir/admin.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TrabalhoSO.dir/admin.c.o   -c /home/delfim/CLionProjects/TrabalhoSO/admin.c

CMakeFiles/TrabalhoSO.dir/admin.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TrabalhoSO.dir/admin.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/delfim/CLionProjects/TrabalhoSO/admin.c > CMakeFiles/TrabalhoSO.dir/admin.c.i

CMakeFiles/TrabalhoSO.dir/admin.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TrabalhoSO.dir/admin.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/delfim/CLionProjects/TrabalhoSO/admin.c -o CMakeFiles/TrabalhoSO.dir/admin.c.s

CMakeFiles/TrabalhoSO.dir/admin.c.o.requires:

.PHONY : CMakeFiles/TrabalhoSO.dir/admin.c.o.requires

CMakeFiles/TrabalhoSO.dir/admin.c.o.provides: CMakeFiles/TrabalhoSO.dir/admin.c.o.requires
	$(MAKE) -f CMakeFiles/TrabalhoSO.dir/build.make CMakeFiles/TrabalhoSO.dir/admin.c.o.provides.build
.PHONY : CMakeFiles/TrabalhoSO.dir/admin.c.o.provides

CMakeFiles/TrabalhoSO.dir/admin.c.o.provides.build: CMakeFiles/TrabalhoSO.dir/admin.c.o


CMakeFiles/TrabalhoSO.dir/cliente.c.o: CMakeFiles/TrabalhoSO.dir/flags.make
CMakeFiles/TrabalhoSO.dir/cliente.c.o: ../cliente.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/TrabalhoSO.dir/cliente.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TrabalhoSO.dir/cliente.c.o   -c /home/delfim/CLionProjects/TrabalhoSO/cliente.c

CMakeFiles/TrabalhoSO.dir/cliente.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TrabalhoSO.dir/cliente.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/delfim/CLionProjects/TrabalhoSO/cliente.c > CMakeFiles/TrabalhoSO.dir/cliente.c.i

CMakeFiles/TrabalhoSO.dir/cliente.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TrabalhoSO.dir/cliente.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/delfim/CLionProjects/TrabalhoSO/cliente.c -o CMakeFiles/TrabalhoSO.dir/cliente.c.s

CMakeFiles/TrabalhoSO.dir/cliente.c.o.requires:

.PHONY : CMakeFiles/TrabalhoSO.dir/cliente.c.o.requires

CMakeFiles/TrabalhoSO.dir/cliente.c.o.provides: CMakeFiles/TrabalhoSO.dir/cliente.c.o.requires
	$(MAKE) -f CMakeFiles/TrabalhoSO.dir/build.make CMakeFiles/TrabalhoSO.dir/cliente.c.o.provides.build
.PHONY : CMakeFiles/TrabalhoSO.dir/cliente.c.o.provides

CMakeFiles/TrabalhoSO.dir/cliente.c.o.provides.build: CMakeFiles/TrabalhoSO.dir/cliente.c.o


CMakeFiles/TrabalhoSO.dir/servidor.c.o: CMakeFiles/TrabalhoSO.dir/flags.make
CMakeFiles/TrabalhoSO.dir/servidor.c.o: ../servidor.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/TrabalhoSO.dir/servidor.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/TrabalhoSO.dir/servidor.c.o   -c /home/delfim/CLionProjects/TrabalhoSO/servidor.c

CMakeFiles/TrabalhoSO.dir/servidor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/TrabalhoSO.dir/servidor.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/delfim/CLionProjects/TrabalhoSO/servidor.c > CMakeFiles/TrabalhoSO.dir/servidor.c.i

CMakeFiles/TrabalhoSO.dir/servidor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/TrabalhoSO.dir/servidor.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/delfim/CLionProjects/TrabalhoSO/servidor.c -o CMakeFiles/TrabalhoSO.dir/servidor.c.s

CMakeFiles/TrabalhoSO.dir/servidor.c.o.requires:

.PHONY : CMakeFiles/TrabalhoSO.dir/servidor.c.o.requires

CMakeFiles/TrabalhoSO.dir/servidor.c.o.provides: CMakeFiles/TrabalhoSO.dir/servidor.c.o.requires
	$(MAKE) -f CMakeFiles/TrabalhoSO.dir/build.make CMakeFiles/TrabalhoSO.dir/servidor.c.o.provides.build
.PHONY : CMakeFiles/TrabalhoSO.dir/servidor.c.o.provides

CMakeFiles/TrabalhoSO.dir/servidor.c.o.provides.build: CMakeFiles/TrabalhoSO.dir/servidor.c.o


# Object files for target TrabalhoSO
TrabalhoSO_OBJECTS = \
"CMakeFiles/TrabalhoSO.dir/admin.c.o" \
"CMakeFiles/TrabalhoSO.dir/cliente.c.o" \
"CMakeFiles/TrabalhoSO.dir/servidor.c.o"

# External object files for target TrabalhoSO
TrabalhoSO_EXTERNAL_OBJECTS =

TrabalhoSO: CMakeFiles/TrabalhoSO.dir/admin.c.o
TrabalhoSO: CMakeFiles/TrabalhoSO.dir/cliente.c.o
TrabalhoSO: CMakeFiles/TrabalhoSO.dir/servidor.c.o
TrabalhoSO: CMakeFiles/TrabalhoSO.dir/build.make
TrabalhoSO: CMakeFiles/TrabalhoSO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable TrabalhoSO"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TrabalhoSO.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TrabalhoSO.dir/build: TrabalhoSO

.PHONY : CMakeFiles/TrabalhoSO.dir/build

CMakeFiles/TrabalhoSO.dir/requires: CMakeFiles/TrabalhoSO.dir/admin.c.o.requires
CMakeFiles/TrabalhoSO.dir/requires: CMakeFiles/TrabalhoSO.dir/cliente.c.o.requires
CMakeFiles/TrabalhoSO.dir/requires: CMakeFiles/TrabalhoSO.dir/servidor.c.o.requires

.PHONY : CMakeFiles/TrabalhoSO.dir/requires

CMakeFiles/TrabalhoSO.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TrabalhoSO.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TrabalhoSO.dir/clean

CMakeFiles/TrabalhoSO.dir/depend:
	cd /home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/delfim/CLionProjects/TrabalhoSO /home/delfim/CLionProjects/TrabalhoSO /home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug /home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug /home/delfim/CLionProjects/TrabalhoSO/cmake-build-debug/CMakeFiles/TrabalhoSO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TrabalhoSO.dir/depend

