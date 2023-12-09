#include "app_comm.h"
#include "simple_ftp_uploader.h"

// 帧率
extern RK_U32 u32ISPFps;

bool quit = false;
FILE *g_output_video_file;
// 文件数量
int g_file_number = 1;

// 每个文件的时长
RK_U32 g_seconds_per_file = 100;

bool ifUpload = false;

FILE *g_output_audio_file;

// 信号量
static void sigterm_handler(int sig)
{
    fprintf(stderr, "signal %d\n", sig);
    quit = true;
}

RK_VOID show_usage(RK_VOID)
{
    printf("Usage:./vi_venc_mp4 -s 300 (seconds per file) -f 10 (keep file number) -p 30(fps) -upload(if you want)\n");
    // printf("Usage:./vi_venc_mp4 -fps 10(fps)\n");
}

// 程序入口
int main(int argc, char *argv[])
{

    if (argc < 7)
    {
        show_usage();
        return -1;
    }

    // 设置保存文件数量跟每个文件的时长(秒为单位)
    g_seconds_per_file = atoi(argv[2]);
    g_file_number = atoi(argv[4]);
    u32ISPFps = atoi(argv[6]);

    ifUpload = (argc == 8) ? true : false;

    // 创建h264文件
    create_new_video_file();

    // 创建mp3文件
    create_audio_file();

    printf("seconds per file:%d\n", g_seconds_per_file);
    printf("keep file number:%d\n", g_file_number);
    printf("isp fps:%d\n", u32ISPFps);

    // 启动视频部分
    // 设置停止信号量
    printf("%s initial finish\n", __func__);
    signal(SIGINT, sigterm_handler);

    // g_output_file = fopen("/userdata/111901.h264", "wb");

    // 视频部分
    vi_venc();

    // 音频部分
    init_audio();

    // 读取imu数据
    pthread_t read_imu_thread;
    // pthread_create(&read_imu_thread, NULL, imu_process, NULL);

    create_cv_thread();

    create_uploader_process();

    while (!quit)
    {
        usleep(500000);
    }

    if (g_output_video_file)
    {
        fclose(g_output_video_file);
    }

    // 扫尾工作
    clean_job();

    // 音频的扫尾工作
    // clean_ai();

    return 0;
}

// 程序入口
int main__(int argc, char *argv[])
{

    if (argc != 5)
    {
        show_usage();
        return 0;
    }

    // 设置保存文件数量跟每个文件的时长(秒为单位)
    g_seconds_per_file = atoi(argv[2]);
    g_file_number = atoi(argv[4]);

    printf("seconds per file:%d\n", g_seconds_per_file);
    printf("keep file number:%d\n", g_file_number);

    // 启动视频部分
    // 设置停止信号量
    printf("%s initial finish\n", __func__);
    signal(SIGINT, sigterm_handler);

    // 视频部分
    vi_venc();

    // 音频部分
    init_audio();

    // 读取imu数据
    pthread_t read_imu_thread;
    pthread_create(&read_imu_thread, NULL, imu_process, NULL);

    create_cv_thread();

    while (!quit)
    {
        usleep(500000);
    }

    if (g_output_video_file)
    {
        fclose(g_output_video_file);
    }

    // 扫尾工作
    clean_job();

    // 音频的扫尾工作
    clean_ai();

    return 0;
}
