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
#define LOG_TAG "FFT"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

#include "common.h"
#include "FFT.h"

using namespace dragonboard;

#define TWOPI (2.0*PI)
/*
    FFT/IFFT. (see pages 507-508 of Numerical Recipes in C)
    Inputs:
    data[] : array of complex* data points of size 2*NFFT+1.
    data[0] is unused,
    the n'th complex number x(n), for 0 <= n <= length(x)-1, is stored as:
    data[2*n+1] = real(x(n))
    data[2*n+2] = imag(x(n))
    if length(Nx) < NFFT, the remainder of the array must be padded with zeros
    nn : FFT order NFFT. This MUST be a power of 2 and >= length(x).
    isign: if set to 1,
    computes the forward FFT
    if set to -1,
    computes Inverse FFT - in this case the output values have
    to be manually normalized by multiplying with 1/NFFT.
    Outputs:
    data[] : The FFT or IFFT results are stored in data, overwriting the input.
*/
void fourier(double data[], int nn, int isign)
{
    int n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;
    n = nn << 1;
    j = 1;

    /* sort input data */
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            tempr = data[j]; data[j] = data[i]; data[i] = tempr;
            tempr = data[j+1]; data[j+1] = data[i+1]; data[i+1] = tempr;
        }

        m = n >> 1;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }

    mmax = 2;
    while (n > mmax) {
        istep = 2*mmax;
        theta = TWOPI/(isign*mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;

        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j =i + mmax;
                tempr = wr*data[j] - wi*data[j+1];
                tempi = wr*data[j+1] + wi*data[j];
                data[j] = data[i] - tempr;
                data[j+1] = data[i+1] - tempi;
                data[i] += tempr;
                data[i+1] += tempi;
            }
            wr = (wtemp = wr)*wpr - wi*wpi + wr;
            wi = wi*wpr + wtemp*wpi + wi;
        }
        mmax = istep;
    }
}

FFT::FFT():
mState(STATE_UNINIT), mFs(0), mNum(0), mData(NULL)
{
}

bool FFT::init(const short *data, int num, int fs)
{
    mNum = num;
    mFs = fs;

    mData = new double[num * 2 + 1];
    if (!mData) {
        ALOGE("line(%d): can't new mData.", __LINE__);
        return false;
    }

    memset(mData, 0, sizeof(double) * (num * 2 + 1));
    /* init mData real part */
    for (int i = 0; i < num; i++) {
        mData[1 + 2*i] = data[i];
    }

    ALOGV("line(%d): init: mNum:%d, mFs:%d", __LINE__, mNum, mFs);
    mState = STATE_INITED;
    return true;
}

bool FFT::init(const double *data, int num, int fs)
{
    mNum = num;
    mFs = fs;

    mData = new double[num * 2 + 1];
    if (!mData) {
        ALOGE("line(%d): can't new mData.", __LINE__);
        return false;
    }

    memset(mData, 0, sizeof(double) * (num * 2 + 1));

    /* init mData real part */
    for (int i = 0; i < num; i++) {
        mData[1 + 2*i] = data[i];
    }

    ALOGV("line(%d): init: mNum:%d, mFs:%d", __LINE__, mNum, mFs);
    mState = STATE_INITED;
    return true;
}

bool FFT::exit()
{
    if (mData) {
        delete mData;
        mData = NULL;
    }

    mState = STATE_UNINIT;
    return true;
}

bool FFT::doFft()
{
    if (mState != STATE_INITED) {
        ALOGE("line(%d): can't do fft(mState:%d).", __LINE__, mState);
        return false;
    }

    mState = STATE_FFT_DOING;
    fourier(mData, mNum, 1);
    mState = STATE_FFT_DONE;

    return true;
}

bool FFT::getMaxFre(double *fre, double *maxVal)
{
    int index = 0;
    double max = 0;
    double val = 0;

    if (mState != STATE_FFT_DONE) {
        ALOGE("line(%d): can't getMaxFre", __LINE__);
        *maxVal = 0.0;
        *fre = 0.0;
        return false;
    }

    ALOGV("%s,line(%d): mNum:%d, mFs:%d", __func__, __LINE__, mNum, mFs);

    /* get max fre index, skip 10 index*/
    for (int i = 10; i < mNum/2; i++) {
        val = sqrt(mData[1+2*i] * mData[1+2*i]  + mData[2+2*i] * mData[2+2*i]);
        if (max < val) {
            max = val;
            index = i;
        }
    }

    *maxVal = max;
    *fre = (double)index * mNum / (2*mFs);
    ALOGV("%s: fre=%f, max=%f", __func__, *fre, *maxVal);

    return true;
}
