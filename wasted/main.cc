
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

#include "sample_common.h"
#include "rkmedia_api.h"
#include "rkmedia_venc.h"

static bool quit = false;
static FILE *g_output_video_file;

static RK_S32 g_s32FrameCnt = -1;

// 信号量
static void sigterm_handler(int sig)
{
    fprintf(stderr, "signal %d\n", sig);
    quit = true;
}

// venc之后的回调函数
void video_packet_cb(MEDIA_BUFFER mb)
{
    static RK_S32 packet_cnt = 0;
    if (quit)
        return;

    const char *nalu_type = "Image data";
    switch (RK_MPI_MB_GetFlag(mb))
    {
    case VENC_NALU_IDRSLICE:
        nalu_type = "IDR Slice";
        break;
    case VENC_NALU_PSLICE:
        nalu_type = "P Slice";
        break;
    default:
        break;
    }

    if (g_output_video_file)
    {
        fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), g_output_video_file);
        printf("#Write packet-%d, %s, size %zu\n", packet_cnt, nalu_type, RK_MPI_MB_GetSize(mb));
    }
    else
    {
        printf("#Get packet-%d, %s, size %zu\n", packet_cnt, nalu_type, RK_MPI_MB_GetSize(mb));
    }
    RK_MPI_MB_TsNodeDump(mb);
    RK_MPI_MB_ReleaseBuffer(mb);

    packet_cnt++;
    if ((g_s32FrameCnt >= 0) && (packet_cnt > g_s32FrameCnt))
        quit = true;
}

// 主程序入口
int main(int argc, char *argv[])
{
    RK_U32 u32Width = 1920;
    RK_U32 u32Height = 1080;
    RK_CHAR *pDeviceName = "rkispp_scale0";
    RK_CHAR *pOutPath = "test.h264";
    RK_CHAR *pIqfilesPath = "/oem/etc/iqfiles";
    CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
    RK_CHAR *pCodecName = "H264";
    RK_S32 s32CamId = 0;
    RK_U32 u32BufCnt = 3;
    RK_BOOL bMultictx = RK_FALSE;
    RK_U32 u32Fps = 30;
    rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

    int ret = 0;

    // 初始化ISP
    SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, pIqfilesPath);
    SAMPLE_COMM_ISP_Run(s32CamId);
    SAMPLE_COMM_ISP_SetFrameRate(s32CamId, u32Fps);

    // 获取文件写入句柄
    if (pOutPath)
    {
        g_output_video_file = fopen(pOutPath, "w");
        if (!g_output_video_file)
        {
            printf("ERROR: open file: %s fail, exit\n", pOutPath);
            return 0;
        }
    }
    RK_MPI_SYS_Init();

    // 简单初始化vi
    VI_CHN_ATTR_S vi_chn_attr;
    vi_chn_attr.pcVideoNode = pDeviceName;
    vi_chn_attr.u32BufCnt = u32BufCnt;
    vi_chn_attr.u32Width = u32Width;
    vi_chn_attr.u32Height = u32Height;
    vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
    vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
    vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
    ret = RK_MPI_VI_SetChnAttr(s32CamId, 0, &vi_chn_attr);
    ret |= RK_MPI_VI_EnableChn(s32CamId, 0);
    if (ret)
    {
        printf("ERROR: create VI[0] error! ret=%d\n", ret);
        return 0;
    }

    // 简单初始化venc
    VENC_CHN_ATTR_S venc_chn_attr;
    memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
    venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
    venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
    venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = 30;
    venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = u32Width * u32Height;
    // frame rate: in 30/1, out 30/1.
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = 30;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = 30;
    venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
    venc_chn_attr.stVencAttr.u32PicWidth = u32Width;
    venc_chn_attr.stVencAttr.u32PicHeight = u32Height;
    venc_chn_attr.stVencAttr.u32VirWidth = u32Width;
    venc_chn_attr.stVencAttr.u32VirHeight = u32Height;
    venc_chn_attr.stVencAttr.u32Profile = 77;

    // 创建venc通道
    ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);
    if (ret)
    {
        printf("ERROR: create VENC[0] error! ret=%d\n", ret);
        return 0;
    }

    // 注册编码后的回调
    MPP_CHN_S stEncChn;
    stEncChn.enModId = RK_ID_VENC;
    stEncChn.s32DevId = 0;
    stEncChn.s32ChnId = 0;
    ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, video_packet_cb);
    if (ret)
    {
        printf("ERROR: register output callback for VENC[0] error! ret=%d\n", ret);
        return 0;
    }

    // 把vi设备号0,通道0, 绑定到venc通道0
    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;
    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = 0;
    ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret)
    {
        printf("ERROR: Bind VI[0] and VENC[0] error! ret=%d\n", ret);
        return 0;
    }
    // 设置停止信号量
    printf("%s initial finish\n", __func__);
    signal(SIGINT, sigterm_handler);

    while (!quit)
    {
        usleep(500000);
    }

    if (g_output_video_file)
    {
        fclose(g_output_video_file);
    }

    // 扫尾工作

    printf("%s exit!\n", __func__);
    // unbind first
    ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret)
    {
        printf("ERROR: UnBind VI[0] and VENC[0] error! ret=%d\n", ret);
        return 0;
    }
    // destroy venc before vi
    ret = RK_MPI_VENC_DestroyChn(0);
    if (ret)
    {
        printf("ERROR: Destroy VENC[0] error! ret=%d\n", ret);
        return 0;
    }
    // destroy vi
    ret = RK_MPI_VI_DisableChn(s32CamId, 0);
    if (ret)
    {
        printf("ERROR: Destroy VI[0] error! ret=%d\n", ret);
        return 0;
    }

    if (pIqfilesPath)
    {
        SAMPLE_COMM_ISP_Stop(s32CamId);
    }

    return 0;
}
