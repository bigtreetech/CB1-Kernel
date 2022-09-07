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

#ifndef DRAGONBOARD_LOOPBACKTESTER_H
#define DRAGONBOARD_LOOPBACKTESTER_H
#include "common/common.h"
#include "AudioTesterInterface.h"
#include "common/FFT.h"
#include "common/AudioPlayer.h"
#include "common/AudioRecorder.h"
#include "common/AudioRouter.h"

using namespace dragonboard;

namespace dragonboard {

class LoopbackTester : public AudioTesterInterface
{
public:
    /* If rStream.fileName equal to NULL, FFT will use buffer directly */
    LoopbackTester(struct kvpair *mixerCtls, int ctlsNum,
                   struct StreamContext pStream,
                   struct StreamContext rStream);
    ~LoopbackTester();

    void testing();
    bool getResult();

private:
    void computeMaxFre(double *freL, double *freR,
                       const char *file);
    void computeMaxFre(double *freL, double *freR,
                       short *buffer, int frames, int ch, int rate);

private:
    struct kvpair *mMixerCtls;
    int mCtlsNum;
    struct StreamContext mPStream;
    struct StreamContext mRStream;

    /*if recordFile equal to NULL, record will use recBuf directly */
    short *recBuf;
    unsigned int mFrames;

    /* playback L/R channels frequency,
     * if mono, mPlayFre_L equal to mPlayFre_R
     */
    double mPlayFre_L;
    double mPlayFre_R;

    /* record L/R channels frequency,
     * if mono, mRecordFre_L equal to mRecordFre_R
     */
    double mRecordFre_L;
    double mRecordFre_R;

    bool mResult;
};

} // namespace dragonboard

#endif
