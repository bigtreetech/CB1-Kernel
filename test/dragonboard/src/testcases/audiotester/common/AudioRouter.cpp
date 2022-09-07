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
#define LOG_TAG "AudioRouter"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "AudioRouter.h"

using namespace dragonboard;

inline static void tinymix_set_value(struct mixer *mixer, unsigned int id,
                              unsigned int value)
{
    struct mixer_ctl *ctl;
    //enum mixer_ctl_type type;
    unsigned int num_values;
    unsigned int i;

    ctl = mixer_get_ctl(mixer, id);
    //type = mixer_ctl_get_type(ctl);
    num_values = mixer_ctl_get_num_values(ctl);

    for (i = 0; i < num_values; i++) {
        if (mixer_ctl_set_value(ctl, i, value)) {
            ALOGE("Error: invalid value");
            return;
        }
    }
}

inline static void tinymix_set_value_byname(struct mixer *mixer, const char *name,
                              unsigned int value)
{
    struct mixer_ctl *ctl;
    enum mixer_ctl_type type;
    unsigned int num_values;
    unsigned int i;

    ctl = mixer_get_ctl_by_name(mixer, name);
    type = mixer_ctl_get_type(ctl);
    num_values = mixer_ctl_get_num_values(ctl);

    if (MIXER_CTL_TYPE_ENUM == type) {
        if (mixer_ctl_set_enum_by_string(ctl, (const char *)value)) {
            ALOGE("line(%d):Error: invalid value", __LINE__);
            return;
        }
    } else {
        for (i = 0; i < num_values; i++) {
            if (mixer_ctl_set_value(ctl, i, value)) {
                ALOGE("line(%d):Error: invalid value", __LINE__);
                return;
            }
        }
    }
}

AudioRouter::AudioRouter(int card)
{
    mCard = card;
}

bool AudioRouter::init()
{
    mMixer = mixer_open(mCard);
    if (!mMixer) {
        ALOGE("FAILED to open mixer");
        return false;
    }

    return true;
}

bool AudioRouter::exit()
{
    if (mMixer)
        mixer_close(mMixer);

    return true;
}

bool AudioRouter::routing(struct kvpair *kvpairs, int num)
{
    for (int i = 0; i < num; i++) {
        tinymix_set_value_byname(mMixer, kvpairs[i].name, kvpairs[i].val);
    }

    return true;
}