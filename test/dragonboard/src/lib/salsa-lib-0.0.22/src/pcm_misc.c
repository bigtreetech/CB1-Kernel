/*
 *  SALSA-Lib - PCM Interface - misc routines
 *
 *  Copyright (c) 2007 by Takashi Iwai <tiwai@suse.de>
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 2.1 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <byteswap.h>
#include "pcm.h"


/* NOTE: "signed" prefix must be given below since the default char is
 *       unsigned on some architectures!
 */
struct pcm_format_data {
	unsigned char width;	/* bit width */
	unsigned char phys;	/* physical bit width */
	signed char le;	/* 0 = big-endian, 1 = little-endian, -1 = others */
	signed char signd;	/* 0 = unsigned, 1 = signed, -1 = others */
	unsigned char silence[8];	/* silence data to fill */
};

static struct pcm_format_data pcm_formats[SND_PCM_FORMAT_LAST+1] = {
	[SND_PCM_FORMAT_S8] = {
		.width = 8, .phys = 8, .le = -EINVAL, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_U8] = {
		.width = 8, .phys = 8, .le = -EINVAL, .signd = 0,
		.silence = { 0x80 },
	},
	[SND_PCM_FORMAT_S16_LE] = {
		.width = 16, .phys = 16, .le = 1, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_S16_BE] = {
		.width = 16, .phys = 16, .le = 0, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_U16_LE] = {
		.width = 16, .phys = 16, .le = 1, .signd = 0,
		.silence = { 0x00, 0x80 },
	},
	[SND_PCM_FORMAT_U16_BE] = {
		.width = 16, .phys = 16, .le = 0, .signd = 0,
		.silence = { 0x80, 0x00 },
	},
	[SND_PCM_FORMAT_S24_LE] = {
		.width = 24, .phys = 32, .le = 1, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_S24_BE] = {
		.width = 24, .phys = 32, .le = 0, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_U24_LE] = {
		.width = 24, .phys = 32, .le = 1, .signd = 0,
		.silence = { 0x00, 0x00, 0x80, 0x00 },
	},
	[SND_PCM_FORMAT_U24_BE] = {
		.width = 24, .phys = 32, .le = 0, .signd = 0,
		.silence = { 0x00, 0x80, 0x00, 0x00 },
	},
	[SND_PCM_FORMAT_S32_LE] = {
		.width = 32, .phys = 32, .le = 1, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_S32_BE] = {
		.width = 32, .phys = 32, .le = 0, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_U32_LE] = {
		.width = 32, .phys = 32, .le = 1, .signd = 0,
		.silence = { 0x00, 0x00, 0x00, 0x80 },
	},
	[SND_PCM_FORMAT_U32_BE] = {
		.width = 32, .phys = 32, .le = 0, .signd = 0,
		.silence = { 0x80, 0x00, 0x00, 0x00 },
	},
	[SND_PCM_FORMAT_FLOAT_LE] = {
		.width = 32, .phys = 32, .le = 1, .signd = -EINVAL,
		.silence = {},
	},
	[SND_PCM_FORMAT_FLOAT_BE] = {
		.width = 32, .phys = 32, .le = 0, .signd = -EINVAL,
		.silence = {},
	},
	[SND_PCM_FORMAT_FLOAT64_LE] = {
		.width = 64, .phys = 64, .le = 1, .signd = -EINVAL,
		.silence = {},
	},
	[SND_PCM_FORMAT_FLOAT64_BE] = {
		.width = 64, .phys = 64, .le = 0, .signd = -EINVAL,
		.silence = {},
	},
	[SND_PCM_FORMAT_IEC958_SUBFRAME_LE] = {
		.width = 32, .phys = 32, .le = 1, .signd = -EINVAL,
		.silence = {},
	},
	[SND_PCM_FORMAT_IEC958_SUBFRAME_BE] = {
		.width = 32, .phys = 32, .le = 0, .signd = -EINVAL,
		.silence = {},
	},
	[SND_PCM_FORMAT_MU_LAW] = {
		.width = 8, .phys = 8, .le = -EINVAL, .signd = -EINVAL,
		.silence = { 0x7f },
	},
	[SND_PCM_FORMAT_A_LAW] = {
		.width = 8, .phys = 8, .le = -EINVAL, .signd = -EINVAL,
		.silence = { 0x55 },
	},
	[SND_PCM_FORMAT_IMA_ADPCM] = {
		.width = 4, .phys = 4, .le = -EINVAL, .signd = -EINVAL,
		.silence = {},
	},
	/* FIXME: the following three formats are not defined properly yet */
	[SND_PCM_FORMAT_MPEG] = {
		.le = -EINVAL, .signd = -EINVAL,
	},
	[SND_PCM_FORMAT_GSM] = {
		.le = -EINVAL, .signd = -EINVAL,
	},
	[SND_PCM_FORMAT_SPECIAL] = {
		.le = -EINVAL, .signd = -EINVAL,
	},
	[SND_PCM_FORMAT_S24_3LE] = {
		.width = 24, .phys = 24, .le = 1, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_S24_3BE] = {
		.width = 24, .phys = 24, .le = 0, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_U24_3LE] = {
		.width = 24, .phys = 24, .le = 1, .signd = 0,
		.silence = { 0x00, 0x00, 0x80 },
	},
	[SND_PCM_FORMAT_U24_3BE] = {
		.width = 24, .phys = 24, .le = 0, .signd = 0,
		.silence = { 0x80, 0x00, 0x00 },
	},
	[SND_PCM_FORMAT_S20_3LE] = {
		.width = 20, .phys = 24, .le = 1, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_S20_3BE] = {
		.width = 20, .phys = 24, .le = 0, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_U20_3LE] = {
		.width = 20, .phys = 24, .le = 1, .signd = 0,
		.silence = { 0x00, 0x00, 0x08 },
	},
	[SND_PCM_FORMAT_U20_3BE] = {
		.width = 20, .phys = 24, .le = 0, .signd = 0,
		.silence = { 0x08, 0x00, 0x00 },
	},
	[SND_PCM_FORMAT_S18_3LE] = {
		.width = 18, .phys = 24, .le = 1, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_S18_3BE] = {
		.width = 18, .phys = 24, .le = 0, .signd = 1,
		.silence = {},
	},
	[SND_PCM_FORMAT_U18_3LE] = {
		.width = 18, .phys = 24, .le = 1, .signd = 0,
		.silence = { 0x00, 0x00, 0x02 },
	},
	[SND_PCM_FORMAT_U18_3BE] = {
		.width = 18, .phys = 24, .le = 0, .signd = 0,
		.silence = { 0x02, 0x00, 0x00 },
	},
};


