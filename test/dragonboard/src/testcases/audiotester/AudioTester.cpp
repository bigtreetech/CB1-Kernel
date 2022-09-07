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
#define LOG_TAG "AudioTester"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#include "common/common.h"
#include "AudioTesterInterface.h"

#include "LoopbackTester.h"
#include "PlayRecTester.h"

/* "platofrm.h" is "platform/CHIP_NAME/platform.h" */
#include "platform.h"

#if defined(__cplusplus)
extern "C" {
#endif

#include "sun8iw6p_display.h"
#include "dragonboard_inc.h"

#if defined(__cplusplus)
} /* extern "C" */
#endif

using namespace dragonboard;

/* script key name */
static char mainName[] = "audio";
static char interLbName[] = "internal_loopback_test";
static char exterLbName[] = "external_loopback_test";
static char playRecName[] = "playback_record_test";
static char hasMainMicName[] = "has_main_mic";
static char hasHsMicName[] = "has_headset_mic";
static char speakerVolumeName[] = "speaker_volume";
static char headsetVolumeName[] = "headset_volume";
static char mainMicGainName[] = "main_mic_gain";
static char headsetMicGainName[] = "headset_mic_gain";
static char musicPlayTimeName[] = "music_playtime";
static char mainMicThresholdName[] = "main_mic_threshold";
static char hsMicThresholdName[] = "hs_mic_threshold";

/* script value */
static int interLoopbackEnable;
static int exterLoopbackEnable;
static int playRecEnable;
static int hasMainMic;
static int hasHsMic;
static int speakerVolume;
static int headsetVolume;
static int mainMicGain;
static int headsetMicGain;
static int musicPlayTime;

static int mainMicThreshold = 8192;
static int hsMicThreshold = 8192;
static FILE *micPipe;
static bool mainMicPassed;
static bool hsMicPassed;
static bool micPassed;

int doLoopbackTest(struct kvpair *mixerCtls, int ctlsNum)
{
    bool result = false;

    AudioTesterInterface *loopback;
    loopback = new LoopbackTester(mixerCtls, ctlsNum,
                                  loopbackPStream, loopbackRStream);
    if (loopback) {
        loopback->testing();
        result = loopback->getResult();
        delete loopback;
    } else {
        ALOGE("line(%d):can't new LoopbackTester", __LINE__);
    }

    ALOGD("Loopback test result:%d", result);
    return result;
}

static void *doPlayRecTest(void *args)
{
    PlayRecTester *tester = (PlayRecTester *)args;

    tester->testing();

    return NULL;
}

