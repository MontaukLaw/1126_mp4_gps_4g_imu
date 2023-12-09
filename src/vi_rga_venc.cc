#include "app_comm.h"
extern bool quit;
extern RK_U32 u32ISPFps;
// H264 codec
static int
create_venc_chn(RK_U32 fpsIn, RK_U32 fpsOut, RK_U32 u32Width, RK_U32 u32Height, RK_U32 vencChn)
{

    VENC_CHN_ATTR_S venc_chn_attr;
    memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));

    int ret = 0;
    venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
    venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
    venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = fpsOut;
    venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = u32Width * u32Height;
    // frame rate: in 1/fpsIn
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = fpsIn;

    // frame rate: out 1/fpsOut.
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
    venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = fpsOut;

    venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
    venc_chn_attr.stVencAttr.u32PicWidth = u32Width;
    venc_chn_attr.stVencAttr.u32PicHeight = u32Height;
    venc_chn_attr.stVencAttr.u32VirWidth = u32Width;
    venc_chn_attr.stVencAttr.u32VirHeight = u32Height;
    venc_chn_attr.stVencAttr.u32Profile = 77; // main profile 66: baseline; 77:MP; 100:HP;

    ret = RK_MPI_VENC_CreateChn(vencChn, &venc_chn_attr);
    if (ret)
    {
        printf("ERROR: create VENC[%d] error! ret=%d\n", vencChn, ret);
        return -1;
    }

    return 0;
}

// 创建rga通道
static int create_rga(RK_S32 rgaChn, RK_U32 u32Width, RK_U32 u32Height, IMAGE_TYPE_E outImgType)
{
    RGA_ATTR_S stRgaAttr;
    memset(&stRgaAttr, 0, sizeof(stRgaAttr));
    stRgaAttr.bEnBufPool = RK_TRUE;
    stRgaAttr.u16BufPoolCnt = 1;
    stRgaAttr.stImgIn.imgType = IMAGE_TYPE_NV12;
    stRgaAttr.u16Rotaion = 0;
    stRgaAttr.stImgIn.u32X = 0;
    stRgaAttr.stImgIn.u32Y = 0;

    stRgaAttr.stImgIn.u32Width = u32Width; // 注意这里是输出的宽高
    stRgaAttr.stImgIn.u32Height = u32Height;
    stRgaAttr.stImgIn.u32HorStride = u32Width; // 所谓虚高
    stRgaAttr.stImgIn.u32VirStride = u32Height;

    stRgaAttr.stImgOut.u32X = 0; // 输出的起始位置
    stRgaAttr.stImgOut.u32Y = 0;
    stRgaAttr.stImgOut.imgType = outImgType; // IMAGE_TYPE_NV12; // 输出的格式
    stRgaAttr.stImgOut.u32Width = u32Width;  // 输出的宽高
    stRgaAttr.stImgOut.u32Height = u32Height;
    stRgaAttr.stImgOut.u32HorStride = u32Width; // 输出的虚高;
    stRgaAttr.stImgOut.u32VirStride = u32Height;
    RK_S32 ret = RK_MPI_RGA_CreateChn(rgaChn, &stRgaAttr); // 创建rga通道

    if (ret)
    {
        printf("ERROR: RGA Set Attr: ImgIn:<%u,%u,%u,%u> "
               "ImgOut:<%u,%u,%u,%u>, rotation=%u failed! ret=%d\n",
               stRgaAttr.stImgIn.u32X, stRgaAttr.stImgIn.u32Y,
               stRgaAttr.stImgIn.u32Width, stRgaAttr.stImgIn.u32Height,
               stRgaAttr.stImgOut.u32X, stRgaAttr.stImgOut.u32Y,
               stRgaAttr.stImgOut.u32Width, stRgaAttr.stImgOut.u32Height,
               stRgaAttr.u16Rotaion, ret);
        return -1;
    }
    return 0;
}

// 将vi绑定到vga
int bind_vi_rga(RK_S32 cameraId, RK_U32 viChnId, RK_U32 rgaChnId)
{

    RK_S32 ret = 0;

    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_VI;
    stSrcChn.s32DevId = cameraId;
    stSrcChn.s32ChnId = viChnId;

    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_RGA;
    stDestChn.s32ChnId = rgaChnId;

    ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret != 0)
    {
        printf("ERROR: Bind vi[%d] and rga[%d] failed! ret=%d\n", viChnId, rgaChnId, ret);
        return -1;
    }

    return 0;
}

