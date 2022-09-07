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
#define LOG_TAG "AudioFileContext"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "common.h"
#include "AudioFileContext.h"

using namespace dragonboard;

inline static void printRiffWaveHeader(struct riff_wave_header riff)
{
    ALOGV("riff_wave_header:\n"
          "\triff_id:%d\n"
          "\triff_sz:%d\n"
          "\twave_id:%d\n",
          riff.riff_id,
          riff.riff_sz,
          riff.wave_id);
}

inline static void printChunkHeader(struct chunk_header chunkHeader)
{
    ALOGV("chunk_header:\n"
          "\tid:%d\n"
          "\tsz:%d\n",
          chunkHeader.id,
          chunkHeader.sz);
}

inline static void printChunkFmt(struct chunk_fmt chunkFmt)
{
    ALOGV("chunk_fmt:\n"
          "\taudio_format:%d\n"
          "\tnum_channels:%d\n"
          "\tsample_rate:%d\n"
          "\tbyte_rate:%d\n"
          "\tblock_align:%d\n"
          "\tbits_per_sample:%d\n",
          chunkFmt.audio_format,
          chunkFmt.num_channels,
          chunkFmt.sample_rate,
          chunkFmt.byte_rate,
          chunkFmt.block_align,
          chunkFmt.bits_per_sample);
}

AudioFileContext::AudioFileContext(const char *filename, parser_type type):
mChannels(0), mRate(0), mFd(-1), state(STATE_UNINIT)
{
    this->type = type;
    this->mFileName = filename;
}

bool AudioFileContext::init(int rate, int channels)
{
    int ret = false;

#if 1
    /* check file permission */
    ret = access(mFileName, F_OK);
    if (-1 == ret) {
        ALOGE("line(%d):can't find file:%s",
              __LINE__, mFileName);
    }

    if (READ == type) {
        ret = access(mFileName, R_OK);
        if (-1 == ret) {
            ALOGE("line(%d):no read permission for file:%s",
                  __LINE__, mFileName);
        }
    }

    if (WRITE == type) {
        ret = access(mFileName, W_OK);
        if (-1 == ret) {
            ALOGE("line(%d):no write permission for file:%s",
                  __LINE__, mFileName);
        }
    }
#endif

    if (type == READ) {
        mFd = open(mFileName, O_RDONLY);
    } else if (type == WRITE) {
        mFd = open(mFileName, O_RDWR, O_CREAT);
    }

    if (mFd < 0) {
        ALOGE("line(%d):can't open file:%s,parser_type:%d",
              __LINE__, mFileName, type);
        return false;
    }

    state = STATE_FILE_OPENED;
    ALOGV("line(%d): open file(%s) ok.", __LINE__, mFileName);

    memset(&mRiffHeader, 0, sizeof(mRiffHeader));
    memset(&mChunkHeader, 0, sizeof(mChunkHeader));
    memset(&mChunkFmt, 0, sizeof(mChunkFmt));
    if (type == READ) {
        ret = readHeader();
    } else if (type == WRITE) {
        ret = writeHeader(rate, channels);
    }

    if (ret)
        state = STATE_INITED;

    ALOGV("line(%d): mRate:%d, mCh:%d, state:%d",
          __LINE__, mRate, mChannels, state);
    return ret;
}

bool AudioFileContext::exit()
{
    int ret;

    if (state != STATE_INITED) {
        return true;
    }

    if (WRITE == type) {
        lseek(mFd, sizeof(mRiffHeader.riff_id), SEEK_SET);
        ret = write(mFd, &mRiffHeader.riff_sz, sizeof(mRiffHeader.riff_sz));
        if (ret != sizeof(mRiffHeader.riff_sz)) {
            ALOGE("line(%d): can't write riff_sz", __LINE__);
            close(mFd);
            state = STATE_UNINIT;
            return false;
        }

        int dataHeaderOffset = sizeof(struct riff_wave_header) +
                               sizeof(struct chunk_header) +
                               sizeof(struct chunk_fmt);
        lseek(mFd, dataHeaderOffset, SEEK_SET);
        ret = write(mFd, &mChunkHeader, sizeof(mChunkHeader));
        if (ret != sizeof(mChunkHeader)) {
            ALOGE("line(%d): can't write data ChunkHeader", __LINE__);
            close(mFd);
            state = STATE_UNINIT;
            return false;
        }

    }

    close(mFd);
    state = STATE_UNINIT;
    return true;
}

