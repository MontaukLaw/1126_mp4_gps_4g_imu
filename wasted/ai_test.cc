// Copyright 2020 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <assert.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "rkmedia_api.h"

typedef struct AudioParams_
{
    RK_U32 u32SampleRate;
    RK_U32 u32ChnCnt;
    SAMPLE_FORMAT_E enSampleFmt;
    const RK_CHAR *pOutPath;
} AudioParams;

static bool quit = false;
static void sigterm_handler(int sig)
{
    fprintf(stderr, "signal %d\n", sig);
    quit = true;
}
static CODEC_TYPE_E code_type = RK_CODEC_TYPE_MP2;

#define MP3_PROFILE_LOW 1

typedef struct FreqIdx_
{
    RK_S32 u32SmpRate;
    RK_U8 u8FreqIdx;
} FreqIdx;

FreqIdx FreqIdxTbl[13] = {{96000, 0}, {88200, 1}, {64000, 2}, {48000, 3}, {44100, 4}, {32000, 5}, {24000, 6}, {22050, 7}, {16000, 8}, {12000, 9}, {11025, 10}, {8000, 11}, {7350, 12}};

static void GetHeader(RK_U8 *pu8Hdr, RK_S32 u32SmpRate, RK_U8 u8Channel,
                      RK_U32 u32DataLen)
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

static void *GetAudioMediaBuffer(void *params)
{
    AudioParams *pstAudioParams = (AudioParams *)params;
    RK_U8 header[7];
    MEDIA_BUFFER mb = NULL;
    int cnt = 0;
    FILE *save_file = NULL;

    printf("#Start %s thread, SampleRate:%u, Channel:%u, Fmt:%x, Path:%s\n",
           __func__, pstAudioParams->u32SampleRate, pstAudioParams->u32ChnCnt,
           pstAudioParams->enSampleFmt, pstAudioParams->pOutPath);

    if (pstAudioParams->pOutPath)
    {
        save_file = fopen(pstAudioParams->pOutPath, "w");
        if (!save_file)
            printf("ERROR: Open %s failed!\n", pstAudioParams->pOutPath);
    }

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

        if (save_file)
        {
            if (code_type == RK_CODEC_TYPE_MP3)
            {
                GetHeader(header, pstAudioParams->u32SampleRate,
                          pstAudioParams->u32ChnCnt, RK_MPI_MB_GetSize(mb));
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

int main(int argc, char *argv[])
{
    RK_U32 u32SampleRate = 16000;
    RK_U32 u32BitRate = 64000; // 64kbps
    RK_U32 u32ChnCnt = 2;
    RK_U32 u32FrameCnt = 1152;
    SAMPLE_FORMAT_E enSampleFmt = RK_SAMPLE_FMT_S16;
    // default:CARD=rockchiprk809co
    RK_CHAR *pDeviceName = "default";
    RK_CHAR *pOutPath = "aenc.mp3";
    
    int ret = 0;
    int c;

    printf("#Device: %s\n", pDeviceName);
    printf("#SampleRate: %d\n", u32SampleRate);
    printf("#Channel Count: %d\n", u32ChnCnt);
    printf("#Frame Count: %d\n", u32FrameCnt);
    printf("#BitRate: %d\n", u32BitRate);
    printf("#SampleFmt: %d\n", enSampleFmt);
    printf("#Output Path: %s\n", pOutPath);
    printf("#code_type: %d\n", code_type);

    AudioParams stAudioParams;
    stAudioParams.u32SampleRate = u32SampleRate;
    stAudioParams.u32ChnCnt = u32ChnCnt;
    stAudioParams.enSampleFmt = enSampleFmt;
    stAudioParams.pOutPath = pOutPath;

    RK_MPI_SYS_Init();

    MPP_CHN_S mpp_chn_ai, mpp_chn_aenc;
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
        return -1;
    }

    printf("%s initial finish\n", __func__);
    signal(SIGINT, sigterm_handler);
    while (!quit)
    {
        usleep(500000);
    }

    RK_MPI_SYS_UnBind(&mpp_chn_ai, &mpp_chn_aenc);
    RK_MPI_AI_DisableChn(mpp_chn_ai.s32ChnId);
    RK_MPI_AENC_DestroyChn(mpp_chn_aenc.s32ChnId);

    return 0;
}
