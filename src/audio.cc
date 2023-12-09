#include "app_comm.h"

#define AUDIO_TIME_SCALE 44100
#define HOW_MANY_US_IN_ONE_SEC 1000000
#define MP3_PROFILE_LOW 1

MP4TrackId audioTrack = MP4_INVALID_TRACK_ID;
extern MP4FileHandle hMP4File;
extern bool quit;
extern int recordingVideo;
extern pthread_mutex_t mp4_mutex;

extern FILE *g_output_audio_file;

static MPP_CHN_S mpp_chn_ai, mpp_chn_aenc;
static CODEC_TYPE_E code_type = RK_CODEC_TYPE_MP2; // RK_CODEC_TYPE_MP2;

typedef struct AudioParams_
{
    RK_U32 u32SampleRate;
    RK_U32 u32ChnCnt;
    SAMPLE_FORMAT_E enSampleFmt;
    const RK_CHAR *pOutPath;
} AudioParams;

typedef struct FreqIdx_
{
    RK_S32 u32SmpRate;
    RK_U8 u8FreqIdx;
} FreqIdx;

FreqIdx FreqIdxTbl[13] = {{96000, 0}, {88200, 1}, {64000, 2}, {48000, 3}, {44100, 4}, {32000, 5}, {24000, 6}, {22050, 7}, {16000, 8}, {12000, 9}, {11025, 10}, {8000, 11}, {7350, 12}};

void create_audio_file(void)
{
    char filename[128] = {0};
    get_file_name_by_date_time(filename, "%s%lld.mp3");
    g_output_audio_file = fopen(filename, "wb");
}

static void GetHeader(RK_U8 *pu8Hdr, RK_S32 u32SmpRate, RK_U8 u8Channel, RK_U32 u32DataLen)
{
    RK_U8 u8FreqIdx = 0;
    for (int i = 0; i < 13; i++)
    {
        if (u32SmpRate == FreqIdxTbl[i].u32SmpRate)
        {
            u8FreqIdx = FreqIdxTbl[i].u8FreqIdx;
            break;
        }
    }

    RK_U32 u32PacketLen = u32DataLen + 7;
    pu8Hdr[0] = 0xFF;
    pu8Hdr[1] = 0xF1;
    pu8Hdr[2] = ((MP3_PROFILE_LOW) << 6) + (u8FreqIdx << 2) + (u8Channel >> 2);
    pu8Hdr[3] = (((u8Channel & 3) << 6) + (u32PacketLen >> 11));
    pu8Hdr[4] = ((u32PacketLen & 0x7FF) >> 3);
    pu8Hdr[5] = (((u32PacketLen & 7) << 5) + 0x1F);
    pu8Hdr[6] = 0xFC;
}

static int64_t getCurrentTimeUs(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}


static void *GetAudioMediaBuffer(void *params)
{
    AudioParams *pstAudioParams = (AudioParams *)params;
    RK_U8 header[7];
    MEDIA_BUFFER mb = NULL;
    int cnt = 0;
    // FILE *save_file = NULL;

    // printf("#Start %s thread, SampleRate:%u, Channel:%u, Fmt:%x, Path:%s\n",
    //        __func__, pstAudioParams->u32SampleRate, pstAudioParams->u32ChnCnt,
    //        pstAudioParams->enSampleFmt, pstAudioParams->pOutPath);

    while (!quit) 
    {
        mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_AENC, 0, -1);
        if (!mb)
        {
            printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
            break;
        }

#if 1
        printf("#%d Get Frame:ptr:%p, size:%zu, mode:%d, channel:%d, "
               "timestamp:%lld\n",
               cnt++, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
               RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb),
               RK_MPI_MB_GetTimestamp(mb));
#endif

        if (g_output_audio_file)
        {
            if (code_type == RK_CODEC_TYPE_MP3)
            {
                GetHeader(header, pstAudioParams->u32SampleRate,
                          pstAudioParams->u32ChnCnt, RK_MPI_MB_GetSize(mb));
                fwrite(header, 1, 7, g_output_audio_file);
            }
            fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), g_output_audio_file);
        }
        RK_MPI_MB_ReleaseBuffer(mb);
    }

    if (g_output_audio_file)
        fclose(g_output_audio_file);

    return NULL;
}