int snd_pcm_format_signed(snd_pcm_format_t format)
{
	if (format < 0 || format > SND_PCM_FORMAT_LAST)
		return -EINVAL;
	return pcm_formats[format].signd;
}

int snd_pcm_format_unsigned(snd_pcm_format_t format)
{
	int val = snd_pcm_format_signed(format);
	if (val < 0)
		return val;
	return !val;
}

int snd_pcm_format_linear(snd_pcm_format_t format)
{
	return snd_pcm_format_signed(format) >= 0;
}

int snd_pcm_format_float(snd_pcm_format_t format)
{
	switch (format) {
	case SND_PCM_FORMAT_FLOAT_LE:
	case SND_PCM_FORMAT_FLOAT_BE:
	case SND_PCM_FORMAT_FLOAT64_LE:
	case SND_PCM_FORMAT_FLOAT64_BE:
		return 1;
	default:
		return 0;
	}
}

int snd_pcm_format_little_endian(snd_pcm_format_t format)
{
	if (format < 0 || format > SND_PCM_FORMAT_LAST)
		return -EINVAL;
	return pcm_formats[format].le;
}

int snd_pcm_format_big_endian(snd_pcm_format_t format)
{
	int val = snd_pcm_format_little_endian(format);
	if (val < 0)
		return val;
	return !val;
}

int snd_pcm_format_cpu_endian(snd_pcm_format_t format)
{
#ifdef SND_LITTLE_ENDIAN
	return snd_pcm_format_little_endian(format);
#else
	return snd_pcm_format_big_endian(format);
#endif
}

int snd_pcm_format_width(snd_pcm_format_t format)
{
	int val;
	if (format < 0 || format > SND_PCM_FORMAT_LAST)
		return -EINVAL;
	if ((val = pcm_formats[format].width) == 0)
		return -EINVAL;
	return val;
}

int snd_pcm_format_physical_width(snd_pcm_format_t format)
{
	int val;
	if (format < 0 || format > SND_PCM_FORMAT_LAST)
		return -EINVAL;
	if ((val = pcm_formats[format].phys) == 0)
		return -EINVAL;
	return val;
}

