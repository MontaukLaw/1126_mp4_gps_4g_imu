cmake_minimum_required(VERSION 3.8)
message(STATUS "cmake version ${CMAKE_VERSION}")
project("1126_mp4")

set(TOOLCHAIN_DIR /opt/atk-dlrv1126-toolchain)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/usr/bin/arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/usr/bin/arm-linux-gnueabihf-gcc)
set(SYSROOT ${TOOLCHAIN_DIR}/arm-buildroot-linux-gnueabihf/sysroot/usr/include)
set(CMAKE_SYSROOT ${TOOLCHAIN_DIR}/arm-buildroot-linux-gnueabihf/sysroot)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-g -O0 -ggdb -gdwarf -funwind-tables -rdynamic)
add_definitions(-Wno-write-strings -Wno-return-type)

set(RKNN_LIBS easymedia rga rkaiq rknn_api sample_common_isp)
set(MP4_LIBS mp4v2)
link_directories(include lib)

include_directories(${SYSROOT})
include_directories(${SYSROOT}/rga)
include_directories(${SYSROOT}/easymedia)
include_directories(${SYSROOT}/rkaiq/uAPI)
include_directories(${SYSROOT}/rkaiq/xcore)
include_directories(${SYSROOT}/rkaiq/algos)
include_directories(${SYSROOT}/rkaiq/common)
include_directories(${SYSROOT}/rkaiq/iq_parser)
include_directories(${SYSROOT}/rknn)
include_directories(.)
include_directories(./include)
include_directories(./include/3rdparty)
include_directories(./include/3rdparty/drm/include/libdrm)
include_directories(./include/3rdparty/rga/include)

add_definitions(-DRKAIQ)

# add_executable(1126_mp4 src/main.cc src/sample_common_isp.cc)
# target_link_libraries(1126_mp4 ${OPENCV_LIBS} ${RKNN_LIBS} pthread dl)
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(THIRD_MEDIA ${CMAKE_CURRENT_LIST_DIR}/lib/libthird_media.so)

# audio part
add_executable(ai_test wasted/ai_test.cc)
target_link_libraries(ai_test ${OPENCV_LIBS} ${RKNN_LIBS} pthread dl ${THIRD_MEDIA})
target_include_directories(ai_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

# app
add_executable(app src/app.cc 
                   src/sample_common_isp.cc 
                   src/video.cc 
                   src/video.cc 
                   src/audio.cc 
                   src/mp4_codec_process.cc
                   src/utils.cc
                   src/imu_test.cpp
                   src/vi_rga_venc.cc
                   src/uploader.cc
                   src/simple_ftp_uploader.cc)

target_link_libraries(app ${RKNN_LIBS} pthread dl ${THIRD_MEDIA} ${MP4_LIBS})
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

# app
# add_executable(imu src/imu_test.cpp)

set(OPENCV_LIBS opencv_core opencv_imgcodecs opencv_imgproc opencv_features2d opencv_flann opencv_highgui opencv_freetype)
# opencv
add_executable(opencv_demo src/cv_demo.cc)
include_directories(opencv_demo ${CMAKE_SOURCE_DIR}/opencv/include)
target_link_libraries(opencv_demo ${OPENCV_LIBS})