static void *GetAudioMediaBuffer__(void *params)
{
    AudioParams *pstAudioParams = (AudioParams *)params;
    RK_U8 header[7];
    MEDIA_BUFFER mb = NULL;
    int cnt = 0;
    FILE *save_file = NULL;
    int64_t temp_us = 0;
    static int64_t elapse_us = 0;

    // printf("#Start %s thread, SampleRate:%u, Channel:%u, Fmt:%x, Path:%s\n",
    //        __func__, pstAudioParams->u32SampleRate, pstAudioParams->u32ChnCnt,
    //        pstAudioParams->enSampleFmt, pstAudioParams->pOutPath);

    // if (pstAudioParams->pOutPath)
    // {
    //     save_file = fopen(pstAudioParams->pOutPath, "w");
    //     if (!save_file)
    //         printf("ERROR: Open %s failed!\n", pstAudioParams->pOutPath);
    // }

    while (!quit)
    {
        mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_AENC, 0, -1);
        if (!mb)
        {
            printf("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
            break;
        }
        
#if DEBUG_INFO

        printf("#%d Get Frame:ptr:%p, size:%zu, mode:%d, channel:%d, "
               "timestamp:%lld\n",
               cnt++, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
               RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb),
               RK_MPI_MB_GetTimestamp(mb));
#endif
        // if (recordingVideo)
        if (0)
        {
            temp_us = getCurrentTimeUs();

            pthread_mutex_lock(&mp4_mutex);

            int64_t time_gap = temp_us - elapse_us;

            MP4WriteSample(hMP4File, audioTrack, (const uint8_t *)RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb), MP4_INVALID_DURATION, 0, 1);

            // printf("saving mp4 sound track spend: %lld us \n", getCurrentTimeUs() - temp_us);
            elapse_us = getCurrentTimeUs();
            pthread_mutex_unlock(&mp4_mutex);
        }

        if (g_output_audio_file)
        {
            printf("#%d Get Audio Frame:ptr:%p, size:%zu, mode:%d, channel:%d, "
                   "timestamp:%lld\n",
                   cnt++, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
                   RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb),
                   RK_MPI_MB_GetTimestamp(mb));

            if (code_type == RK_CODEC_TYPE_MP3)
            {
                GetHeader(header, pstAudioParams->u32SampleRate, pstAudioParams->u32ChnCnt, RK_MPI_MB_GetSize(mb));
                fwrite(header, 1, 7, save_file);
            }
            fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), save_file);
        }

        RK_MPI_MB_ReleaseBuffer(mb);
    }

    if (save_file)
        fclose(save_file);

    return NULL;
}

