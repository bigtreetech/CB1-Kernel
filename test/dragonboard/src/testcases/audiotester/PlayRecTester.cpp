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
#define LOG_TAG "PlayRecTester"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "PlayRecTester.h"

#define UNUSED(x) (void)(x)

bool playThreadExited;
bool RecThreadExited;
bool startPlay;
bool startRec;
bool playFinished;
bool recFinished;

using namespace dragonboard;

PlayRecTester::PlayRecTester(struct kvpair *spkCtls, int spkCtlsNum,
                             struct kvpair *hpCtls, int hpCtlsNum,
                             struct kvpair *hsCtls, int hsCtlsNum,
                             struct HsDetectInfo hsDetectInfo,
                             struct StreamContext pStream,
                             struct StreamContext rStream) :
mResult(false),
mIsHsMic(0), mLeftVal(0), mRightVal(0)
{
    mSpkCtls = spkCtls;
    mSpkCtlsNum = spkCtlsNum;
    mHpCtls = hpCtls;
    mHpCtlsNum = hpCtlsNum;
    mHsCtls = hsCtls;
    mHsCtlsNum = hsCtlsNum;
    mHsDetectInfo = hsDetectInfo;
    mPStream = pStream;
    mRStream = rStream;

    ALOGV("mSpkCtlsNum:%d, mHpCtlsNum:%d, mHsCtlsNum:%d.",
          mSpkCtlsNum, mHpCtlsNum, mHsCtlsNum);
}

PlayRecTester::~PlayRecTester()
{
}

void PlayRecTester::getRecInfo(int *isHsMic, int *left, int *right)
{
    *isHsMic = mIsHsMic;
    *left = mLeftVal;
    *right = mRightVal;
}

inline static int getOutDev(const char *path)
{
    int fd;
    char jack_buf[16];
    int jack_state = 0;
    int read_count;

    fd = open(path, O_RDONLY|O_NONBLOCK);
    if (fd < 0) {
        ALOGE("cannot open %s(%s)\n", path, strerror(errno));
        return 0;
    }

    read_count = read(fd, jack_buf, sizeof(jack_buf));
    if (read_count < 0) {
        ALOGE("jack state read error\n");
        return 0;
    }
    jack_state = atoi(jack_buf);
    close(fd);

    return jack_state;
}

void PlayRecTester::testing()
{
    int newDev = -1;
    int outDev = -1000;
    int loopcount = 0;

    ALOGV("line(%d):start PlayRecTest...", __LINE__);

    AudioPlayer *player;
    player = new AudioPlayer(mPStream);
    if (!player) {
        ALOGE("line(%d): new AudioPlayer failed", __LINE__);
        return;
    }

    AudioRecorder *recorder;
    recorder = new AudioRecorder(mRStream);
    if (!recorder) {
        ALOGE("line(%d): new AudioRecorder failed", __LINE__);
        return;
    }

    AudioRouter *router;
    router = new AudioRouter(mPStream.card);
    if (!router) {
        ALOGE("line(%d): new AudioRouter failed", __LINE__);
        return;
    }

    char *recBuf = NULL;
    size_t recBufBytes = recorder->getBufferSize();
    recBuf = (char*)calloc(recBufBytes, 1);
    if (!recBuf) {
        ALOGE("line(%d): calloc recBuf failed(buf size:%dbytes)",
              __LINE__, recBufBytes);
        return;
    }

    router->init();

    while (1) {
        newDev = getOutDev(mHsDetectInfo.path);

        if (newDev != outDev) {
            outDev = newDev;
            ALOGD("out device:%d", outDev);

            player->stop();
            recorder->stop();

            if (mHsDetectInfo.SPK == outDev) {
                router->routing(mSpkCtls, mSpkCtlsNum);
            } else if (mHsDetectInfo.HEADPHONE == outDev) {
                 router->routing(mHpCtls, mHpCtlsNum);
            } else if (mHsDetectInfo.HEADSET == outDev) {
                router->routing(mHsCtls, mHsCtlsNum);
            } else {
                ALOGW("unknow outDev:%d,use spk", outDev);
                router->routing(mSpkCtls, mSpkCtlsNum);
            }

            player->play();

            /* skip start frames */
            int skipcount = 5;
            while(--skipcount) {
                recorder->record(recBuf, recBufBytes);
            }
        }

        recorder->record(recBuf, recBufBytes);
        if (++loopcount == 2) {
            loopcount = 0;

            short *ptr = (short*)recBuf;
            int left = 0;
            int right = 0;
            uint32_t i;

            for (i = 0; i < (recBufBytes>>2); i++) {
                left += abs(*ptr++);
                right += abs(*ptr++);
            }
            left /= i;
            if (left >= 32767)
                left = 32767;
            right /= i;
            if (right >= 32767)
                right = 32767;

            mIsHsMic = (mHsDetectInfo.HEADSET == outDev);
            mLeftVal = left;
            mRightVal = right;
        }
    }

    player->stop();
    delete player;

    recorder->stop();
    delete recorder;

    router->exit();
    delete router;
}

bool PlayRecTester::getResult()
{
    return mResult;
}
