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
include CMakeFiles/app.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/app.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/app.dir/flags.make

CMakeFiles/app.dir/src/app.cc.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/src/app.cc.o: /home/marc/my_project/1126_mp4/src/app.cc
CMakeFiles/app.dir/src/app.cc.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/app.dir/src/app.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/src/app.cc.o -MF CMakeFiles/app.dir/src/app.cc.o.d -o CMakeFiles/app.dir/src/app.cc.o -c /home/marc/my_project/1126_mp4/src/app.cc

CMakeFiles/app.dir/src/app.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/src/app.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/app.cc > CMakeFiles/app.dir/src/app.cc.i

CMakeFiles/app.dir/src/app.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/src/app.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/app.cc -o CMakeFiles/app.dir/src/app.cc.s

CMakeFiles/app.dir/src/sample_common_isp.cc.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/src/sample_common_isp.cc.o: /home/marc/my_project/1126_mp4/src/sample_common_isp.cc
CMakeFiles/app.dir/src/sample_common_isp.cc.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/app.dir/src/sample_common_isp.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/src/sample_common_isp.cc.o -MF CMakeFiles/app.dir/src/sample_common_isp.cc.o.d -o CMakeFiles/app.dir/src/sample_common_isp.cc.o -c /home/marc/my_project/1126_mp4/src/sample_common_isp.cc

CMakeFiles/app.dir/src/sample_common_isp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/src/sample_common_isp.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/sample_common_isp.cc > CMakeFiles/app.dir/src/sample_common_isp.cc.i

CMakeFiles/app.dir/src/sample_common_isp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/src/sample_common_isp.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/sample_common_isp.cc -o CMakeFiles/app.dir/src/sample_common_isp.cc.s

CMakeFiles/app.dir/src/video.cc.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/src/video.cc.o: /home/marc/my_project/1126_mp4/src/video.cc
CMakeFiles/app.dir/src/video.cc.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/app.dir/src/video.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/src/video.cc.o -MF CMakeFiles/app.dir/src/video.cc.o.d -o CMakeFiles/app.dir/src/video.cc.o -c /home/marc/my_project/1126_mp4/src/video.cc

CMakeFiles/app.dir/src/video.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/src/video.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/video.cc > CMakeFiles/app.dir/src/video.cc.i

CMakeFiles/app.dir/src/video.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/src/video.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/video.cc -o CMakeFiles/app.dir/src/video.cc.s

CMakeFiles/app.dir/src/audio.cc.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/src/audio.cc.o: /home/marc/my_project/1126_mp4/src/audio.cc
CMakeFiles/app.dir/src/audio.cc.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/app.dir/src/audio.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/src/audio.cc.o -MF CMakeFiles/app.dir/src/audio.cc.o.d -o CMakeFiles/app.dir/src/audio.cc.o -c /home/marc/my_project/1126_mp4/src/audio.cc

CMakeFiles/app.dir/src/audio.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/src/audio.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/audio.cc > CMakeFiles/app.dir/src/audio.cc.i

CMakeFiles/app.dir/src/audio.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/src/audio.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/audio.cc -o CMakeFiles/app.dir/src/audio.cc.s

CMakeFiles/app.dir/src/mp4_codec_process.cc.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/src/mp4_codec_process.cc.o: /home/marc/my_project/1126_mp4/src/mp4_codec_process.cc
CMakeFiles/app.dir/src/mp4_codec_process.cc.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/app.dir/src/mp4_codec_process.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/src/mp4_codec_process.cc.o -MF CMakeFiles/app.dir/src/mp4_codec_process.cc.o.d -o CMakeFiles/app.dir/src/mp4_codec_process.cc.o -c /home/marc/my_project/1126_mp4/src/mp4_codec_process.cc

CMakeFiles/app.dir/src/mp4_codec_process.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/src/mp4_codec_process.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/mp4_codec_process.cc > CMakeFiles/app.dir/src/mp4_codec_process.cc.i

