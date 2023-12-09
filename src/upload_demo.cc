#include "simple_ftp_uploader.h"
#include "app_comm.h"

char h264FileName[128];
char *h264FileNameStr = "/app/1209/20231209150052.h264";
bool ifUpload = true;
bool quit = false;
extern bool ifUploaderFileReady;
int main(void)
{

    memcpy(h264FileName, h264FileNameStr, strlen(h264FileNameStr) + 1);
    create_uploader_process();
    while (1)
    {
        ifUploaderFileReady = true;
        sleep(10);
    }
}