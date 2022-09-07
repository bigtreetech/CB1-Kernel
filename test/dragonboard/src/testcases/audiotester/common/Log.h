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

#ifndef DRAGONBOARD_LOG_H
#define DRAGONBOARD_LOG_H

#if defined(__cplusplus)
extern "C" {
#endif
#include "dragonboard_inc.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif

#ifndef LOG_TAG
#define LOG_TAG "NULL"
#endif

#ifndef ALOGE
#define ALOGE(fmt, ...) \
    do { db_error(LOG_TAG ":" fmt "\n", ##__VA_ARGS__); } while (0)
#endif

#ifndef ALOGW
#define ALOGW(fmt, ...) \
    do { db_warn(LOG_TAG ":" fmt "\n", ##__VA_ARGS__); } while (0)
#endif

#ifndef ALOGD
#define ALOGD(fmt, ...) \
    do { db_msg(LOG_TAG ":" fmt "\n", ##__VA_ARGS__); } while (0)
#endif

#ifndef ALOGI
#define ALOGI(fmt, ...) \
    do { db_msg(LOG_TAG ":" fmt "\n", ##__VA_ARGS__); } while (0)
#endif

#ifndef ALOGV
#ifdef LOG_NDEBUG
#define ALOGV(fmt, ...) \
    do { db_msg(LOG_TAG ":" fmt "\n", ##__VA_ARGS__); } while (0)
#else
#define ALOGV(fmt, ...) \
    do {} while (0)
#endif
#endif

#endif
