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
#define CHIP_NAME "sun8iw6p"

/* headset detect info */
struct HsDetectInfo hsDetectInfo = {
    path:"/sys/class/switch/h2w/state",
    SPK:0,
    HEADPHONE:2,
    HEADSET:1
};

/* mixer clts for internal loopback */
struct kvpair interLbCtls[] = {
};
int interLbNumCtls = sizeof(interLbCtls)/sizeof(interLbCtls[0]);

/* mixer clts for external loopback */
struct kvpair exterLbCtls[] = {
};
int exterLbNumCtls = sizeof(exterLbCtls)/sizeof(exterLbCtls[0]);

/* mixer clts for spk & mainmic */
struct kvpair spkCtls[] = {
    {"Headphone Switch", 0},

    {"AIF1IN0R Mux", (unsigned int)"AIF1_DA0R"},
    {"AIF1IN0L Mux", (unsigned int)"AIF1_DA0L"},
    {"DACR Mixer AIF1DA0R Switch", 1},
    {"DACL Mixer AIF1DA0L Switch", 1},
    {"Right Output Mixer DACR Switch", 1},
    {"Left Output Mixer DACL Switch", 1},
    {"SPK_L Mux", (unsigned int)"MIXEL Switch"},
    {"SPK_R Mux", (unsigned int)"MIXER Switch"},
    {"External Speaker Switch", 1},

    {"AIF1OUT0L Mux", (unsigned int)"AIF1_AD0L"},
    {"AIF1OUT0R Mux", (unsigned int)"AIF1_AD0R"},
    {"AIF1 AD0L Mixer ADCL Switch", 1},
    {"AIF1 AD0R Mixer ADCR Switch", 1},
    {"ADCR Mux", (unsigned int)"ADC"},
    {"ADCL Mux", (unsigned int)"ADC"},
    {"LEFT ADC input Mixer MIC1 boost Switch", 1},
    {"RIGHT ADC input Mixer MIC1 boost Switch", 1}
};
int spkCtlsNum = sizeof(spkCtls)/sizeof(spkCtls[0]);

/* mixer clts for headphone & mainmic */
struct kvpair hpCtls[] = {
    {"External Speaker Switch", 0},

    {"AIF1IN0R Mux", (unsigned int)"AIF1_DA0R"},
    {"AIF1IN0L Mux", (unsigned int)"AIF1_DA0L"},
    {"DACR Mixer AIF1DA0R Switch", 1},
    {"DACL Mixer AIF1DA0L Switch", 1},
    {"HP_R Mux", (unsigned int)"DACR HPR Switch"},
    {"HP_L Mux", (unsigned int)"DACL HPL Switch"},
    {"Headphone Switch", 1},

    {"AIF1OUT0L Mux", (unsigned int)"AIF1_AD0L"},
    {"AIF1OUT0R Mux", (unsigned int)"AIF1_AD0R"},
    {"AIF1 AD0L Mixer ADCL Switch", 1},
    {"AIF1 AD0R Mixer ADCR Switch", 1},
    {"ADCR Mux", (unsigned int)"ADC"},
    {"ADCL Mux", (unsigned int)"ADC"},
    {"LEFT ADC input Mixer MIC1 boost Switch", 1},
    {"RIGHT ADC input Mixer MIC1 boost Switch", 1}
};
int hpCtlsNum = sizeof(hpCtls)/sizeof(hpCtls[0]);

/* mixer clts for headset & headsetmic */
struct kvpair hsCtls[] = {
    {"External Speaker Switch", 0},

    {"AIF1IN0R Mux", (unsigned int)"AIF1_DA0R"},
    {"AIF1IN0L Mux", (unsigned int)"AIF1_DA0L"},
    {"DACR Mixer AIF1DA0R Switch", 1},
    {"DACL Mixer AIF1DA0L Switch",1},
    {"HP_R Mux", (unsigned int)"DACR HPR Switch"},
    {"HP_L Mux", (unsigned int)"DACL HPL Switch"},
    {"Headphone Switch", 1},

    {"AIF1OUT0L Mux", (unsigned int)"AIF1_AD0L"},
    {"AIF1OUT0R Mux", (unsigned int)"AIF1_AD0R"},
    {"AIF1 AD0L Mixer ADCL Switch", 1},
    {"AIF1 AD0R Mixer ADCR Switch", 1},
    {"ADCR Mux", (unsigned int)"ADC"},
    {"ADCL Mux", (unsigned int)"ADC"},
    {"RIGHT ADC input Mixer MIC2 boost Switch", 1},
    {"LEFT ADC input Mixer MIC2 boost Switch", 1},
    {"MIC2 SRC", (unsigned int)"MIC2"}
};
int hsCtlsNum = sizeof(hsCtls)/sizeof(hsCtls[0]);

/* loopback playback stream */
struct StreamContext loopbackPStream = {
    card : 0,
    card_name : "snddaudio0",
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
    card_name : "snddaudio0",
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
    card_name : "snddaudio0",
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
    card_name : "snddaudio0",
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

