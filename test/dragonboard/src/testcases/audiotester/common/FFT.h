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

#ifndef DRAGONBOARD_FFT_H
#define DRAGONBOARD_FFT_H

#define PI 3.1415926535897932384626433832795028841971

namespace dragonboard {

class FFT
{
public:
    FFT();
    bool init(const short *data, int num, int fs);
    bool init(const double *data, int num, int fs);
    bool exit();

    bool doFft();
    bool getMaxFre(double *fre, double *val);

private:
    enum {
        STATE_UNINIT,
        STATE_INITED,
        STATE_FFT_DOING,
        STATE_FFT_DONE,
    };

    int mState;
    int mFs;   /* samp rate */
    int mNum; /* num of data */

    /* array of complex* data points of size mNum * 2 + 1
     * mData[0] is unused
     * mData[2*n+1] = real(x(n))
     * mData[2*n+2] = imag(x(n))
     */
    double *mData;

};

} // namespace dragonboard
#endif
