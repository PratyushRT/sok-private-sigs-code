# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alishahc/libgroupsig

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alishahc/libgroupsig/build

# Include any dependencies generated for this target.
include src/groupsig/klap20/CMakeFiles/klap20.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.make

# Include the progress variables for this target.
include src/groupsig/klap20/CMakeFiles/klap20.dir/progress.make

# Include the compile flags for this target's objects.
include src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make

src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.o: ../src/groupsig/klap20/grp_key.c
src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.o -MF CMakeFiles/klap20.dir/grp_key.c.o.d -o CMakeFiles/klap20.dir/grp_key.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/grp_key.c

src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/grp_key.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/grp_key.c > CMakeFiles/klap20.dir/grp_key.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/grp_key.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/grp_key.c -o CMakeFiles/klap20.dir/grp_key.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.o: ../src/groupsig/klap20/mgr_key.c
src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.o -MF CMakeFiles/klap20.dir/mgr_key.c.o.d -o CMakeFiles/klap20.dir/mgr_key.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/mgr_key.c

src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/mgr_key.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/mgr_key.c > CMakeFiles/klap20.dir/mgr_key.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/mgr_key.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/mgr_key.c -o CMakeFiles/klap20.dir/mgr_key.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.o: ../src/groupsig/klap20/mem_key.c
src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.o -MF CMakeFiles/klap20.dir/mem_key.c.o.d -o CMakeFiles/klap20.dir/mem_key.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/mem_key.c

src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/mem_key.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/mem_key.c > CMakeFiles/klap20.dir/mem_key.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/mem_key.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/mem_key.c -o CMakeFiles/klap20.dir/mem_key.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.o: ../src/groupsig/klap20/signature.c
src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.o -MF CMakeFiles/klap20.dir/signature.c.o.d -o CMakeFiles/klap20.dir/signature.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/signature.c

src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/signature.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/signature.c > CMakeFiles/klap20.dir/signature.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/signature.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/signature.c -o CMakeFiles/klap20.dir/signature.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.o: ../src/groupsig/klap20/setup.c
src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.o -MF CMakeFiles/klap20.dir/setup.c.o.d -o CMakeFiles/klap20.dir/setup.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/setup.c

src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/setup.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/setup.c > CMakeFiles/klap20.dir/setup.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/setup.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/setup.c -o CMakeFiles/klap20.dir/setup.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.o: ../src/groupsig/klap20/join_mem.c
src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.o -MF CMakeFiles/klap20.dir/join_mem.c.o.d -o CMakeFiles/klap20.dir/join_mem.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/join_mem.c

src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/join_mem.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/join_mem.c > CMakeFiles/klap20.dir/join_mem.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/join_mem.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/join_mem.c -o CMakeFiles/klap20.dir/join_mem.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.o: ../src/groupsig/klap20/join_mgr.c
src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.o -MF CMakeFiles/klap20.dir/join_mgr.c.o.d -o CMakeFiles/klap20.dir/join_mgr.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/join_mgr.c

src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/join_mgr.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/join_mgr.c > CMakeFiles/klap20.dir/join_mgr.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/join_mgr.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/join_mgr.c -o CMakeFiles/klap20.dir/join_mgr.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.o: ../src/groupsig/klap20/sign.c
src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.o -MF CMakeFiles/klap20.dir/sign.c.o.d -o CMakeFiles/klap20.dir/sign.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/sign.c

src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/sign.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/sign.c > CMakeFiles/klap20.dir/sign.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/sign.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/sign.c -o CMakeFiles/klap20.dir/sign.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.o: ../src/groupsig/klap20/verify.c
src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.o -MF CMakeFiles/klap20.dir/verify.c.o.d -o CMakeFiles/klap20.dir/verify.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/verify.c

src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/verify.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/verify.c > CMakeFiles/klap20.dir/verify.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/verify.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/verify.c -o CMakeFiles/klap20.dir/verify.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.o: ../src/groupsig/klap20/open.c
src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.o -MF CMakeFiles/klap20.dir/open.c.o.d -o CMakeFiles/klap20.dir/open.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/open.c

src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/open.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/open.c > CMakeFiles/klap20.dir/open.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/open.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/open.c -o CMakeFiles/klap20.dir/open.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.o: ../src/groupsig/klap20/open_verify.c
src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.o -MF CMakeFiles/klap20.dir/open_verify.c.o.d -o CMakeFiles/klap20.dir/open_verify.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/open_verify.c