bool AudioFileContext::readHeader()
{
    bool moreChunks = true;
    unsigned int ret;

    if (state != STATE_FILE_OPENED || type != READ) {
        return false;
    }

    /* 1. read riff header */
    ret = read(mFd, &mRiffHeader, sizeof(mRiffHeader));
    if (ret < sizeof(mRiffHeader) ||
        (mRiffHeader.riff_id != ID_RIFF) ||
        (mRiffHeader.wave_id != ID_WAVE)) {
        ALOGE("Error: audio file is not a riff/wave file\n");
        return false;
    }
    printRiffWaveHeader(mRiffHeader);

    do {
        /* 2. read chunk header */
        ret = read(mFd, &mChunkHeader, sizeof(mChunkHeader));
        if (ret < sizeof(mChunkHeader)) {
            ALOGE("EOF reading chunk headers");
            return false;
        }
        printChunkHeader(mChunkHeader);

        switch (mChunkHeader.id) {
            case ID_FMT:
                /* 3. read chunk fmt */
                ret = read(mFd, &mChunkFmt, sizeof(mChunkFmt));
                if (ret < sizeof(mChunkFmt)) {
                    ALOGE("format not found in WAV file");
                    return false;
                }
                printChunkFmt(mChunkFmt);
                break;
            case ID_DATA:
                /* Stop looking for chunks */
                moreChunks = 0;
                break;
            default:
                /* Unknown chunk, skip bytes */
            break;
        }
    } while (moreChunks);

    mChannels = mChunkFmt.num_channels;
    mRate = mChunkFmt.sample_rate;
    mFrameSize = mChannels * 2;


    if (mChunkFmt.bits_per_sample != 16) {
        ALOGE("only 16 bit WAV files are supported");
        return false;
    }

    return true;
}

bool AudioFileContext::writeHeader(int rate, int channels)
{
    unsigned int ret;

    if (state != STATE_FILE_OPENED|| type != WRITE) {
        return false;
    }

    if (-1 == ftruncate(mFd, 0)) {
        ALOGE("line(%d): ftruncate failed(errno:%d)", __LINE__, errno);
        return false;
    }

    /* 1. write riff header */
    memset(&mRiffHeader, 0, sizeof(mRiffHeader));
    mRiffHeader.riff_id = ID_RIFF;

    /* will change later */
    mRiffHeader.riff_sz = sizeof(mRiffHeader) + sizeof(mChunkHeader) +
                          sizeof(mChunkFmt) + sizeof(mChunkHeader) -
                          sizeof(mRiffHeader.riff_id) -
                          sizeof(mRiffHeader.riff_sz);

    mRiffHeader.wave_id = ID_WAVE;
    ret = write(mFd, &mRiffHeader, sizeof(mRiffHeader));
    if (ret < sizeof(mRiffHeader)) {
        ALOGE("line(%d):write header err.", __LINE__);
        return false;
    }
    printRiffWaveHeader(mRiffHeader);

    /* 2. write chunk header for chunk fmt */
    memset(&mChunkHeader, 0, sizeof(mChunkHeader));
    mChunkHeader.id = ID_FMT;
    mChunkHeader.sz =sizeof(mChunkFmt);
    ret = write(mFd, &mChunkHeader, sizeof(mChunkHeader));
    if (ret < sizeof(mChunkHeader)) {
        ALOGE("line(%d):write header err.", __LINE__);
        return false;
    }
    printChunkHeader(mChunkHeader);

    /* 3. write chunk fmt */
    memset(&mChunkFmt, 0, sizeof(mChunkFmt));
    mRate = rate;
    mChannels = channels;
    mFrameSize = mChannels * 2;
    mChunkFmt.audio_format = 1;
    mChunkFmt.num_channels = (uint16_t)mChannels;
    mChunkFmt.sample_rate = (uint32_t)mRate;
    mChunkFmt.byte_rate = (uint32_t)mRate * mChannels * mFrameSize;
    mChunkFmt.block_align = (uint16_t)mFrameSize;
    mChunkFmt.bits_per_sample = 16;
    printChunkFmt(mChunkFmt);

    ret = write(mFd, &mChunkFmt, sizeof(mChunkFmt));
    if (ret < sizeof(mChunkFmt)) {
        ALOGE("line(%d):write header err.", __LINE__);
        return false;
    }

    /* 2. write chunk header form data */
    memset(&mChunkHeader, 0, sizeof(mChunkHeader));
    mChunkHeader.id = ID_DATA;
    mChunkHeader.sz = 0; /* change later */
    ret = write(mFd, &mChunkHeader, sizeof(mChunkHeader));
    if (ret < sizeof(mChunkHeader)) {
        ALOGE("line(%d):write header err.", __LINE__);
        return false;
    }
    printChunkHeader(mChunkHeader);

    return true;
}

size_t AudioFileContext::readData(void *buffer, size_t bytes)
{
    int ret;

    if (STATE_INITED != state || type != READ) {
        ALOGE("can't readData.");
        return 0;
    }

    ret = read(mFd, buffer, bytes);

    return ret;
}

size_t AudioFileContext::writeData(void *buffer, size_t bytes)
{
    int ret;

    if (STATE_INITED != state || type != WRITE) {
        ALOGE("can't writeData.");
        return 0;
    }

    ret = write(mFd, buffer, bytes);
    mRiffHeader.riff_sz += ret;
    mChunkHeader.sz += ret; /*chunk header for data */

    return ret;
}

int AudioFileContext::getRate()
{
    return mRate;
}

int AudioFileContext::getChannels()
{
    return mChannels;
}
