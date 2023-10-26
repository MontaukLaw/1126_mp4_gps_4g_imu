
#include "app_comm.h"

// 文件数量
extern int g_file_number;
extern FILE *g_output_file;
// 每个文件的时长
extern int g_seconds_per_file;
extern bool quit;
extern pthread_mutex_t mp4_mutex;

static MPP_CHN_S stSrcChn;
static MPP_CHN_S stDestChn;
static RK_S32 s32CamId = 0;
static RK_CHAR *pIqfilesPath = "/oem/etc/iqfiles";

// venc之后的回调函数
void video_packet_cb(MEDIA_BUFFER mb)
{
    static RK_S32 packet_cnt = 0;

    if (quit)
    {
        return;
    }
    // printf("#Get packet-%d, size %zu\n", packet_cnt, RK_MPI_MB_GetSize(mb));
    pthread_mutex_lock(&mp4_mutex);

    RK_S32 frameType = RK_MPI_MB_GetFlag(mb);
    // printf("#Get packet type: %d, size %zu\n", frameType, RK_MPI_MB_GetSize(mb));

    record_mp4(mb);
    // 直接写入h264文件
    // fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), g_output_file);

    pthread_mutex_unlock(&mp4_mutex);
    RK_MPI_MB_TsNodeDump(mb);
    RK_MPI_MB_ReleaseBuffer(mb);

    packet_cnt++;
}

int vi_venc(void)
{
    RK_U32 u32Width = 1920;
    RK_U32 u32Height = 1080;

    RK_CHAR *pDeviceName = "rkispp_scale0";
    // RK_CHAR *pOutPath = "/userdata/video/test.h264";

    // g_output_file = fopen(pOutPath, "w");

    CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
    RK_CHAR *pCodecName = "H264";

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
    // g_output_file = fopen(pOutPath, "w");
    // if (!g_output_file)
    // {
    //     printf("ERROR: open file: %s fail, exit\n", pOutPath);
    //     return 0;
    // }

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
        return -1;
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
        return ret;
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
        return ret;
    }

    // 把vi设备号0,通道0, 绑定到venc通道0
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32DevId = 0;
    stDestChn.s32ChnId = 0;
    ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret)
    {
        printf("ERROR: Bind VI[0] and VENC[0] error! ret=%d\n", ret);
        return ret;
    }

    return 0;
}

int clean_job(void)
{
    int ret = 0;
    printf("%s exit!\n", __func__);
    // unbind first
    ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
    if (ret)
    {
        printf("ERROR: UnBind VI[0] and VENC[0] error! ret=%d\n", ret);
        return ret;
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
}