src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/open_verify.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/open_verify.c > CMakeFiles/klap20.dir/open_verify.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/open_verify.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/open_verify.c -o CMakeFiles/klap20.dir/open_verify.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.o: ../src/groupsig/klap20/gml.c
src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.o -MF CMakeFiles/klap20.dir/gml.c.o.d -o CMakeFiles/klap20.dir/gml.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/gml.c

src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/gml.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/gml.c > CMakeFiles/klap20.dir/gml.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/gml.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/gml.c -o CMakeFiles/klap20.dir/gml.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.o: ../src/groupsig/klap20/spk.c
src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.o -MF CMakeFiles/klap20.dir/spk.c.o.d -o CMakeFiles/klap20.dir/spk.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/spk.c

src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/spk.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/spk.c > CMakeFiles/klap20.dir/spk.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/spk.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/spk.c -o CMakeFiles/klap20.dir/spk.c.s

src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/flags.make
src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.o: ../src/groupsig/klap20/proof.c
src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.o: src/groupsig/klap20/CMakeFiles/klap20.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building C object src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.o"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.o -MF CMakeFiles/klap20.dir/proof.c.o.d -o CMakeFiles/klap20.dir/proof.c.o -c /home/alishahc/libgroupsig/src/groupsig/klap20/proof.c

src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/klap20.dir/proof.c.i"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/alishahc/libgroupsig/src/groupsig/klap20/proof.c > CMakeFiles/klap20.dir/proof.c.i

src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/klap20.dir/proof.c.s"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/alishahc/libgroupsig/src/groupsig/klap20/proof.c -o CMakeFiles/klap20.dir/proof.c.s

# Object files for target klap20
klap20_OBJECTS = \
"CMakeFiles/klap20.dir/grp_key.c.o" \
"CMakeFiles/klap20.dir/mgr_key.c.o" \
"CMakeFiles/klap20.dir/mem_key.c.o" \
"CMakeFiles/klap20.dir/signature.c.o" \
"CMakeFiles/klap20.dir/setup.c.o" \
"CMakeFiles/klap20.dir/join_mem.c.o" \
"CMakeFiles/klap20.dir/join_mgr.c.o" \
"CMakeFiles/klap20.dir/sign.c.o" \
"CMakeFiles/klap20.dir/verify.c.o" \
"CMakeFiles/klap20.dir/open.c.o" \
"CMakeFiles/klap20.dir/open_verify.c.o" \
"CMakeFiles/klap20.dir/gml.c.o" \
"CMakeFiles/klap20.dir/spk.c.o" \
"CMakeFiles/klap20.dir/proof.c.o"

# External object files for target klap20
klap20_EXTERNAL_OBJECTS =

lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/grp_key.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/mgr_key.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/mem_key.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/signature.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/setup.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/join_mem.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/join_mgr.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/sign.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/verify.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/open.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/open_verify.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/gml.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/spk.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/proof.c.o
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/build.make
lib/libklap20.a: src/groupsig/klap20/CMakeFiles/klap20.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alishahc/libgroupsig/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking C static library ../../../lib/libklap20.a"
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && $(CMAKE_COMMAND) -P CMakeFiles/klap20.dir/cmake_clean_target.cmake
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/klap20.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/groupsig/klap20/CMakeFiles/klap20.dir/build: lib/libklap20.a
.PHONY : src/groupsig/klap20/CMakeFiles/klap20.dir/build

src/groupsig/klap20/CMakeFiles/klap20.dir/clean:
	cd /home/alishahc/libgroupsig/build/src/groupsig/klap20 && $(CMAKE_COMMAND) -P CMakeFiles/klap20.dir/cmake_clean.cmake
.PHONY : src/groupsig/klap20/CMakeFiles/klap20.dir/clean

src/groupsig/klap20/CMakeFiles/klap20.dir/depend:
	cd /home/alishahc/libgroupsig/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alishahc/libgroupsig /home/alishahc/libgroupsig/src/groupsig/klap20 /home/alishahc/libgroupsig/build /home/alishahc/libgroupsig/build/src/groupsig/klap20 /home/alishahc/libgroupsig/build/src/groupsig/klap20/CMakeFiles/klap20.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/groupsig/klap20/CMakeFiles/klap20.dir/depend