int init_audio(void)
{

    RK_U32 u32SampleRate = 16000;
    RK_U32 u32BitRate = 64000; // 64kbps
    RK_U32 u32ChnCnt = 2;
    RK_U32 u32FrameCnt = 1152;
    SAMPLE_FORMAT_E enSampleFmt = RK_SAMPLE_FMT_S16;
    // default:CARD=rockchiprk809co
    RK_CHAR *pDeviceName = "default";
    RK_CHAR *pOutPath = "aenc1209.mp3";
    int ret = 0;

    printf("# Device: %s\n", pDeviceName);
    printf("# SampleRate: %d\n", u32SampleRate);
    printf("# Channel Count: %d\n", u32ChnCnt);
    printf("# Frame Count: %d\n", u32FrameCnt);
    printf("# BitRate: %d\n", u32BitRate);
    printf("# SampleFmt: %d\n", enSampleFmt);
    printf("# Output Path: %s\n", pOutPath);
    printf("# code_type: %d\n", code_type);

    AudioParams stAudioParams;
    stAudioParams.u32SampleRate = u32SampleRate;
    stAudioParams.u32ChnCnt = u32ChnCnt;
    stAudioParams.enSampleFmt = enSampleFmt;
    stAudioParams.pOutPath = pOutPath;

    RK_MPI_SYS_Init();

    mpp_chn_ai.enModId = RK_ID_AI;
    mpp_chn_ai.s32ChnId = 0;
    mpp_chn_aenc.enModId = RK_ID_AENC;
    mpp_chn_aenc.s32ChnId = 0;

    // 1. create AI
    AI_CHN_ATTR_S ai_attr;
    ai_attr.pcAudioNode = pDeviceName;
    ai_attr.enSampleFormat = enSampleFmt;
    ai_attr.u32NbSamples = u32FrameCnt;
    ai_attr.u32SampleRate = u32SampleRate;
    ai_attr.u32Channels = u32ChnCnt;
    ai_attr.enAiLayout = AI_LAYOUT_NORMAL;
    ret = RK_MPI_AI_SetChnAttr(mpp_chn_ai.s32ChnId, &ai_attr);
    ret |= RK_MPI_AI_EnableChn(mpp_chn_ai.s32ChnId);
    if (ret)
    {
        printf("Create AI[0] failed! ret=%d\n", ret);
        return -1;
    }

    // 2. create AENC
    AENC_CHN_ATTR_S aenc_attr;
    aenc_attr.enCodecType = code_type;
    aenc_attr.u32Bitrate = u32BitRate;
    aenc_attr.u32Quality = 1;
    aenc_attr.stAencMP3.u32Channels = u32ChnCnt;
    aenc_attr.stAencMP3.u32SampleRate = u32SampleRate;
    ret = RK_MPI_AENC_CreateChn(mpp_chn_aenc.s32ChnId, &aenc_attr);
    if (ret)
    {
        printf("Create AENC[0] failed! ret=%d\n", ret);
        return -1;
    }

    pthread_t read_thread;
    pthread_create(&read_thread, NULL, GetAudioMediaBuffer, &stAudioParams);

    // 3. bind AI-AENC
    ret = RK_MPI_SYS_Bind(&mpp_chn_ai, &mpp_chn_aenc);
    if (ret)
    {
        printf("Bind AI[0] to AENC[0] failed! ret=%d\n", ret);
        return ret;
    }

    return 0;
}

void clean_ai(void)
{

    RK_MPI_SYS_UnBind(&mpp_chn_ai, &mpp_chn_aenc);
    RK_MPI_AI_DisableChn(mpp_chn_ai.s32ChnId);
    RK_MPI_AENC_DestroyChn(mpp_chn_aenc.s32ChnId);
}

void init_audio_track(void)
{
    // const unsigned char data[2] = {0x50, 0x40};
    // const unsigned char data[2] = {0x12, 0x8};
    // const unsigned char data[2] = {0x02, 0x81};
    // 创建音频通道
    audioTrack = MP4AddAudioTrack(hMP4File, AUDIO_TIME_SCALE, 1024, MP4_MPEG4_AUDIO_TYPE);
    if (audioTrack == MP4_INVALID_TRACK_ID)
    {
        printf("add audio track failed\n");
        return;
    }
    else
    {
        printf("add audio track %d success\n", audioTrack);
    }

    MP4SetAudioProfileLevel(hMP4File, 0x2);

    // 上面的{18，16}就是pConfig了，2就是configSize。那么{18， 16}代表什么呢？
    // 首先，config有2个字节组成，共16位，具体含义如下：
    // 5 bits | 4 bits | 4 bits | 3 bits
    // 第一欄    第二欄    第三欄    第四欄
    // 第一欄：AAC Object Type
    // 第二欄：Sample Rate Index
    // 第三欄：Channel Number
    // 第四欄：Don't care，設 0

    // AAC_MAIN = 1,
    // 4 : 44100 Hz
    // 2 : 2 channels : front - left, front - right
    uint8_t config[] = {0x12, 0x10};
    // printf("GetDecoderSpecificInfo(1, 4, 2): 0x%04x\n", config);
    if (!MP4SetTrackESConfiguration(hMP4File, audioTrack, config, 2))
    {
        printf("set config failed\n");
        return;
    }
    // recordingAudio = true;
}