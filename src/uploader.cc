#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_ftp_uploader.h"
#include "pthread.h"
#include <unistd.h>
#include <sys/stat.h>

#define FTP_SERVER_IP "118.195.247.171"
#define FTP_SERVER_USER "huang"
#define FTP_SERVER_PASS "huang"
#define MAX_BUF_LEN 64 * 1024 * 1024

typedef struct
{
    char usr[32];
    char passwd[32];
    char ser_filepath[512];
    char ser_filename[64];
    char new_filename[64];
    int control_sock;

} ftp_client_st;

ftp_client_st ftp_st;
bool ifUploaderFileReady = false;
extern bool ifUpload;
// extern char uploadFileName[];
extern bool quit;
extern char h264FileName[];

static void ftp_upload(void);

void *file_uploader_process(void *params)
{

    while (quit == false)
    {
        if (!ifUpload)
        {
            continue;
        }

        if (ifUploaderFileReady == false)
        {
            continue;
        }

        ftp_upload();

        ifUploaderFileReady = false;

        usleep(100000);
    }

    return NULL;
}

void create_uploader_process(void)
{
    pthread_t uploader_thread;
    pthread_create(&uploader_thread, NULL, file_uploader_process, NULL);
}

// char str[1024 * 1024];
static void ftp_upload(void)
{
    // char *str = (char *)malloc(1024 * 1024);
    char str[128];
    int ret = -1;
    struct stat file_stat;
    printf("\n");
    printf("\n");

    printf("**********************************************************\n");

    // ifUploaderFileReady = true;
    if (stat(h264FileName, &file_stat) == 0)
    {
        // 文件大小以字节为单位
        printf("File size of %s: %ld bytes\n", h264FileName, file_stat.st_size);
    }
    else
    {
        perror("Error getting file size");
        return;
    }
    // printf("Please input the ftp server ip: ");
    memset(str, 0, sizeof(str));
    // scanf("%s",str); //从终端获取到服务器ip地址。
    strcpy(str, FTP_SERVER_IP);
    printf("input fpt server ip:%s\n", str);
    /*连接到服务器*/
    memset(&ftp_st, 0, sizeof(ftp_client_st));
    ftp_st.control_sock = connect_ftp_server(str, FTP_SERVER_PORT);
    if (ftp_st.control_sock <= 0)
    {
        printf("connect ftp server failed\n");
        goto err0;
    }

    strcpy(ftp_st.usr, FTP_SERVER_USER);
    strcpy(ftp_st.passwd, FTP_SERVER_PASS);
    printf("FTP Username:%s Password:%s\n", ftp_st.usr, ftp_st.passwd);
    ret = login_ftp_server(ftp_st.control_sock, ftp_st.usr, ftp_st.passwd);
    if (ret < 0)
    {
        printf("\nUser or Passwd is wrong,input agin");
        goto err0;
    }

    char remoteFileName[128];
    sprintf(remoteFileName, "/home/huang/%s", h264FileName);

    up_file_ftpserver(ftp_st.control_sock, remoteFileName, h264FileName, 1, 0);
    get_fsize_ftpserver(ftp_st.control_sock, remoteFileName);

err0:
    quit_fpt_server(ftp_st.control_sock);

    printf("**********************************************************\n");
    printf("\n");
    printf("\n");

    // free(str);
}