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
#define LOG_TAG "AudioPlayer"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#include "AudioPlayer.h"

using namespace dragonboard;

AudioPlayer::AudioPlayer(StreamContext streamCt)
{
    info.mStreamCt = streamCt;
    info.mPlayTimeSet = true;
    info.mPlayTimeInMsecs = streamCt.timeInMsecs;
    info.mStop = false;
    info.mPlayed = false;
    info.mState = STATE_UNINIT;
    info.mContext = NULL;
    info.mPcm = NULL;
}

AudioPlayer::~AudioPlayer()
{
    stop();
}

bool AudioPlayer::init()
{
    bool ret = true;

    if (info.mState != STATE_UNINIT) {
        ALOGV("line(%d):AudioPlayer inited", __LINE__);
        return true;
    }

    info.mState = STATE_INITING;

    info.mContext = new AudioFileContext(info.mStreamCt.fileName,
                                         AudioFileContext::READ);
    if (!info.mContext) {
        ALOGE("%s:can't new AudioFileContext", __func__);
        info.mState = STATE_UNINIT;
        return false;
    }

    ret = info.mContext->init(info.mStreamCt.config.rate,
                              info.mStreamCt.config.channels);

    info.mStop = false;
    info.mPlayed = false;
    info.mState = STATE_INITED;

    return ret;
}

bool AudioPlayer::exit()
{
    bool ret = true;

    info.mStop = true;

    if (info.mContext) {
        ret = info.mContext->exit();
        delete info.mContext;
        info.mContext = NULL;
    }

    info.mState = STATE_UNINIT;
    return ret;
}

void AudioPlayer::setPlayTime(unsigned int ms)
{
    info.mPlayTimeSet = true;
    info.mPlayTimeInMsecs = ms;
}

void AudioPlayer::stop()
{
    info.mStop = true;

    while (STATE_PLAYING == info.mState) {
        usleep(3000);
    }

    exit();
}

bool AudioPlayer::isPlayed()
{
    return info.mPlayed;
}

void *playThreadLoop(void *args)
{

    struct RuntimeInfo *info = (struct RuntimeInfo*)args;

    info->mPcm = pcm_open(info->mStreamCt.card, info->mStreamCt.device,
                          PCM_OUT, &info->mStreamCt.config);
    if (!info->mPcm || !pcm_is_ready(info->mPcm)) {
        ALOGE("Unable to open PCM device (%s)\n", pcm_get_error(info->mPcm));
        info->mState = STATE_PLAYED;
        return 0;
    }

    size_t buf_size = pcm_frames_to_bytes(info->mPcm,
                                          pcm_get_buffer_size(info->mPcm));
    void *buffer = malloc(buf_size);
    if (!buffer) {
        ALOGE("can't malloc buffer");
        if (info->mPcm)
            pcm_close(info->mPcm);
        info->mState = STATE_PLAYED;
        return 0;
    }

    long playBytes = info->mPlayTimeInMsecs *
                     (info->mStreamCt.config.rate/1000) *
                     info->mStreamCt.config.channels * sizeof(short);
    long played = 0;
    while(1) {
        int ret = info->mContext->readData(buffer, buf_size);
        if (!ret) {
            ALOGV("%s,line:%d", __func__, __LINE__);
            break;
        }
        if (pcm_write(info->mPcm, buffer, buf_size)) {
            ALOGE("pcm_write failed (%s)", pcm_get_error(info->mPcm));
            break;
        }

        played += ret;
        if (info->mPlayTimeSet) {
            if (playBytes <= played) {
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
    info->mPlayed = true;
    info->mState = STATE_PLAYED;

    return NULL;
}

long AudioPlayer::play()
{
    int ret = 0;

    if (STATE_UNINIT  == info.mState) {
        init();
    }

    while(STATE_INITING  == info.mState) {
        usleep(2000);
    }

    if (STATE_INITED  != info.mState) {
        ALOGV("AudioPlayer can't playing(state:%d)", info.mState);
        return 0;
    }

    info.mStop = false;
    info.mState = STATE_PLAYING;

    if (!info.mContext)
        return 0;

    pthread_t ptid;
    ret = pthread_create(&ptid, NULL, playThreadLoop, &info);
    if (ret != 0) {
        ALOGE("line(%d): create thread for playback failed", __LINE__);
    }

    return 0;
}