int main(int argc, char **argv)
{
    int result = 0;

    ALOGD("start audio test for %s", CHIP_NAME);

    /* override the snd card id */
    int id;
    if (loopbackPStream.card_name) {
        id = get_card(loopbackPStream.card_name);
        if (id >= 0)
            loopbackPStream.card = id;
    }
    if (loopbackRStream.card_name) {
        id = get_card(loopbackRStream.card_name);
        if (id >= 0)
            loopbackRStream.card = id;
    }
    if (pStream.card_name) {
        id = get_card(pStream.card_name);
        if (id >= 0)
            pStream.card = id;
    }
    if (rStream.card_name) {
        id = get_card(rStream.card_name);
        if (id >= 0)
            rStream.card = id;
    }

    init_script(atoi(argv[2]));

    if (script_fetch(mainName, interLbName, &interLoopbackEnable, 1)) {
        interLoopbackEnable = 0;
        ALOGW("unknown %s,use default(interLoopbackEnable:%d)",
              interLbName, interLoopbackEnable);
    }
    if (script_fetch(mainName, exterLbName, &exterLoopbackEnable, 1)) {
        exterLoopbackEnable = 0;
        ALOGW("unknown %s,use default(exterLoopbackEnable:%d)",
              exterLbName, exterLoopbackEnable);
    }
    if (script_fetch(mainName, playRecName, &playRecEnable, 1)) {
        playRecEnable = 0;
        ALOGW("unknown %s,use default(playRecEnable:%d)",
              playRecName, playRecEnable);
    }

    if (script_fetch(mainName, hasMainMicName, &hasMainMic, 1)) {
        hasMainMic = 0;
        ALOGW("unknown %s,use default(hasMainMic:%d)",
              hasMainMicName, hasMainMic);
    }
    if (script_fetch(mainName, hasHsMicName, &hasHsMic, 1)) {
        hasHsMic = 0;
        ALOGW("unknown %s,use default(hasHsMic:%d)",
              hasHsMicName, hasHsMic);
    }
    if (script_fetch(mainName, speakerVolumeName, &speakerVolume, 1)) {
        speakerVolume = -1;
        ALOGW("unknown %s,use default(speakerVolume:%d)",
              speakerVolumeName, speakerVolume);
    }
    if (script_fetch(mainName, headsetVolumeName, &headsetVolume, 1)) {
        headsetVolume = -1;
        ALOGW("unknown %s,use default(headsetVolume:%d)",
              headsetVolumeName, headsetVolume);
    }
    if (script_fetch(mainName, mainMicGainName, &mainMicGain, 1)) {
        mainMicGain = -1;
        ALOGW("unknown %s,use default(mainMicGain:%d)",
              mainMicGainName, mainMicGain);
    }
    if (script_fetch(mainName, headsetMicGainName, &headsetMicGain, 1)) {
        headsetMicGain = -1;
        ALOGW("unknown %s,use default(headsetMicGain:%d)",
              headsetMicGainName, headsetMicGain);
    }
    if (script_fetch(mainName, musicPlayTimeName, &musicPlayTime, 1)) {
        musicPlayTime = 2;
        ALOGW("unknown %s,use default(musicPlayTime:%d)",
              musicPlayTimeName, musicPlayTime);
    }
    if (script_fetch(mainName, mainMicThresholdName, &mainMicThreshold, 1)) {
        mainMicThreshold = 8192;
        ALOGW("unknown %s,use default(mainMicThreshold:%d)",
              mainMicThresholdName, mainMicThreshold);
    }
    if (script_fetch(mainName, hsMicThresholdName, &hsMicThreshold, 1)) {
        hsMicThreshold = 8192;
        ALOGW("unknown %s,use default(hsMicThreshold:%d)",
              hsMicThresholdName, hsMicThreshold);
    }

    INIT_CMD_PIPE();

    /* check file node */
    char pPath[20];
    char rPath[20];
    sprintf(pPath, "/dev/snd/pcmC%dD0p", pStream.card);
    sprintf(rPath, "/dev/snd/pcmC%dD0c", rStream.card);
    if (-1 == access(pPath, R_OK)) {
            ALOGE("%s missing or no read permission", pPath);
            SEND_CMD_PIPE_FAIL();
            return 0;
    }
    if (-1 == access(rPath, R_OK)) {
            ALOGE("%s missing or no read permission", rPath);
            SEND_CMD_PIPE_FAIL();
            return 0;
    }
    if (-1 == access(hsDetectInfo.path, R_OK)) {
            ALOGE("%s missing or no read permission", hsDetectInfo.path);
            SEND_CMD_PIPE_FAIL();
            return 0;
    }

    if (interLoopbackEnable) {
        result = doLoopbackTest(interLbCtls, interLbNumCtls);
        if (result) {
            //SEND_CMD_PIPE_OK();
        } else {
            SEND_CMD_PIPE_FAIL();
            return 0;
        }
    }

    if (exterLoopbackEnable) {
        result = doLoopbackTest(exterLbCtls, exterLbNumCtls);
        if (result) {
            //SEND_CMD_PIPE_OK();
        } else {
            SEND_CMD_PIPE_FAIL();
            return 0;
        }
    }

    if (playRecEnable) {

        micPipe = fopen(MIC_PIPE_NAME, "w");
        if (micPipe == NULL) {
            ALOGE("fail to open mic_pipe");
            return 0;
        }
        setlinebuf(micPipe);

        if (musicPlayTime > 0)
            pStream.timeInMsecs = musicPlayTime;

        PlayRecTester *tester;
        tester = new PlayRecTester(spkCtls, spkCtlsNum,
                                   hpCtls, hpCtlsNum,
                                   hsCtls, hsCtlsNum,
                                   hsDetectInfo,
                                   pStream,
                                   rStream);

        pthread_t testtid;
        int ret = pthread_create(&testtid, NULL, doPlayRecTest, tester);
        if (ret != 0) {
            ALOGE("line(%d): create thread for PlayRecTest failed", __LINE__);
        }

        while (1) {
            char audioData[32];
            int isHsMic = 0;
            int recLeftVal = 0;
            int recRightVal = 0;

            tester->getRecInfo(&isHsMic, &recLeftVal, &recRightVal);

            if ((hasHsMic && isHsMic) ||
                (hasMainMic && !isHsMic)) {
                sprintf(audioData, "%d#%d", recLeftVal, recRightVal);
                fprintf(micPipe, "%s\n", audioData);
            } else {
                sprintf(audioData, "%d#%d", 0, 0);
                fprintf(micPipe, "%s\n", audioData);
            }

            usleep(20000);

            if (micPassed)
                continue;

            if (isHsMic && hasHsMic) {
                if (!hsMicPassed) {
                    if (recLeftVal > hsMicThreshold ||
                        recRightVal > hsMicThreshold) {
                        hsMicPassed = true;
                        ALOGD("headset mic passed.");
                    }
                }

            } else if(!isHsMic && hasMainMic) {
                if (!mainMicPassed) {
                    if (recLeftVal > mainMicThreshold ||
                        recRightVal > mainMicThreshold) {
                        mainMicPassed = true;
                        ALOGD("main mic passed.");
                    }
                }
            }

            if (hasHsMic && hsMicPassed &&
                hasMainMic && mainMicPassed) {
                micPassed = true;
            } else if (!hasHsMic &&
                       hasMainMic && mainMicPassed) {
                micPassed = true;
            } else if (hasHsMic && hsMicPassed &&
                       !hasMainMic) {
                micPassed = true;
            } else if (!hasHsMic &&
                       !hasMainMic) {
                micPassed = true;
            }

            if (micPassed) {
                ALOGD("mic passed.");
                SEND_CMD_PIPE_OK();
            }
        } //while

        delete tester;
    }

    SEND_CMD_PIPE_OK();

    return 0;
}
