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
CMAKE_SOURCE_DIR = /home/marc/my_project/1126_h264

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marc/my_project/1126_h264/build

# Include any dependencies generated for this target.
include CMakeFiles/uploader.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/uploader.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/uploader.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/uploader.dir/flags.make

CMakeFiles/uploader.dir/src/upload_demo.cc.o: CMakeFiles/uploader.dir/flags.make
CMakeFiles/uploader.dir/src/upload_demo.cc.o: /home/marc/my_project/1126_h264/src/upload_demo.cc
CMakeFiles/uploader.dir/src/upload_demo.cc.o: CMakeFiles/uploader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_h264/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/uploader.dir/src/upload_demo.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/uploader.dir/src/upload_demo.cc.o -MF CMakeFiles/uploader.dir/src/upload_demo.cc.o.d -o CMakeFiles/uploader.dir/src/upload_demo.cc.o -c /home/marc/my_project/1126_h264/src/upload_demo.cc

CMakeFiles/uploader.dir/src/upload_demo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/uploader.dir/src/upload_demo.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_h264/src/upload_demo.cc > CMakeFiles/uploader.dir/src/upload_demo.cc.i

CMakeFiles/uploader.dir/src/upload_demo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/uploader.dir/src/upload_demo.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_h264/src/upload_demo.cc -o CMakeFiles/uploader.dir/src/upload_demo.cc.s

CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o: CMakeFiles/uploader.dir/flags.make
CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o: /home/marc/my_project/1126_h264/src/simple_ftp_uploader.cc
CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o: CMakeFiles/uploader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_h264/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o -MF CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o.d -o CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o -c /home/marc/my_project/1126_h264/src/simple_ftp_uploader.cc

CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_h264/src/simple_ftp_uploader.cc > CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.i

CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_h264/src/simple_ftp_uploader.cc -o CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.s

CMakeFiles/uploader.dir/src/uploader.cc.o: CMakeFiles/uploader.dir/flags.make
CMakeFiles/uploader.dir/src/uploader.cc.o: /home/marc/my_project/1126_h264/src/uploader.cc
CMakeFiles/uploader.dir/src/uploader.cc.o: CMakeFiles/uploader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_h264/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/uploader.dir/src/uploader.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/uploader.dir/src/uploader.cc.o -MF CMakeFiles/uploader.dir/src/uploader.cc.o.d -o CMakeFiles/uploader.dir/src/uploader.cc.o -c /home/marc/my_project/1126_h264/src/uploader.cc

CMakeFiles/uploader.dir/src/uploader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/uploader.dir/src/uploader.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_h264/src/uploader.cc > CMakeFiles/uploader.dir/src/uploader.cc.i

CMakeFiles/uploader.dir/src/uploader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/uploader.dir/src/uploader.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_h264/src/uploader.cc -o CMakeFiles/uploader.dir/src/uploader.cc.s

CMakeFiles/uploader.dir/src/utils.cc.o: CMakeFiles/uploader.dir/flags.make
CMakeFiles/uploader.dir/src/utils.cc.o: /home/marc/my_project/1126_h264/src/utils.cc
CMakeFiles/uploader.dir/src/utils.cc.o: CMakeFiles/uploader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_h264/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/uploader.dir/src/utils.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/uploader.dir/src/utils.cc.o -MF CMakeFiles/uploader.dir/src/utils.cc.o.d -o CMakeFiles/uploader.dir/src/utils.cc.o -c /home/marc/my_project/1126_h264/src/utils.cc

CMakeFiles/uploader.dir/src/utils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/uploader.dir/src/utils.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_h264/src/utils.cc > CMakeFiles/uploader.dir/src/utils.cc.i

CMakeFiles/uploader.dir/src/utils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/uploader.dir/src/utils.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_h264/src/utils.cc -o CMakeFiles/uploader.dir/src/utils.cc.s

# Object files for target uploader
uploader_OBJECTS = \
"CMakeFiles/uploader.dir/src/upload_demo.cc.o" \
"CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o" \
"CMakeFiles/uploader.dir/src/uploader.cc.o" \
"CMakeFiles/uploader.dir/src/utils.cc.o"

# External object files for target uploader
uploader_EXTERNAL_OBJECTS =

uploader: CMakeFiles/uploader.dir/src/upload_demo.cc.o
uploader: CMakeFiles/uploader.dir/src/simple_ftp_uploader.cc.o
uploader: CMakeFiles/uploader.dir/src/uploader.cc.o
uploader: CMakeFiles/uploader.dir/src/utils.cc.o
uploader: CMakeFiles/uploader.dir/build.make
uploader: /home/marc/my_project/1126_h264/lib/libthird_media.so
uploader: CMakeFiles/uploader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marc/my_project/1126_h264/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable uploader"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/uploader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/uploader.dir/build: uploader
.PHONY : CMakeFiles/uploader.dir/build

CMakeFiles/uploader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/uploader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/uploader.dir/clean

CMakeFiles/uploader.dir/depend:
	cd /home/marc/my_project/1126_h264/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marc/my_project/1126_h264 /home/marc/my_project/1126_h264 /home/marc/my_project/1126_h264/build /home/marc/my_project/1126_h264/build /home/marc/my_project/1126_h264/build/CMakeFiles/uploader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/uploader.dir/depend
