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
CMAKE_SOURCE_DIR = /home/forlin/project/ddz-net

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/forlin/project/ddz-net/cmake-build-remote-debug

# Include any dependencies generated for this target.
include database/CMakeFiles/database.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include database/CMakeFiles/database.dir/compiler_depend.make

# Include the progress variables for this target.
include database/CMakeFiles/database.dir/progress.make

# Include the compile flags for this target's objects.
include database/CMakeFiles/database.dir/flags.make

database/CMakeFiles/database.dir/MysqlConn.cpp.o: database/CMakeFiles/database.dir/flags.make
database/CMakeFiles/database.dir/MysqlConn.cpp.o: ../database/MysqlConn.cpp
database/CMakeFiles/database.dir/MysqlConn.cpp.o: database/CMakeFiles/database.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/forlin/project/ddz-net/cmake-build-remote-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object database/CMakeFiles/database.dir/MysqlConn.cpp.o"
	cd /home/forlin/project/ddz-net/cmake-build-remote-debug/database && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT database/CMakeFiles/database.dir/MysqlConn.cpp.o -MF CMakeFiles/database.dir/MysqlConn.cpp.o.d -o CMakeFiles/database.dir/MysqlConn.cpp.o -c /home/forlin/project/ddz-net/database/MysqlConn.cpp

database/CMakeFiles/database.dir/MysqlConn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/database.dir/MysqlConn.cpp.i"
	cd /home/forlin/project/ddz-net/cmake-build-remote-debug/database && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/forlin/project/ddz-net/database/MysqlConn.cpp > CMakeFiles/database.dir/MysqlConn.cpp.i

database/CMakeFiles/database.dir/MysqlConn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/database.dir/MysqlConn.cpp.s"
	cd /home/forlin/project/ddz-net/cmake-build-remote-debug/database && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/forlin/project/ddz-net/database/MysqlConn.cpp -o CMakeFiles/database.dir/MysqlConn.cpp.s

# Object files for target database
database_OBJECTS = \
"CMakeFiles/database.dir/MysqlConn.cpp.o"

# External object files for target database
database_EXTERNAL_OBJECTS =

database/libdatabase.a: database/CMakeFiles/database.dir/MysqlConn.cpp.o
database/libdatabase.a: database/CMakeFiles/database.dir/build.make
database/libdatabase.a: database/CMakeFiles/database.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/forlin/project/ddz-net/cmake-build-remote-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libdatabase.a"
	cd /home/forlin/project/ddz-net/cmake-build-remote-debug/database && $(CMAKE_COMMAND) -P CMakeFiles/database.dir/cmake_clean_target.cmake
	cd /home/forlin/project/ddz-net/cmake-build-remote-debug/database && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/database.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
database/CMakeFiles/database.dir/build: database/libdatabase.a
.PHONY : database/CMakeFiles/database.dir/build

database/CMakeFiles/database.dir/clean:
	cd /home/forlin/project/ddz-net/cmake-build-remote-debug/database && $(CMAKE_COMMAND) -P CMakeFiles/database.dir/cmake_clean.cmake
.PHONY : database/CMakeFiles/database.dir/clean

database/CMakeFiles/database.dir/depend:
	cd /home/forlin/project/ddz-net/cmake-build-remote-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/forlin/project/ddz-net /home/forlin/project/ddz-net/database /home/forlin/project/ddz-net/cmake-build-remote-debug /home/forlin/project/ddz-net/cmake-build-remote-debug/database /home/forlin/project/ddz-net/cmake-build-remote-debug/database/CMakeFiles/database.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : database/CMakeFiles/database.dir/depend
