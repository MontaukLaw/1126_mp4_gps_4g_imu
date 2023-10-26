#ifndef __APP_COMM_H__
#define __APP_COMM_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#include "sample_common.h"
#include "rkmedia_api.h"
#include "rkmedia_venc.h"
#include "pthread.h"
#include "mp4v2/mp4v2.h"

// #include <assert.h>
// #include <fcntl.h>
// #include <pthread.h>
// #include <signal.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include <unistd.h>
// #include "rkmedia_api.h"

// #define VIDEO_FOLDER "/sd/video/"
#define VIDEO_FOLDER "/userdata/video/"

#define PASS_FRAME_NUM 20
#define IS_FILE_NOT_FOLDER 8
#define IS_NOT_HIDEN_FILE 20
#define MP4_FILE_NAME_LEN 18
#define VIDEO_TRACK_TIME_SCALE 90000
#define VIDEO_FPS 30
#define MIPI_2_DEV_ID 1

#define SPS_FRAME_LEN 26
#define PPS_FRAME_LEN 4
#define FRAME_START_FLAG_LEN 4

    int vi_venc(void);

    int count_my_mp4_file_number(void);

    void find_oldest_file_name_prefix(char *file_name_buf);

    void get_file_name_by_time(char *file_name_buf);

    RK_S32 record_mp4(MEDIA_BUFFER mb);

    int64_t get_current_time_us(void);

    void init_audio_track(void);

    int clean_job(void);

    void clean_ai(void);

    int init_audio(void);

    void *imu_process(void *param);
    
    // void start_imu_process(void);

#ifdef __cplusplus
}
#endif

#endif