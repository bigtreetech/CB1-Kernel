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
#define LOG_TAG "LoopbackTester"

#include<pthread.h>

#include "LoopbackTester.h"

#define UNUSED(x) (void)(x)

/* max frequency compare threshold (Hz) */
#define MAX_FRE_CMP_THRESHOLD (200.0)

/* playback file path */
#define LOOPBACK_PLAY_FILE "/system/media/sin1k.wav"

/* record file path */
#define LOOPBACK_RECORD_FILE "/system/media/record.wav"

using namespace dragonboard;

/* get frames by rate. frames is power of 2 and frames >= rate */
inline static unsigned int getFramesByRate(unsigned int rate)
{
    unsigned int i = 0;

    while(i < sizeof(rate) * 8) {
        if (!(rate>>i)) {
            break;
        }
        i++;
    }

    return 1<<i;
}

LoopbackTester::LoopbackTester(struct kvpair *mixerCtls, int ctlsNum,
                               struct StreamContext pStream,
                               struct StreamContext rStream) :
recBuf(NULL), mFrames(0), mResult(false)
{
    mMixerCtls = mixerCtls;
    mCtlsNum = ctlsNum;
    mPStream = pStream;
    mRStream = rStream;

    /* if not indicate record file, use buffer for record & fft */
    if (NULL == mRStream.fileName) {
        mFrames = getFramesByRate(mRStream.config.rate);
        recBuf = (short*)calloc(mRStream.config.channels * mFrames *
                                sizeof(short), 1);
        if (!recBuf) {
            ALOGE("line(%d): can't calloc for recBuf", __LINE__);
        }
    }
}

LoopbackTester::~LoopbackTester()
{
    if (recBuf)
        free(recBuf);
}

void LoopbackTester::computeMaxFre(double *freL, double *freR,
                                   const char *file)
{
    double fre = 0.0;
    double val = 0.0;

    int ret = 0;
    int rate = 0;
    int ch = 0;
    int frames = 0;

    *freL = *freR = 0.0;

    AudioFileContext *context;
    ALOGV("%s,line(%d): file:%s", __func__, __LINE__, file);
    context = new AudioFileContext(file, AudioFileContext::READ);
    if (!context) {
        ALOGE("line(%d): can't new AudioFileContext", __LINE__);
        return;
    }

    context->init(0, 0);

    rate = context->getRate();
    ch = context->getChannels();
    frames = getFramesByRate(rate);

    int bytes = frames * ch * sizeof(short);

    ALOGV("line(%d): rate:%d,ch:%d,frames:%d", __LINE__, rate, ch, frames);

    short *buffer = (short*)calloc(bytes, 1);
    if (buffer) {
        ret = context->readData(buffer, bytes);
        if (ret != bytes) {
            ALOGE("readData, bytes=%d", ret);
            frames = ret / (ch * sizeof(short));
        }
    }

    FFT *fft;
    fft = new FFT();
    if (1 == ch) {
        fft->init(buffer, frames, rate);
        fft->doFft();
        fft->getMaxFre(&fre, &val);
        *freL = *freR = fre;
        fft->exit();
    } else if (2 == ch) {
        short *chBuf = (short*)calloc(frames * sizeof(short), 1);
        if (!buffer) {
            ALOGE("line(%d): can't calloc buffer", __LINE__);
        } else {

            /* fft for left ch */
            for (int i = 0; i < frames; i ++) {
                chBuf[i] = buffer[2*i];
            }
            fft->init(chBuf, frames, rate);
            fft->doFft();
            fft->getMaxFre(&fre, &val);
            *freL = fre;
            fft->exit();

            /* fft for right ch */
            for (int i = 0; i < frames; i ++) {
                chBuf[i] = buffer[1 + 2*i];
            }
            fft->init(chBuf, frames, rate);
            fft->doFft();
            fft->getMaxFre(&fre, &val);
            *freR = fre;
            fft->exit();

            free(chBuf);
        }
    }
    delete fft;

    ALOGV("computeMaxFre:freL:%f,freR:%f", *freL, *freR);

    delete context;
    free(buffer);
}

