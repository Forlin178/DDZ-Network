# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\38491\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\38491\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\CLionProject\DDZ-Network\Server-ddz

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug

# Include any dependencies generated for this target.
include reactor/CMakeFiles/reactor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include reactor/CMakeFiles/reactor.dir/compiler_depend.make

# Include the progress variables for this target.
include reactor/CMakeFiles/reactor.dir/progress.make

# Include the compile flags for this target's objects.
include reactor/CMakeFiles/reactor.dir/flags.make

reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/includes_CXX.rsp
reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.obj: D:/CLionProject/DDZ-Network/Server-ddz/reactor/Dispatcher.cpp
reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.obj"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.obj -MF CMakeFiles\reactor.dir\Dispatcher.cpp.obj.d -o CMakeFiles\reactor.dir\Dispatcher.cpp.obj -c D:\CLionProject\DDZ-Network\Server-ddz\reactor\Dispatcher.cpp

reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/Dispatcher.cpp.i"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CLionProject\DDZ-Network\Server-ddz\reactor\Dispatcher.cpp > CMakeFiles\reactor.dir\Dispatcher.cpp.i

reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/Dispatcher.cpp.s"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\CLionProject\DDZ-Network\Server-ddz\reactor\Dispatcher.cpp -o CMakeFiles\reactor.dir\Dispatcher.cpp.s

reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/includes_CXX.rsp
reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj: D:/CLionProject/DDZ-Network/Server-ddz/reactor/EpollDispatcher.cpp
reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj -MF CMakeFiles\reactor.dir\EpollDispatcher.cpp.obj.d -o CMakeFiles\reactor.dir\EpollDispatcher.cpp.obj -c D:\CLionProject\DDZ-Network\Server-ddz\reactor\EpollDispatcher.cpp

reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/EpollDispatcher.cpp.i"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CLionProject\DDZ-Network\Server-ddz\reactor\EpollDispatcher.cpp > CMakeFiles\reactor.dir\EpollDispatcher.cpp.i

reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/EpollDispatcher.cpp.s"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\CLionProject\DDZ-Network\Server-ddz\reactor\EpollDispatcher.cpp -o CMakeFiles\reactor.dir\EpollDispatcher.cpp.s

reactor/CMakeFiles/reactor.dir/EventLoop.cpp.obj: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/EventLoop.cpp.obj: reactor/CMakeFiles/reactor.dir/includes_CXX.rsp
reactor/CMakeFiles/reactor.dir/EventLoop.cpp.obj: D:/CLionProject/DDZ-Network/Server-ddz/reactor/EventLoop.cpp
reactor/CMakeFiles/reactor.dir/EventLoop.cpp.obj: reactor/CMakeFiles/reactor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object reactor/CMakeFiles/reactor.dir/EventLoop.cpp.obj"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reactor/CMakeFiles/reactor.dir/EventLoop.cpp.obj -MF CMakeFiles\reactor.dir\EventLoop.cpp.obj.d -o CMakeFiles\reactor.dir\EventLoop.cpp.obj -c D:\CLionProject\DDZ-Network\Server-ddz\reactor\EventLoop.cpp

reactor/CMakeFiles/reactor.dir/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/EventLoop.cpp.i"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CLionProject\DDZ-Network\Server-ddz\reactor\EventLoop.cpp > CMakeFiles\reactor.dir\EventLoop.cpp.i

reactor/CMakeFiles/reactor.dir/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/EventLoop.cpp.s"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\CLionProject\DDZ-Network\Server-ddz\reactor\EventLoop.cpp -o CMakeFiles\reactor.dir\EventLoop.cpp.s

reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/includes_CXX.rsp
reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.obj: D:/CLionProject/DDZ-Network/Server-ddz/reactor/PollDispatcher.cpp
reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.obj"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.obj -MF CMakeFiles\reactor.dir\PollDispatcher.cpp.obj.d -o CMakeFiles\reactor.dir\PollDispatcher.cpp.obj -c D:\CLionProject\DDZ-Network\Server-ddz\reactor\PollDispatcher.cpp

reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/PollDispatcher.cpp.i"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CLionProject\DDZ-Network\Server-ddz\reactor\PollDispatcher.cpp > CMakeFiles\reactor.dir\PollDispatcher.cpp.i

reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/PollDispatcher.cpp.s"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\CLionProject\DDZ-Network\Server-ddz\reactor\PollDispatcher.cpp -o CMakeFiles\reactor.dir\PollDispatcher.cpp.s

reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/flags.make
reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/includes_CXX.rsp
reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj: D:/CLionProject/DDZ-Network/Server-ddz/reactor/SelectDispatcher.cpp
reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj: reactor/CMakeFiles/reactor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj -MF CMakeFiles\reactor.dir\SelectDispatcher.cpp.obj.d -o CMakeFiles\reactor.dir\SelectDispatcher.cpp.obj -c D:\CLionProject\DDZ-Network\Server-ddz\reactor\SelectDispatcher.cpp

reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reactor.dir/SelectDispatcher.cpp.i"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\CLionProject\DDZ-Network\Server-ddz\reactor\SelectDispatcher.cpp > CMakeFiles\reactor.dir\SelectDispatcher.cpp.i

reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reactor.dir/SelectDispatcher.cpp.s"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\CLionProject\DDZ-Network\Server-ddz\reactor\SelectDispatcher.cpp -o CMakeFiles\reactor.dir\SelectDispatcher.cpp.s

# Object files for target reactor
reactor_OBJECTS = \
"CMakeFiles/reactor.dir/Dispatcher.cpp.obj" \
"CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj" \
"CMakeFiles/reactor.dir/EventLoop.cpp.obj" \
"CMakeFiles/reactor.dir/PollDispatcher.cpp.obj" \
"CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj"

# External object files for target reactor
reactor_EXTERNAL_OBJECTS =

reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/Dispatcher.cpp.obj
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/EpollDispatcher.cpp.obj
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/EventLoop.cpp.obj
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/PollDispatcher.cpp.obj
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/SelectDispatcher.cpp.obj
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/build.make
reactor/libreactor.a: reactor/CMakeFiles/reactor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library libreactor.a"
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && $(CMAKE_COMMAND) -P CMakeFiles\reactor.dir\cmake_clean_target.cmake
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\reactor.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
reactor/CMakeFiles/reactor.dir/build: reactor/libreactor.a
.PHONY : reactor/CMakeFiles/reactor.dir/build

reactor/CMakeFiles/reactor.dir/clean:
	cd /d D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor && $(CMAKE_COMMAND) -P CMakeFiles\reactor.dir\cmake_clean.cmake
.PHONY : reactor/CMakeFiles/reactor.dir/clean

reactor/CMakeFiles/reactor.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\CLionProject\DDZ-Network\Server-ddz D:\CLionProject\DDZ-Network\Server-ddz\reactor D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor D:\CLionProject\DDZ-Network\Server-ddz\cmake-build-debug\reactor\CMakeFiles\reactor.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : reactor/CMakeFiles/reactor.dir/depend