ssize_t snd_pcm_format_size(snd_pcm_format_t format, size_t samples)
{
	int phys_width = snd_pcm_format_physical_width(format);
	if (phys_width < 0)
		return -EINVAL;
	return samples * phys_width / 8;
}

u_int64_t snd_pcm_format_silence_64(snd_pcm_format_t format)
{
	struct pcm_format_data *fmt;
	int i, p, w;
	u_int64_t silence;

	if (format < 0 || format > SNDRV_PCM_FORMAT_LAST)
		return 0;
	fmt = &pcm_formats[format];
	if (!fmt->phys)
		return 0;
	w = fmt->width / 8;
	p = 0;
	silence = 0;
	while (p < w) {
		for (i = 0; i < w; i++, p++)
			silence = (silence << 8) | fmt->silence[i];
	}
	return silence;
}

u_int32_t snd_pcm_format_silence_32(snd_pcm_format_t format)
{
	return (u_int32_t)snd_pcm_format_silence_64(format);
}

u_int16_t snd_pcm_format_silence_16(snd_pcm_format_t format)
{
	return (u_int16_t)snd_pcm_format_silence_64(format);
}

u_int8_t snd_pcm_format_silence(snd_pcm_format_t format)
{
	return (u_int8_t)snd_pcm_format_silence_64(format);
}

int snd_pcm_format_set_silence(snd_pcm_format_t format, void *data,
			       unsigned int samples)
{
	int width;
	unsigned char *dst, *pat;

	if (format < 0 || format > SND_PCM_FORMAT_LAST)
		return -EINVAL;
	if (samples == 0)
		return 0;
	width = pcm_formats[format].phys; /* physical width */
	pat = pcm_formats[format].silence;
	if (!width)
		return -EINVAL;
	/* signed or 1 byte data */
	if (pcm_formats[format].signd == 1 || width <= 8) {
		unsigned int bytes = samples * width / 8;
		memset(data, *pat, bytes);
		return 0;
	}
	/* non-zero samples, fill using a loop */
	width /= 8;
	dst = data;
	while (samples--) {
		memcpy(dst, pat, width);
		dst += width;
	}
	return 0;
}

static int linear_formats[4][2][2] = {
	{ { SND_PCM_FORMAT_S8, SND_PCM_FORMAT_S8 },
	  { SND_PCM_FORMAT_U8, SND_PCM_FORMAT_U8 } },
	{ { SND_PCM_FORMAT_S16_LE, SND_PCM_FORMAT_S16_BE },
	  { SND_PCM_FORMAT_U16_LE, SND_PCM_FORMAT_U16_BE } },
	{ { SND_PCM_FORMAT_S24_LE, SND_PCM_FORMAT_S24_BE },
	  { SND_PCM_FORMAT_U24_LE, SND_PCM_FORMAT_U24_BE } },
	{ { SND_PCM_FORMAT_S32_LE, SND_PCM_FORMAT_S32_BE },
	  { SND_PCM_FORMAT_U32_LE, SND_PCM_FORMAT_U32_BE } }
};

static int linear24_formats[3][2][2] = {
	{ { SND_PCM_FORMAT_S24_3LE, SND_PCM_FORMAT_S24_3BE },
	  { SND_PCM_FORMAT_U24_3LE, SND_PCM_FORMAT_U24_3BE } },
	{ { SND_PCM_FORMAT_S20_3LE, SND_PCM_FORMAT_S20_3BE },
	  { SND_PCM_FORMAT_U20_3LE, SND_PCM_FORMAT_U20_3BE } },
	{ { SND_PCM_FORMAT_S18_3LE, SND_PCM_FORMAT_S18_3BE },
	  { SND_PCM_FORMAT_U18_3LE, SND_PCM_FORMAT_U18_3BE } },
};

snd_pcm_format_t snd_pcm_build_linear_format(int width, int pwidth,
					     int unsignd, int big_endian)
{
	if (pwidth == 24) {
		switch (width) {
		case 24:
			width = 0;
			break;
		case 20:
			width = 1;
			break;
		case 18:
			width = 2;
			break;
		default:
			return SND_PCM_FORMAT_UNKNOWN;
		}
		return linear24_formats[width][!!unsignd][!!big_endian];
	} else {
		switch (width) {
		case 8:
			width = 0;
			break;
		case 16:
			width = 1;
			break;
		case 24:
			width = 2;
			break;
		case 32:
			width = 3;
			break;
		default:
			return SND_PCM_FORMAT_UNKNOWN;
		}
		return linear_formats[width][!!unsignd][!!big_endian];
	}
}