void LoopbackTester::computeMaxFre(double *freL, double *freR,
                                   short *buffer, int frames, int ch, int rate)
{
    double fre = 0.0;
    double val = 0.0;

    *freL = *freR = 0.0;

    ALOGV("line(%d): rate:%d,ch:%d,frames:%d", __LINE__, rate, ch, frames);

    FFT fft;
    if (1 == ch) {
        fft.init(buffer, frames, rate);
        fft.doFft();
        fft.getMaxFre(&fre, &val);
        *freL = *freR = fre;
        fft.exit();
    } else if (2 == ch) {
        short *chBuf = (short*)calloc(frames * sizeof(short), 1);
        if (!buffer) {
            ALOGE("line(%d): can't calloc buffer", __LINE__);
        } else {

            /* fft for left ch */
            for (int i = 0; i < frames; i ++) {
                chBuf[i] = buffer[2*i];
            }
            fft.init(chBuf, frames, rate);
            fft.doFft();
            fft.getMaxFre(&fre, &val);
            *freL = fre;
            fft.exit();

            /* fft for right ch */
            for (int i = 0; i < frames; i ++) {
                chBuf[i] = buffer[1 + 2*i];
            }
            fft.init(chBuf, frames, rate);
            fft.doFft();
            fft.getMaxFre(&fre, &val);
            *freR = fre;
            fft.exit();

            free(chBuf);
        }
    }

    ALOGV("computeMaxFre:freL:%f,freR:%f", *freL, *freR);
}

static void *do_play(void *args)
{
    struct StreamContext *ct = (struct StreamContext *)args;

    AudioPlayer *player;
    player = new AudioPlayer(*ct);
    if (player) {
        player->setPlayTime(ct->timeInMsecs);
        player->play();
        while (!player->isPlayed()) {
            usleep(400000);
        }
        delete player;
    } else {
        ALOGE("line(%d): new AudioPlayer failed", __LINE__);
    }
    return NULL;
}

static void *do_record(void *args)
{
    struct StreamContext *ct = (struct StreamContext *)args;

    /* wait 500ms. Make sure the music has started to play */
    usleep(500000);

    if (ct->fileName) {
        /* record to file */
        AudioRecorder *recorder;
        recorder = new AudioRecorder(*ct);
        if (recorder) {
            recorder->setRecordTime(ct->timeInMsecs);
            recorder->record();
            while (!recorder->isRecorded()) {
                usleep(400000);
            }
            delete recorder;
        } else {
            ALOGE("line(%d): new AudioRecorder failed", __LINE__);
        }
    } else {
        /* record to buffer */
    }

    return NULL;
}

void LoopbackTester::testing()
{
    int ret = 0;

    /* 1. audio mixer routing */
    AudioRouter *router;
    router = new AudioRouter(mPStream.card);
    router->init();
    router->routing(mMixerCtls, mCtlsNum);
    router->exit();
    delete router;

    /* 2. play sina wave file in playback thread */
    pthread_t ptid;
    ret = pthread_create(&ptid, NULL, do_play, &mPStream);
    if (ret != 0) {
        ALOGE("line(%d): create thread for playback failed", __LINE__);
    }

    /* 3. record sound in record thread */
    pthread_t rtid;
    ret = pthread_create(&rtid, NULL, do_record, &mRStream);
    if (ret != 0) {
        ALOGE("line(%d):create thread for record failed", __LINE__);
    }

    /* 4. wait playback and record thread finished */
    void *tret = NULL;
    if (pthread_join(ptid, &tret)) {
        ALOGE("line(%d):join playback thread failed(%p)", __LINE__, tret);
    } else {
        ALOGV("playback thread finished.");
    }

    if (pthread_join(rtid, &tret)) {
        ALOGE("line(%d):join record thread failed(%p)", __LINE__, tret);
    } else {
        ALOGV("record thread finished.");
    }

    /* 5. get test result by fft */
    computeMaxFre(&mPlayFre_L, &mPlayFre_R, mPStream.fileName);
    if (mRStream.fileName) {
        computeMaxFre(&mRecordFre_L, &mRecordFre_R, mRStream.fileName);
    } else {
    }

    ALOGD("line(%d): playFre(L,R)=(%f, %f), recordFre(L,R)=(%f, %f)",
                __LINE__, mPlayFre_L, mPlayFre_R,
                mRecordFre_L, mRecordFre_R);

    /* left ch */
    if ((mPlayFre_L - mRecordFre_L) < MAX_FRE_CMP_THRESHOLD &&
        (mPlayFre_L - mRecordFre_L) > -MAX_FRE_CMP_THRESHOLD) {
        mResult = true;
    } else {
        mResult = false;
        return;
    }

    /* right ch */
    if ((mPlayFre_R - mRecordFre_R) < MAX_FRE_CMP_THRESHOLD &&
        (mPlayFre_R - mRecordFre_R) > -MAX_FRE_CMP_THRESHOLD) {
        mResult = true;
    } else {
        mResult = false;
    }
}

bool LoopbackTester::getResult()
{
    return mResult;
}