CMakeFiles/app.dir/src/mp4_codec_process.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/src/mp4_codec_process.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/mp4_codec_process.cc -o CMakeFiles/app.dir/src/mp4_codec_process.cc.s

CMakeFiles/app.dir/src/utils.cc.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/src/utils.cc.o: /home/marc/my_project/1126_mp4/src/utils.cc
CMakeFiles/app.dir/src/utils.cc.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/app.dir/src/utils.cc.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/src/utils.cc.o -MF CMakeFiles/app.dir/src/utils.cc.o.d -o CMakeFiles/app.dir/src/utils.cc.o -c /home/marc/my_project/1126_mp4/src/utils.cc

CMakeFiles/app.dir/src/utils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/src/utils.cc.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/utils.cc > CMakeFiles/app.dir/src/utils.cc.i

CMakeFiles/app.dir/src/utils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/src/utils.cc.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/utils.cc -o CMakeFiles/app.dir/src/utils.cc.s

CMakeFiles/app.dir/src/imu_test.cpp.o: CMakeFiles/app.dir/flags.make
CMakeFiles/app.dir/src/imu_test.cpp.o: /home/marc/my_project/1126_mp4/src/imu_test.cpp
CMakeFiles/app.dir/src/imu_test.cpp.o: CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/app.dir/src/imu_test.cpp.o"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/app.dir/src/imu_test.cpp.o -MF CMakeFiles/app.dir/src/imu_test.cpp.o.d -o CMakeFiles/app.dir/src/imu_test.cpp.o -c /home/marc/my_project/1126_mp4/src/imu_test.cpp

CMakeFiles/app.dir/src/imu_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/app.dir/src/imu_test.cpp.i"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marc/my_project/1126_mp4/src/imu_test.cpp > CMakeFiles/app.dir/src/imu_test.cpp.i

CMakeFiles/app.dir/src/imu_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/app.dir/src/imu_test.cpp.s"
	/opt/atk-dlrv1126-toolchain/usr/bin/arm-linux-gnueabihf-g++ --sysroot=/opt/atk-dlrv1126-toolchain/arm-buildroot-linux-gnueabihf/sysroot $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marc/my_project/1126_mp4/src/imu_test.cpp -o CMakeFiles/app.dir/src/imu_test.cpp.s

# Object files for target app
app_OBJECTS = \
"CMakeFiles/app.dir/src/app.cc.o" \
"CMakeFiles/app.dir/src/sample_common_isp.cc.o" \
"CMakeFiles/app.dir/src/video.cc.o" \
"CMakeFiles/app.dir/src/audio.cc.o" \
"CMakeFiles/app.dir/src/mp4_codec_process.cc.o" \
"CMakeFiles/app.dir/src/utils.cc.o" \
"CMakeFiles/app.dir/src/imu_test.cpp.o"

# External object files for target app
app_EXTERNAL_OBJECTS =

app: CMakeFiles/app.dir/src/app.cc.o
app: CMakeFiles/app.dir/src/sample_common_isp.cc.o
app: CMakeFiles/app.dir/src/video.cc.o
app: CMakeFiles/app.dir/src/audio.cc.o
app: CMakeFiles/app.dir/src/mp4_codec_process.cc.o
app: CMakeFiles/app.dir/src/utils.cc.o
app: CMakeFiles/app.dir/src/imu_test.cpp.o
app: CMakeFiles/app.dir/build.make
app: /home/marc/my_project/1126_mp4/lib/libthird_media.so
app: CMakeFiles/app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/marc/my_project/1126_mp4/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/app.dir/build: app
.PHONY : CMakeFiles/app.dir/build

CMakeFiles/app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/app.dir/clean

CMakeFiles/app.dir/depend:
	cd /home/marc/my_project/1126_mp4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marc/my_project/1126_mp4 /home/marc/my_project/1126_mp4 /home/marc/my_project/1126_mp4/build /home/marc/my_project/1126_mp4/build /home/marc/my_project/1126_mp4/build/CMakeFiles/app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/app.dir/depend

