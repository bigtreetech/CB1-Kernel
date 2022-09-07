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

#ifndef DRAGONBOARD_AUDIORECORDER_H
#define DRAGONBOARD_AUDIORECORDER_H

#include "common.h"
#include "AudioFileContext.h"

namespace dragonboard {

enum recstate {
    RECSTATE_UNINIT,
    RECSTATE_INITING,
    RECSTATE_INITED,
    RECSTATE_RECORDING,
    RECSTATE_RECORDED,
};

struct RecRuntimeInfo {
    StreamContext mStreamCt;
    AudioFileContext *mContext;
    struct pcm *mPcm;

    bool mRecordTimeSet;
    unsigned int mRecordTimeInMsecs;

    volatile bool mStop;
    volatile bool mRecorded;
    volatile recstate mState;
};

class AudioRecorder
{
public:
    AudioRecorder(StreamContext streamCt);
    ~AudioRecorder();
    long record();
    long record(char *recBuf, size_t recBufBytes);
    void setRecordTime(unsigned int ms);
    void stop();
    bool isRecorded();
    size_t getBufferSize();

private:
    bool init();
    bool exit();
private:
    enum sink_type {
        SINK_FILE,
        SINK_BUFFER,
    };
    sink_type mSink;

    struct RecRuntimeInfo info;
};

} // namespace dragonboard

#endif