int bind_rga_to_venc(RK_S32 rgaChanId, RK_S32 vencChanId)
{
    int ret = 0;
    MPP_CHN_S stSrcChn;
    stSrcChn.enModId = RK_ID_RGA;
    stSrcChn.s32ChnId = rgaChanId;

    MPP_CHN_S stDestChn;
    stDestChn.enModId = RK_ID_VENC;
    stDestChn.s32ChnId = vencChanId;

    ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
    if (ret != 0)
    {
        printf("ERROR: Bind rga[%d] and venc[%d] failed! ret=%d\n", 0, 0, ret);
        return -1;
    }
}

int bind_rgn_venc(void)
{
    int ret = 0;
    int s32CamId = 0;

    // 创建rga通道，通道号为1
    ret = create_rga(VENC_RGN_CHN, VIDEO_WIDTH, VIDEO_HEIGHT, IMAGE_TYPE_NV12);
    if (ret < 0)
    {
        printf("create_rga_chn failed ret:%d\n", ret);
        return -1;
    }

    // 创建rga通道，通道号为1
    ret = create_rga(CV_RGN_CHN, VIDEO_WIDTH, VIDEO_HEIGHT, IMAGE_TYPE_RGB888);
    if (ret < 0)
    {
        printf("create_rga_chn failed ret:%d\n", ret);
        return -1;
    }

    // 把rga chn 0绑定到vi chn0
    ret = bind_vi_rga(s32CamId, 0, VENC_RGN_CHN);
    if (ret < 0)
    {
        printf("ERROR: bind vi 0 to rga 0 failed\n");
        return -1;
    }

    // 把rga chn 1绑定到vi chn0
    ret = bind_vi_rga(s32CamId, 0, CV_RGN_CHN);
    if (ret < 0)
    {
        printf("ERROR: bind vi 0 to rga 1 failed\n");
        return -1;
    }

    // 创建venc通道，通道号为0
    // 30fps, 1920x1080
    ret = create_venc_chn(u32ISPFps, u32ISPFps, VIDEO_WIDTH, VIDEO_HEIGHT, 0);
    if (ret < 0)
    {
        printf("ERROR: create_venc_chn error%d\n", 0);
        return -1;
    }

    MPP_CHN_S stEncChn;
    stEncChn.enModId = RK_ID_VENC;
    stEncChn.s32DevId = 0;
    stEncChn.s32ChnId = 0;
    ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, venc_video_packet_cb);
    if (ret)
    {
        printf("ERROR: register output callback for VENC[0] error! ret=%d\n", ret);
        return ret;
    }

    // 把rga0绑定到venc0
    bind_rga_to_venc(VENC_RGN_CHN, 0);
}

void print_mb_info(MEDIA_BUFFER buffer)
{
    int cnt = 0;
    printf("#%d Get Frame:ptr:%p, size:%zu, mode:%d, channel:%d, timestamp:%lld\n",
           cnt++, RK_MPI_MB_GetPtr(buffer), RK_MPI_MB_GetSize(buffer),
           RK_MPI_MB_GetModeID(buffer), RK_MPI_MB_GetChannelID(buffer),
           RK_MPI_MB_GetTimestamp(buffer));
}

static void *cv_thread(void *args)
{

    int ret = 0;
    int counter = 0;

    printf("Start get_media_buffer thread, \n");

    MEDIA_BUFFER buffer = NULL;

    // get data from vi
    while (quit == 0)
    {
        buffer = RK_MPI_SYS_GetMediaBuffer(RK_ID_RGA, CV_RGN_CHN, -1);
        if (!buffer)
        {
            usleep(1000);
            continue;
        }
        // print_mb_info(buffer);
        counter++;
        if (counter == 60)
        {
            // 在这里写入一个测试用的文件.
            FILE *fp = fopen("/userdata/raw.rgb", "w");
            if (fp == NULL)
            {
                printf("open file failed\n");
                continue;
            }
            printf("write test file >>>>>>>>>>>>>>>>>>>>>>>>>\n");
            fwrite(RK_MPI_MB_GetPtr(buffer), 1, RK_MPI_MB_GetSize(buffer), fp);
        }

        RK_MPI_MB_ReleaseBuffer(buffer);
    }
}

static pthread_t cv_thread_id;
void create_cv_thread(void)
{
    // cv线程
    pthread_create(&cv_thread_id, NULL, cv_thread, NULL);
}