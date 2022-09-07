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

#ifndef DRAGONBOARD_AUDIOTESTERINTERFACE_H
#define DRAGONBOARD_AUDIOTESTERINTERFACE_H
#include "common/common.h"

namespace dragonboard {

class AudioTesterInterface
{
public:

public:
    virtual ~AudioTesterInterface(){};
    virtual void testing() = 0;
    virtual bool getResult() = 0;
};

} // namespace dragonboard

#endif
