/*
 * Copyright (C) 2016 The Dragonboard Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "AudioRecorder"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#include "AudioRecorder.h"

using namespace dragonboard;

AudioRecorder::AudioRecorder(StreamContext streamCt)
{
    info.mStreamCt = streamCt;
    info.mRecordTimeSet = true;
    info.mRecordTimeInMsecs = streamCt.timeInMsecs;
    info.mStop = false;
    info.mRecorded = false;
    info.mState = RECSTATE_UNINIT;
    info.mContext = NULL;
    info.mPcm = NULL;

    if (info.mStreamCt.fileName) {
        mSink = SINK_FILE;
    } else {
        mSink = SINK_BUFFER;
    }
}

AudioRecorder::~AudioRecorder()
{
    exit();
}

bool AudioRecorder::init()
{
    bool ret;

    if (info.mState != RECSTATE_UNINIT) {
        ALOGV("line(%d):AudioPlayer inited", __LINE__);
        return true;
    }

    info.mState = RECSTATE_INITING;

    if (info.mStreamCt.fileName) {
        info.mContext = new AudioFileContext(info.mStreamCt.fileName,
                                             AudioFileContext::WRITE);
        if (!info.mContext) {
            ALOGE("line(%d):new AudioFileContext", __LINE__);
            info.mState = RECSTATE_UNINIT;
            return false;
        }

        ret = info.mContext->init(info.mStreamCt.config.rate,
                                  info.mStreamCt.config.channels);
    } else {
        ret = true;
    }

    info.mStop = false;
    info.mRecorded = false;
    info.mState = RECSTATE_INITED;
    return ret;
}

bool AudioRecorder::exit()
{
    bool ret = true;

    info.mStop = true;

    if (info.mContext) {
        ret = info.mContext->exit();
        delete info.mContext;
        info.mContext = NULL;
    }

    if (info.mPcm) {
        pcm_close(info.mPcm);
        info.mPcm = NULL;
    }

    info.mState = RECSTATE_UNINIT;

    return ret;
}

void AudioRecorder::setRecordTime(unsigned int ms)
{
    info.mRecordTimeSet = true;
    info.mRecordTimeInMsecs = ms;
}

void AudioRecorder::stop()
{
    info.mStop = true;

    while (RECSTATE_RECORDING == info.mState) {
        usleep(3000);
    }

    exit();
}

bool AudioRecorder::isRecorded()
{
    return info.mRecorded;
}

void *recordThreadLoop(void *args)
{
    struct RecRuntimeInfo *info = (struct RecRuntimeInfo*)args;

    info->mPcm = pcm_open(info->mStreamCt.card, info->mStreamCt.device,
                    PCM_IN, &info->mStreamCt.config);

    if (!info->mPcm || !pcm_is_ready(info->mPcm)) {
        ALOGE("Unable to open PCM device (%s)\n", pcm_get_error(info->mPcm));
        info->mState = RECSTATE_RECORDED;
        return NULL;
    }

    size_t buf_size = pcm_frames_to_bytes(info->mPcm,
                                          pcm_get_buffer_size(info->mPcm));
    void *buffer = malloc(buf_size);
    if (!buffer) {
        ALOGE("can't malloc buffer");
        if (info->mPcm)
            pcm_close(info->mPcm);
        info->mState = RECSTATE_RECORDED;
        return NULL;
    }

    long recordBytes = info->mRecordTimeInMsecs *
                       (info->mStreamCt.config.rate/1000) *
                       info->mStreamCt.config.channels * sizeof(short);
    long recored = 0;
    while(1) {
        if (pcm_read(info->mPcm, buffer, buf_size)) {
            ALOGE("pcm_write failed (%s)", pcm_get_error(info->mPcm));
            break;
        }

        int ret = info->mContext->writeData(buffer, buf_size);
        if (!ret) {
            ALOGV("%s,line:%d", __func__, __LINE__);
            break;
        }

        recored += ret;
        if (info->mRecordTimeSet) {
            if (recordBytes < recored) {
                break;
            }
        }

        if (info->mStop) {
            break;
        }
    }
    free(buffer);

    if (info->mPcm) {
        pcm_close(info->mPcm);
        info->mPcm = NULL;
    }

    info->mStop = true;
    info->mRecorded = true;
    info->mState = RECSTATE_RECORDED;

    return NULL;
}

long AudioRecorder::record()
{
    int ret = 0;
    if (SINK_BUFFER == mSink) {
        ALOGE("line(%d): can't record(mSink:%d)",
              __LINE__, mSink);
        return 0;
    }

    if (RECSTATE_UNINIT  == info.mState) {
        init();
    }

    while(RECSTATE_INITING  == info.mState) {
        usleep(2000);
    }

    if (RECSTATE_INITED  != info.mState) {
        ALOGV("AudioRecorder can't recording(state:%d)", info.mState);
        return 0;
    }

    info.mStop = false;
    info.mState = RECSTATE_RECORDING;

    if (!info.mContext)
        return 0;

    pthread_t rtid;
    ret = pthread_create(&rtid, NULL, recordThreadLoop, &info);
    if (ret != 0) {
        ALOGE("line(%d): create thread for record failed", __LINE__);
        return 0;
    }

    return 0;
}

size_t AudioRecorder::getBufferSize()
{
    return info.mStreamCt.config.period_size *
           info.mStreamCt.config.channels *
           sizeof(short);
}

long AudioRecorder::record(char *recBuf, size_t recBufBytes)
{

    if (SINK_FILE == mSink) {
        ALOGE("line(%d): can't record for SINK_FILE", __LINE__);
        return 0;
    }

    if (RECSTATE_UNINIT  == info.mState) {
        info.mPcm = pcm_open(info.mStreamCt.card, info.mStreamCt.device,
                        PCM_IN, &info.mStreamCt.config);

        if (!info.mPcm || !pcm_is_ready(info.mPcm)) {
            ALOGE("Unable to open PCM device (%s)\n", pcm_get_error(info.mPcm));
            return 0;
        }
        info.mState = RECSTATE_INITED;
    }

    info.mState = RECSTATE_RECORDING;

    if (pcm_read(info.mPcm, recBuf, recBufBytes)) {
        ALOGE("pcm_write failed (%s)", pcm_get_error(info.mPcm));
        return 0;
    }

    info.mState = RECSTATE_RECORDED;

    return recBufBytes;
}
