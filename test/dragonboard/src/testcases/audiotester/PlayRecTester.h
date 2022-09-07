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

#ifndef DRAGONBOARD_PLAYRECTESTER_H
#define DRAGONBOARD_PLAYRECTESTER_H
#include "common/common.h"
#include "AudioTesterInterface.h"
#include "common/FFT.h"
#include "common/AudioPlayer.h"
#include "common/AudioRecorder.h"
#include "common/AudioRouter.h"

using namespace dragonboard;

namespace dragonboard {

class PlayRecTester : public AudioTesterInterface
{
public:
    PlayRecTester(struct kvpair *spkCtls, int spkCtlsNum,
                  struct kvpair *hpCtls, int hpCtlsNum,
                  struct kvpair *hsCtls, int hsCtlsNum,
                  struct HsDetectInfo hsDetectInfo,
                  struct StreamContext pStream,
                  struct StreamContext rStream);

    ~PlayRecTester();

    void testing();
    bool getResult();
    void getRecInfo(int *isHsMic, int *left, int *right);

private:
    struct kvpair *mSpkCtls;
    int mSpkCtlsNum;
    struct kvpair *mHpCtls;
    int mHpCtlsNum;
    struct kvpair *mHsCtls;
    int mHsCtlsNum;

    struct HsDetectInfo mHsDetectInfo;

    struct StreamContext mPStream;
    struct StreamContext mRStream;

    bool mResult;

    volatile int mIsHsMic;
    volatile int mLeftVal;
    volatile int mRightVal;
};

} // namespace dragonboard

#endif
