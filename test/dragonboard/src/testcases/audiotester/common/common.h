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

#ifndef DRAGONBOARD_COMMON_H
#define DRAGONBOARD_COMMON_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Log.h"
#include "tinyalsa/asoundlib.h"

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

namespace dragonboard {

/* headset detect info */
struct HsDetectInfo {
    const char *path;
    int SPK;
    int HEADPHONE;
    int HEADSET;
};

/* mixer ctls key value pair */
struct kvpair {
    const char *name;
    int val;
};

int get_card(const char *card_name);

/* streamContext represents the information used to play or record */
struct StreamContext {
    /*If you specify the card_name, it will override the card id*/
    int card;
    const char *card_name;
    int device;
    struct pcm_config config;

    const char *fileName;

    unsigned int timeInMsecs;
};

} // namespace dragonboard

#endif
