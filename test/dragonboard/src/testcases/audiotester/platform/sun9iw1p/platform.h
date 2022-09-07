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

#ifndef DRAGONBOARD_PLATFORM_H
#define DRAGONBOARD_PLATFORM_H
#include "../../common/common.h"

/* chip name for log */
#define CHIP_NAME "sun9iw1p"

/* headset detect info */
struct HsDetectInfo hsDetectInfo = {
    path:"/sys/class/switch/h2w/state",
    SPK:0,
    HEADPHONE:2,
    HEADSET:1
};

/* mixer clts for internal loopback */
struct kvpair interLbCtls[] = {
    {"Audio loopback", 1}
};
int interLbNumCtls = sizeof(interLbCtls)/sizeof(interLbCtls[0]);

/* mixer clts for external loopback */
struct kvpair exterLbCtls[] = {
    {"Audio loopback", 1}
};
int exterLbNumCtls = sizeof(exterLbCtls)/sizeof(exterLbCtls[0]);

/* mixer clts for spk & mainmic */
struct kvpair spkCtls[] = {
    {"Audio loopback", 0},
    {"Speaker Function", (unsigned int)"spk"},
    {"Audio headsetmic voicerecord", 0}
};
int spkCtlsNum = sizeof(spkCtls)/sizeof(spkCtls[0]);

/* mixer clts for headphone & mainmic */
struct kvpair hpCtls[] = {
    {"Audio loopback", 0},
    {"Speaker Function", (unsigned int)"headset"},
    {"Audio headsetmic voicerecord", 0}
};
int hpCtlsNum = sizeof(hpCtls)/sizeof(hpCtls[0]);

/* mixer clts for headset & headsetmic */
struct kvpair hsCtls[] = {
    {"Audio loopback", 0},
    {"Speaker Function", (unsigned int)"headset"},
    {"Audio headsetmic voicerecord", 1}
};
int hsCtlsNum = sizeof(hsCtls)/sizeof(hsCtls[0]);

/* loopback playback stream */
struct StreamContext loopbackPStream = {
    card : 0,
    card_name : NULL,
    device : 0,
    config : {
        channels : 1,
        rate : 44100,
        period_size : 1024,
        period_count : 2,
        format : PCM_FORMAT_S16_LE,
    },
    fileName : "/dragonboard/data/sin1k.wav",
    timeInMsecs : 5000,
};

/* loopback record stream */
struct StreamContext loopbackRStream = {
    card : 0,
    card_name : NULL,
    device : 0,
    config : {
        channels : 1,
        rate : 44100,
        period_size : 1024,
        period_count : 2,
        format : PCM_FORMAT_S16_LE,
    },
    fileName : "/dragonboard/data/loopback_rec.wav",
    timeInMsecs : 3000,
};

/* normal playback stream */
struct StreamContext pStream = {
    card : 0,
    card_name : NULL,
    device : 0,
    config : {
        channels : 2,
        rate : 48000,
        period_size : 1024,
        period_count : 2,
        format : PCM_FORMAT_S16_LE,
    },
    fileName : "/dragonboard/data/test48000.wav",
    timeInMsecs : 2000,
};

/* normal record stream */
struct StreamContext rStream = {
    card : 0,
    card_name : NULL,
    device : 0,
    config : {
        channels : 2,
        rate : 48000,
        period_size : 1024,
        period_count : 2,
        format : PCM_FORMAT_S16_LE,
    },
    fileName : NULL,
    timeInMsecs : 2000,
};

#endif
