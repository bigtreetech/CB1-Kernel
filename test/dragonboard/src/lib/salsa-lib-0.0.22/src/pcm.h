/*
 *  SALSA-Lib - PCM Interface
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

#ifndef __ALSA_PCM_H_INC
#define __ALSA_PCM_H_INC

#include "recipe.h"
#include "global.h"
#include "output.h"
#include "pcm_types.h"

int snd_pcm_open(snd_pcm_t **pcm, const char *name,
		 snd_pcm_stream_t stream, int mode);
int snd_pcm_close(snd_pcm_t *pcm);
int snd_pcm_hw_params(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
int snd_pcm_hw_free(snd_pcm_t *pcm);
int snd_pcm_hw_params_any(snd_pcm_t *pcm, snd_pcm_hw_params_t *params);
int snd_pcm_hw_params_get_min_align(const snd_pcm_hw_params_t *params,
				    snd_pcm_uframes_t *val);
int snd_pcm_sw_params(snd_pcm_t *pcm, snd_pcm_sw_params_t *params);

snd_pcm_sframes_t snd_pcm_avail_update(snd_pcm_t *pcm);
int snd_pcm_avail_delay(snd_pcm_t *pcm, snd_pcm_sframes_t *availp,
			snd_pcm_sframes_t *delayp);
snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm, const void *buffer,
				 snd_pcm_uframes_t size);
snd_pcm_sframes_t snd_pcm_readi(snd_pcm_t *pcm, void *buffer,
				snd_pcm_uframes_t size);
snd_pcm_sframes_t snd_pcm_writen(snd_pcm_t *pcm, void **bufs,
				 snd_pcm_uframes_t size);
snd_pcm_sframes_t snd_pcm_readn(snd_pcm_t *pcm, void **bufs,
				snd_pcm_uframes_t size);
int snd_pcm_wait(snd_pcm_t *pcm, int timeout);

int snd_pcm_recover(snd_pcm_t *pcm, int err, int silent);

int snd_pcm_dump(snd_pcm_t *pcm, snd_output_t *out);
int snd_pcm_dump_hw_setup(snd_pcm_t *pcm, snd_output_t *out);
int snd_pcm_dump_sw_setup(snd_pcm_t *pcm, snd_output_t *out);
int snd_pcm_dump_setup(snd_pcm_t *pcm, snd_output_t *out);
int snd_pcm_hw_params_dump(snd_pcm_hw_params_t *params, snd_output_t *out);
int snd_pcm_sw_params_dump(snd_pcm_sw_params_t *params, snd_output_t *out);
int snd_pcm_status_dump(snd_pcm_status_t *status, snd_output_t *out);

int snd_pcm_mmap_begin(snd_pcm_t *pcm,
		       const snd_pcm_channel_area_t **areas,
		       snd_pcm_uframes_t *offset,
		       snd_pcm_uframes_t *frames);
snd_pcm_sframes_t snd_pcm_mmap_commit(snd_pcm_t *pcm,
				      snd_pcm_uframes_t offset,
				      snd_pcm_uframes_t frames);

int snd_pcm_format_signed(snd_pcm_format_t format);
int snd_pcm_format_unsigned(snd_pcm_format_t format);
int snd_pcm_format_linear(snd_pcm_format_t format);
int snd_pcm_format_float(snd_pcm_format_t format);
int snd_pcm_format_little_endian(snd_pcm_format_t format);
int snd_pcm_format_big_endian(snd_pcm_format_t format);
int snd_pcm_format_cpu_endian(snd_pcm_format_t format);
int snd_pcm_format_width(snd_pcm_format_t format);	/* in bits */
int snd_pcm_format_physical_width(snd_pcm_format_t format);	/* in bits */
snd_pcm_format_t snd_pcm_build_linear_format(int width, int pwidth,
					     int unsignd, int big_endian);
ssize_t snd_pcm_format_size(snd_pcm_format_t format, size_t samples);
uint8_t snd_pcm_format_silence(snd_pcm_format_t format);
uint16_t snd_pcm_format_silence_16(snd_pcm_format_t format);
uint32_t snd_pcm_format_silence_32(snd_pcm_format_t format);
uint64_t snd_pcm_format_silence_64(snd_pcm_format_t format);
int snd_pcm_format_set_silence(snd_pcm_format_t format, void *buf,
			       unsigned int samples);
snd_pcm_format_t snd_pcm_format_value(const char* name);

int snd_pcm_area_silence(const snd_pcm_channel_area_t *dst_channel,
			 snd_pcm_uframes_t dst_offset,
			 unsigned int samples, snd_pcm_format_t format);
int snd_pcm_areas_silence(const snd_pcm_channel_area_t *dst_channels,
			  snd_pcm_uframes_t dst_offset,
			  unsigned int channels, snd_pcm_uframes_t frames,
			  snd_pcm_format_t format);
int snd_pcm_area_copy(const snd_pcm_channel_area_t *dst_channel,
		      snd_pcm_uframes_t dst_offset,
		      const snd_pcm_channel_area_t *src_channel,
		      snd_pcm_uframes_t src_offset,
		      unsigned int samples, snd_pcm_format_t format);
int snd_pcm_areas_copy(const snd_pcm_channel_area_t *dst_channels,
		       snd_pcm_uframes_t dst_offset,
		       const snd_pcm_channel_area_t *src_channels,
		       snd_pcm_uframes_t src_offset,
		       unsigned int channels, snd_pcm_uframes_t frames,
		       snd_pcm_format_t format);

#if SALSA_HAS_ASYNC_SUPPORT
int snd_async_add_pcm_handler(snd_async_handler_t **handler, snd_pcm_t *pcm,
			      snd_async_callback_t callback,
			      void *private_data);
#endif

#include "pcm_macros.h"

#define snd_pcm_info_alloca(ptr)	__snd_alloca(ptr, snd_pcm_info)
#define snd_pcm_hw_params_alloca(ptr)	__snd_alloca(ptr, snd_pcm_hw_params)
#define snd_pcm_sw_params_alloca(ptr)	__snd_alloca(ptr, snd_pcm_sw_params)
#define snd_pcm_access_mask_alloca(ptr)	__snd_alloca(ptr, snd_pcm_access_mask)
#define snd_pcm_format_mask_alloca(ptr)	__snd_alloca(ptr, snd_pcm_format_mask)
#define snd_pcm_subformat_mask_alloca(ptr) \
	__snd_alloca(ptr, snd_pcm_subformat_mask)
#define snd_pcm_status_alloca(ptr)	__snd_alloca(ptr, snd_pcm_status)

#endif /* __ALSA_PCM_H_INC */
