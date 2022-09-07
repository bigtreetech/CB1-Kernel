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

#ifndef DRAGONBOARD_AUDIOFILECONTEXT_H
#define DRAGONBOARD_AUDIOFILECONTEXT_H

namespace dragonboard {
#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

struct riff_wave_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t wave_id;
};

struct chunk_header {
    uint32_t id;
    uint32_t sz;
};

struct chunk_fmt {
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
};

class AudioFileContext
{
public:
    int mChannels;
    int mRate;
    int mFrameSize;

public:
    enum parser_type {
        WRITE,
        READ
    };

    AudioFileContext(const char *filename, parser_type type);

    bool init(int rate, int channels);
    bool exit();

    int getRate();
    int getChannels();

    size_t readData(void *buffer, size_t bytes);
    size_t writeData(void *buffer, size_t bytes);

private:
    const char *mFileName;
    int mFd;
    int type;
    int state;
    enum {
        STATE_UNINIT,
        STATE_FILE_OPENED,
        STATE_INITED,
    };

    struct riff_wave_header mRiffHeader;
    struct chunk_header mChunkHeader;
    struct chunk_fmt mChunkFmt;

    bool readHeader();
    bool writeHeader(int rate, int channels);
};

} // namespace dragonboard

#endif
