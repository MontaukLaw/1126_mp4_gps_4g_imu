# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /opt/cmake/bin/cmake

# The command to remove a file.
RM = /opt/cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/marc/my_project/1126_mp4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marc/my_project/1126_mp4/build

# Include any dependencies generated for this target.
include CMakeFiles/imu.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/imu.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/imu.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/imu.dir/flags.make

CMakeFiles/imu.dir/src/imu_test.cpp.o: CMakeFiles/imu.dir/flags.make
CMakeFiles/imu.dir/src/imu_test.cpp.o: /home/marc/my_project/1126_mp4/src/imu_test.cpp
CMakeFiles/imu.dir/src/imu_test.cpp.o: CMakeFiles/imu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/imu.dir/src/imu_test.cpp.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/imu.dir/src/imu_test.cpp.o -MF CMakeFiles/imu.dir/src/imu_test.cpp.o.d -o CMakeFiles/imu.dir/src/imu_test.cpp.o -c /home/marc/my_project/1126_mp4/src/imu_test.cpp

CMakeFiles/imu.dir/src/imu_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imu.dir/src/imu_test.cpp.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/imu_test.cpp > CMakeFiles/imu.dir/src/imu_test.cpp.i

CMakeFiles/imu.dir/src/imu_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imu.dir/src/imu_test.cpp.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/imu_test.cpp -o CMakeFiles/imu.dir/src/imu_test.cpp.s

# Object files for target imu
imu_OBJECTS = \
"CMakeFiles/imu.dir/src/imu_test.cpp.o"

# External object files for target imu
imu_EXTERNAL_OBJECTS =

imu: CMakeFiles/imu.dir/src/imu_test.cpp.o
imu: CMakeFiles/imu.dir/build.make
imu: CMakeFiles/imu.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable imu"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imu.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/imu.dir/build: imu
.PHONY : CMakeFiles/imu.dir/build

CMakeFiles/imu.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/imu.dir/cmake_clean.cmake
.PHONY : CMakeFiles/imu.dir/clean

CMakeFiles/imu.dir/depend:
	cd /home/marc/my_project/1126_mp4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marc/my_project/1126_mp4 /home/marc/my_project/1126_mp4 /home/marc/my_project/1126_mp4/build /home/marc/my_project/1126_mp4/build /home/marc/my_project/1126_mp4/build/CMakeFiles/imu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/imu.dir/depend

