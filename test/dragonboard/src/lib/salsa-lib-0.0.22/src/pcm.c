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

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "pcm.h"
#include "control.h"
#include "local.h"

/*
 */
static int snd_pcm_hw_mmap_status(snd_pcm_t *pcm);
static void snd_pcm_hw_munmap_status(snd_pcm_t *pcm);

/*
 * open/close
 */

/* open the substream with the given subdevice number */
static int open_with_subdev(const char *filename, int fmode,
			    int card, int subdev)
{
	snd_pcm_info_t info;
	snd_ctl_t *ctl;
	int err, counts, fd;

	err = _snd_ctl_hw_open(&ctl, card);
	if (err < 0)
		return err;

	err = snd_ctl_pcm_prefer_subdevice(ctl, subdev);
	if (err < 0)
		return err;

	for (counts = 0; counts < 3; counts++) {
		fd = open(filename, fmode);
		if (fd < 0)
		        return -errno;
		memset(&info, 0, sizeof(info));
		if (ioctl(fd, SNDRV_PCM_IOCTL_INFO, &info) >= 0 &&
		    info.subdevice == subdev) {
			snd_ctl_close(ctl);
			return fd;
		}
		close(fd);
	}
	snd_ctl_close(ctl);
	return -EBUSY;
}

int snd_pcm_open(snd_pcm_t **pcmp, const char *name,
		 snd_pcm_stream_t stream, int mode)
{
	char filename[32];
	int card, dev, subdev;
	int fd, err, fmode, ver;
	snd_pcm_t *pcm = NULL;

	*pcmp = NULL;

	err = _snd_dev_get_device(name, &card, &dev, &subdev);
	if (err < 0)
		return err;

	snprintf(filename, sizeof(filename), "%s/pcmC%dD%d%c",
		 SALSA_DEVPATH, card, dev,
		 (stream == SND_PCM_STREAM_PLAYBACK ? 'p' : 'c'));
	fmode = O_RDWR | O_NONBLOCK;
	if (mode & SND_PCM_ASYNC)
		fmode |= O_ASYNC;

	if (subdev >= 0) {
		fd = open_with_subdev(filename, fmode, card, subdev);
		if (fd < 0)
			return fd;
	} else {
		fd = open(filename, fmode);
		if (fd < 0)
			return -errno;
	}
	if (!(mode & SND_PCM_NONBLOCK)) {
		fmode &= ~O_NONBLOCK;
		fcntl(fd, F_SETFL, fmode);
	}

	if (ioctl(fd, SNDRV_PCM_IOCTL_PVERSION, &ver) < 0) {
		err = -errno;
		goto error;
	}
	if (subdev < 0) {
		snd_pcm_info_t info;
		memset(&info, 0, sizeof(info));
		if (ioctl(fd, SNDRV_PCM_IOCTL_INFO, &info) < 0) {
			err = -errno;
			goto error;
		}
		subdev = info.subdevice;
	}

	pcm = calloc(1, sizeof(*pcm));
	if (!pcm) {
		err = -ENOMEM;
		goto error;
	}

	pcm->card = card;
	pcm->stream = stream;
	pcm->device = dev;
	pcm->subdevice = subdev;
	pcm->protocol = ver;
	pcm->fd = fd;
	pcm->pollfd.fd = fd;
	pcm->pollfd.events =
		(stream == SND_PCM_STREAM_PLAYBACK ? POLLOUT : POLLIN)
		| POLLERR | POLLNVAL;

	err = snd_pcm_hw_mmap_status(pcm);
	if (err < 0)
		goto error;

	*pcmp = pcm;
	return 0;

 error:
	free(pcm);
	if (fd >= 0)
		close(fd);
	return err;
}

int snd_pcm_close(snd_pcm_t *pcm)
{
	if (pcm->setup) {
		snd_pcm_drop(pcm);
		snd_pcm_hw_free(pcm);
	}
	_snd_pcm_munmap(pcm);
	snd_pcm_hw_munmap_status(pcm);
#if SALSA_HAS_ASYNC_SUPPORT
	if (pcm->async)
		snd_async_del_handler(pcm->async);
#endif
	close(pcm->fd);
	free(pcm);
	return 0;
}


/*
 * READ/WRITE
 */

static int correct_pcm_error(snd_pcm_t *pcm, int err)
{
	switch (snd_pcm_state(pcm)) {
	case SND_PCM_STATE_XRUN:
		return -EPIPE;
	case SND_PCM_STATE_SUSPENDED:
		return -ESTRPIPE;
	case SND_PCM_STATE_DISCONNECTED:
		return -ENODEV;
	default:
		return err;
	}
}

static inline int snd_pcm_check_error(snd_pcm_t *pcm, int err)
{
	if (err == -EINTR)
		return correct_pcm_error(pcm, err);
	return err;
}

snd_pcm_sframes_t snd_pcm_writei(snd_pcm_t *pcm, const void *buffer,
				 snd_pcm_uframes_t size)
{
	struct sndrv_xferi xferi;

	xferi.buf = (char*) buffer;
	xferi.frames = size;
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_WRITEI_FRAMES, &xferi) < 0)
		return snd_pcm_check_error(pcm, -errno);
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	return xferi.result;
}

snd_pcm_sframes_t snd_pcm_writen(snd_pcm_t *pcm, void **bufs,
				 snd_pcm_uframes_t size)
{
	struct sndrv_xfern xfern;

	xfern.bufs = bufs;
	xfern.frames = size;
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_WRITEN_FRAMES, &xfern) < 0)
		return snd_pcm_check_error(pcm, -errno);
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	return xfern.result;
}

snd_pcm_sframes_t snd_pcm_readi(snd_pcm_t *pcm, void *buffer,
				snd_pcm_uframes_t size)
{
	struct sndrv_xferi xferi;

	xferi.buf = buffer;
	xferi.frames = size;
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_READI_FRAMES, &xferi) < 0)
		return snd_pcm_check_error(pcm, -errno);
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	return xferi.result;
}

snd_pcm_sframes_t snd_pcm_readn(snd_pcm_t *pcm, void **bufs,
				snd_pcm_uframes_t size)
{
	struct sndrv_xfern xfern;

	xfern.bufs = bufs;
	xfern.frames = size;
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_READN_FRAMES, &xfern) < 0)
		return snd_pcm_check_error(pcm, -errno);
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	return xfern.result;
}


/*
 * STRINGS, DUMP
 */
#define PCMTYPE(v) [SND_PCM_TYPE_##v] = #v
#define STATE(v) [SND_PCM_STATE_##v] = #v
#define STREAM(v) [SND_PCM_STREAM_##v] = #v
#define READY(v) [SND_PCM_READY_##v] = #v
#define XRUN(v) [SND_PCM_XRUN_##v] = #v
#define SILENCE(v) [SND_PCM_SILENCE_##v] = #v
#define TSTAMP(v) [SND_PCM_TSTAMP_##v] = #v
#define ACCESS(v) [SND_PCM_ACCESS_##v] = #v
#define START(v) [SND_PCM_START_##v] = #v
#define HW_PARAM(v) [SND_PCM_HW_PARAM_##v] = #v
#define SW_PARAM(v) [SND_PCM_SW_PARAM_##v] = #v
#define FORMAT(v) [SND_PCM_FORMAT_##v] = #v
#define SUBFORMAT(v) [SND_PCM_SUBFORMAT_##v] = #v

#define FORMATD(v, d) [SND_PCM_FORMAT_##v] = d
#define SUBFORMATD(v, d) [SND_PCM_SUBFORMAT_##v] = d


const char *_snd_pcm_stream_names[] = {
	STREAM(PLAYBACK),
	STREAM(CAPTURE),
};

const char *_snd_pcm_state_names[] = {
	STATE(OPEN),
	STATE(SETUP),
	STATE(PREPARED),
	STATE(RUNNING),
	STATE(XRUN),
	STATE(DRAINING),
	STATE(PAUSED),
	STATE(SUSPENDED),
	STATE(DISCONNECTED),
};

const char *_snd_pcm_access_names[SND_MASK_MAX + 1] = {
	ACCESS(MMAP_INTERLEAVED),
	ACCESS(MMAP_NONINTERLEAVED),
	ACCESS(MMAP_COMPLEX),
	ACCESS(RW_INTERLEAVED),
	ACCESS(RW_NONINTERLEAVED),
};

const char *_snd_pcm_format_names[SND_MASK_MAX + 1] = {
	FORMAT(S8),
	FORMAT(U8),
	FORMAT(S16_LE),
	FORMAT(S16_BE),
	FORMAT(U16_LE),
	FORMAT(U16_BE),
	FORMAT(S24_LE),
	FORMAT(S24_BE),
	FORMAT(U24_LE),
	FORMAT(U24_BE),
	FORMAT(S32_LE),
	FORMAT(S32_BE),
	FORMAT(U32_LE),
	FORMAT(U32_BE),
	FORMAT(FLOAT_LE),
	FORMAT(FLOAT_BE),
	FORMAT(FLOAT64_LE),
	FORMAT(FLOAT64_BE),
	FORMAT(IEC958_SUBFRAME_LE),
	FORMAT(IEC958_SUBFRAME_BE),
	FORMAT(MU_LAW),
	FORMAT(A_LAW),
	FORMAT(IMA_ADPCM),
	FORMAT(MPEG),
	FORMAT(GSM),
	FORMAT(SPECIAL),
	FORMAT(S24_3LE),
	FORMAT(S24_3BE),
	FORMAT(U24_3LE),
	FORMAT(U24_3BE),
	FORMAT(S20_3LE),
	FORMAT(S20_3BE),
	FORMAT(U20_3LE),
	FORMAT(U20_3BE),
	FORMAT(S18_3LE),
	FORMAT(S18_3BE),
	FORMAT(U18_3LE),
	FORMAT(U18_3BE),
};

static const char *_snd_pcm_format_aliases[SND_MASK_MAX + 1] = {
	FORMAT(S16),
	FORMAT(U16),
	FORMAT(S24),
	FORMAT(U24),
	FORMAT(S32),
	FORMAT(U32),
	FORMAT(FLOAT),
	FORMAT(FLOAT64),
	FORMAT(IEC958_SUBFRAME),
};

const char *_snd_pcm_format_descriptions[SND_MASK_MAX + 1] = {
	FORMATD(S8, "Signed 8 bit"),
	FORMATD(U8, "Unsigned 8 bit"),
	FORMATD(S16_LE, "Signed 16 bit Little Endian"),
	FORMATD(S16_BE, "Signed 16 bit Big Endian"),
	FORMATD(U16_LE, "Unsigned 16 bit Little Endian"),
	FORMATD(U16_BE, "Unsigned 16 bit Big Endian"),
	FORMATD(S24_LE, "Signed 24 bit Little Endian"),
	FORMATD(S24_BE, "Signed 24 bit Big Endian"),
	FORMATD(U24_LE, "Unsigned 24 bit Little Endian"),
	FORMATD(U24_BE, "Unsigned 24 bit Big Endian"),
	FORMATD(S32_LE, "Signed 32 bit Little Endian"),
	FORMATD(S32_BE, "Signed 32 bit Big Endian"),
	FORMATD(U32_LE, "Unsigned 32 bit Little Endian"),
	FORMATD(U32_BE, "Unsigned 32 bit Big Endian"),
	FORMATD(FLOAT_LE, "Float 32 bit Little Endian"),
	FORMATD(FLOAT_BE, "Float 32 bit Big Endian"),
	FORMATD(FLOAT64_LE, "Float 64 bit Little Endian"),
	FORMATD(FLOAT64_BE, "Float 64 bit Big Endian"),
	FORMATD(IEC958_SUBFRAME_LE, "IEC-958 Little Endian"),
	FORMATD(IEC958_SUBFRAME_BE, "IEC-958 Big Endian"),
	FORMATD(MU_LAW, "Mu-Law"),
	FORMATD(A_LAW, "A-Law"),
	FORMATD(IMA_ADPCM, "Ima-ADPCM"),
	FORMATD(MPEG, "MPEG"),
	FORMATD(GSM, "GSM"),
	FORMATD(SPECIAL, "Special"),
	FORMATD(S24_3LE, "Signed 24 bit Little Endian in 3bytes"),
	FORMATD(S24_3BE, "Signed 24 bit Big Endian in 3bytes"),
	FORMATD(U24_3LE, "Unsigned 24 bit Little Endian in 3bytes"),
	FORMATD(U24_3BE, "Unsigned 24 bit Big Endian in 3bytes"),
	FORMATD(S20_3LE, "Signed 20 bit Little Endian in 3bytes"),
	FORMATD(S20_3BE, "Signed 20 bit Big Endian in 3bytes"),
	FORMATD(U20_3LE, "Unsigned 20 bit Little Endian in 3bytes"),
	FORMATD(U20_3BE, "Unsigned 20 bit Big Endian in 3bytes"),
	FORMATD(S18_3LE, "Signed 18 bit Little Endian in 3bytes"),
	FORMATD(S18_3BE, "Signed 18 bit Big Endian in 3bytes"),
	FORMATD(U18_3LE, "Unsigned 18 bit Little Endian in 3bytes"),
	FORMATD(U18_3BE, "Unsigned 18 bit Big Endian in 3bytes"),
};

const char *_snd_pcm_type_names[] = {
	PCMTYPE(HW),
	PCMTYPE(HOOKS),
	PCMTYPE(MULTI),
	PCMTYPE(FILE),
	PCMTYPE(NULL),
	PCMTYPE(SHM),
	PCMTYPE(INET),
	PCMTYPE(COPY),
	PCMTYPE(LINEAR),
	PCMTYPE(ALAW),
	PCMTYPE(MULAW),
	PCMTYPE(ADPCM),
	PCMTYPE(RATE),
	PCMTYPE(ROUTE),
	PCMTYPE(PLUG),
	PCMTYPE(SHARE),
	PCMTYPE(METER),
	PCMTYPE(MIX),
	PCMTYPE(DROUTE),
	PCMTYPE(LBSERVER),
	PCMTYPE(LINEAR_FLOAT),
	PCMTYPE(LADSPA),
	PCMTYPE(DMIX),
	PCMTYPE(JACK),
        PCMTYPE(DSNOOP),
        PCMTYPE(IEC958),
	PCMTYPE(SOFTVOL),
        PCMTYPE(IOPLUG),
        PCMTYPE(EXTPLUG),
};

const char *_snd_pcm_subformat_names[SND_MASK_MAX + 1] = {
	SUBFORMAT(STD),
};

const char *_snd_pcm_subformat_descriptions[SND_MASK_MAX + 1] = {
	SUBFORMATD(STD, "Standard"),
};

const char *_snd_pcm_tstamp_mode_names[] = {
	TSTAMP(NONE),
	TSTAMP(MMAP),
};

snd_pcm_format_t snd_pcm_format_value(const char* name)
{
	snd_pcm_format_t format;
	for (format = 0; format <= SND_PCM_FORMAT_LAST; format++) {
		if (_snd_pcm_format_names[format] &&
		    strcasecmp(name, _snd_pcm_format_names[format]) == 0) {
			return format;
		}
		if (_snd_pcm_format_aliases[format] &&
		    strcasecmp(name, _snd_pcm_format_aliases[format]) == 0) {
			return format;
		}
	}
	for (format = 0; format <= SND_PCM_FORMAT_LAST; format++) {
		if (_snd_pcm_format_descriptions[format] &&
		    strcasecmp(name, _snd_pcm_format_descriptions[format]) == 0) {
			return format;
		}
	}
	return SND_PCM_FORMAT_UNKNOWN;
}


int snd_pcm_dump_hw_setup(snd_pcm_t *pcm, snd_output_t *out)
{
	snd_output_printf(out, "  stream       : %s\n",
			  snd_pcm_stream_name(pcm->stream));
	snd_output_printf(out, "  access       : %s\n",
			  snd_pcm_access_name(pcm->access));
	snd_output_printf(out, "  format       : %s\n",
			  snd_pcm_format_name(pcm->format));
	snd_output_printf(out, "  subformat    : %s\n",
			  snd_pcm_subformat_name(pcm->subformat));
	snd_output_printf(out, "  channels     : %u\n", pcm->channels);
	snd_output_printf(out, "  rate         : %u\n", pcm->rate);
	snd_output_printf(out, "  exact rate   : %g (%u/%u)\n",
			  (pcm->hw_params.rate_den ?
			   ((double) pcm->hw_params.rate_num / pcm->hw_params.rate_den) : 0.0),
			  pcm->hw_params.rate_num, pcm->hw_params.rate_den);
	snd_output_printf(out, "  msbits       : %u\n", pcm->hw_params.msbits);
	snd_output_printf(out, "  buffer_size  : %lu\n", pcm->buffer_size);
	snd_output_printf(out, "  period_size  : %lu\n", pcm->period_size);
	snd_output_printf(out, "  period_time  : %u\n", pcm->period_time);
#if 0 /* deprecated */
	snd_output_printf(out, "  tick_time    : %u\n", pcm->tick_time);
#endif
	return 0;
}

int snd_pcm_dump_sw_setup(snd_pcm_t *pcm, snd_output_t *out)
{
	snd_output_printf(out, "  tstamp_mode  : %s\n",
			  snd_pcm_tstamp_mode_name(pcm->sw_params.tstamp_mode));
	snd_output_printf(out, "  period_step  : %d\n",
			  pcm->sw_params.period_step);
#if 0 /* deprecated */
	snd_output_printf(out, "  sleep_min    : %d\n",
			  pcm->sw_params.sleep_min);
#endif
	snd_output_printf(out, "  avail_min    : %ld\n",
			  pcm->sw_params.avail_min);
#if 0 /* deprecated */
	snd_output_printf(out, "  xfer_align   : %ld\n",
			  pcm->sw_params.xfer_align);
#endif
	snd_output_printf(out, "  start_threshold  : %ld\n",
			  pcm->sw_params.start_threshold);
	snd_output_printf(out, "  stop_threshold   : %ld\n",
			  pcm->sw_params.stop_threshold);
	snd_output_printf(out, "  silence_threshold: %ld\n",
			  pcm->sw_params.silence_threshold);
	snd_output_printf(out, "  silence_size : %ld\n",
			  pcm->sw_params.silence_size);
	snd_output_printf(out, "  boundary     : %ld\n",
			  pcm->sw_params.boundary);
	return 0;
}

int snd_pcm_dump_setup(snd_pcm_t *pcm, snd_output_t *out)
{
	snd_pcm_dump_hw_setup(pcm, out);
	snd_pcm_dump_sw_setup(pcm, out);
	return 0;
}

int snd_pcm_status_dump(snd_pcm_status_t *status, snd_output_t *out)
{
	snd_output_printf(out, "  state       : %s\n",
			  snd_pcm_state_name((snd_pcm_state_t) status->state));
	snd_output_printf(out, "  trigger_time: %ld.%06ld\n",
		status->trigger_tstamp.tv_sec, status->trigger_tstamp.tv_nsec);
	snd_output_printf(out, "  tstamp      : %ld.%06ld\n",
		status->tstamp.tv_sec, status->tstamp.tv_nsec);
	snd_output_printf(out, "  delay       : %ld\n", (long)status->delay);
	snd_output_printf(out, "  avail       : %ld\n", (long)status->avail);
	snd_output_printf(out, "  avail_max   : %ld\n", (long)status->avail_max);
	return 0;
}

int snd_pcm_dump(snd_pcm_t *pcm, snd_output_t *out)
{
	char *name;
	int err = snd_card_get_name(pcm->card, &name);
	if (err < 0)
		return err;
	snd_output_printf(out,
			  "Hardware PCM card %d '%s' device %d subdevice %d\n",
			  pcm->card, name, pcm->device, pcm->subdevice);
	if (pcm->setup) {
		snd_output_printf(out, "Its setup is:\n");
		snd_pcm_dump_setup(pcm, out);
	}
	free(name);
	return 0;
}


/*
 * SILENCE AND COPY AREAS
 */

static inline
void *snd_pcm_channel_area_addr(const snd_pcm_channel_area_t *area,
				snd_pcm_uframes_t offset)
{
	unsigned int bitofs = area->first + area->step * offset;
	return (char *) area->addr + bitofs / 8;
}

static int area_silence_4bit(const snd_pcm_channel_area_t *dst_area,
			     snd_pcm_uframes_t dst_offset,
			     unsigned int samples, snd_pcm_format_t format)
{
	char *dst = snd_pcm_channel_area_addr(dst_area, dst_offset);
	int dst_step = dst_area->step / 8;
	int dstbit = dst_area->first % 8;
	int dstbit_step = dst_area->step % 8;
	while (samples-- > 0) {
		if (dstbit)
			*dst &= 0xf0;
		else
			*dst &= 0x0f;
		dst += dst_step;
		dstbit += dstbit_step;
		if (dstbit == 8) {
			dst++;
			dstbit = 0;
		}
	}
	return 0;
}

int snd_pcm_area_silence(const snd_pcm_channel_area_t *dst_area,
			 snd_pcm_uframes_t dst_offset,
			 unsigned int samples, snd_pcm_format_t format)
{
	char *dst;
	unsigned int dst_step;
	int width;

	if (!dst_area->addr)
		return 0;
	width = snd_pcm_format_physical_width(format);
	if (width < 8)
		return area_silence_4bit(dst_area, dst_offset, samples, format);
	dst = snd_pcm_channel_area_addr(dst_area, dst_offset);
	if (dst_area->step == (unsigned int) width) {
		snd_pcm_format_set_silence(format, dst, samples);
		return 0;
	}
	dst_step = dst_area->step / 8;
	while (samples-- > 0) {
		snd_pcm_format_set_silence(format, dst, 1);
		dst += dst_step;
	}
	return 0;
}

int snd_pcm_areas_silence(const snd_pcm_channel_area_t *dst_areas,
			  snd_pcm_uframes_t dst_offset,
			  unsigned int channels, snd_pcm_uframes_t frames,
			  snd_pcm_format_t format)
{
	while (channels > 0) {
		snd_pcm_area_silence(dst_areas, dst_offset, frames, format);
		dst_areas++;
		channels--;
	}
	return 0;
}

static int area_copy_4bit(const snd_pcm_channel_area_t *dst_area,
			  snd_pcm_uframes_t dst_offset,
			  const snd_pcm_channel_area_t *src_area,
			  snd_pcm_uframes_t src_offset,
			  unsigned int samples, snd_pcm_format_t format)
{
	int srcbit = src_area->first % 8;
	int srcbit_step = src_area->step % 8;
	int dstbit = dst_area->first % 8;
	int dstbit_step = dst_area->step % 8;
	const unsigned char *src = snd_pcm_channel_area_addr(src_area,
							     src_offset);
	unsigned char *dst = snd_pcm_channel_area_addr(dst_area, dst_offset);
	int src_step = src_area->step / 8;
	int dst_step = dst_area->step / 8;

	while (samples-- > 0) {
		unsigned char srcval;
		if (srcbit)
			srcval = *src & 0x0f;
		else
			srcval = *src & 0xf0;
		if (dstbit)
			*dst &= 0xf0;
		else
			*dst &= 0x0f;
		*dst |= srcval;
		src += src_step;
		srcbit += srcbit_step;
		if (srcbit == 8) {
			src++;
			srcbit = 0;
		}
		dst += dst_step;
		dstbit += dstbit_step;
		if (dstbit == 8) {
			dst++;
			dstbit = 0;
		}
	}
	return 0;
}

int snd_pcm_area_copy(const snd_pcm_channel_area_t *dst_area,
		      snd_pcm_uframes_t dst_offset,
		      const snd_pcm_channel_area_t *src_area,
		      snd_pcm_uframes_t src_offset,
		      unsigned int samples, snd_pcm_format_t format)
{
	const char *src;
	char *dst;
	int width;
	int src_step, dst_step;

	if (!dst_area->addr)
		return 0;
	if (dst_area == src_area && dst_offset == src_offset)
		return 0;
	if (!src_area->addr)
		return snd_pcm_area_silence(dst_area, dst_offset, samples,
					    format);
	width = snd_pcm_format_physical_width(format);
	if (width < 8)
		return area_copy_4bit(dst_area, dst_offset,
				      src_area, src_offset,
				      samples, format);
	src = snd_pcm_channel_area_addr(src_area, src_offset);
	dst = snd_pcm_channel_area_addr(dst_area, dst_offset);
	if (src_area->step == (unsigned int) width &&
	    dst_area->step == (unsigned int) width) {
		memcpy(dst, src, samples * width / 8);
		return 0;
	}
	src_step = src_area->step / 8;
	dst_step = dst_area->step / 8;
	width /= 8;
	while (samples-- > 0) {
		memcpy(dst, src, width);
		src += src_step;
		dst += dst_step;
	}
	return 0;
}

int snd_pcm_areas_copy(const snd_pcm_channel_area_t *dst_areas,
		       snd_pcm_uframes_t dst_offset,
		       const snd_pcm_channel_area_t *src_areas,
		       snd_pcm_uframes_t src_offset,
		       unsigned int channels, snd_pcm_uframes_t frames,
		       snd_pcm_format_t format)
{
	int width;
	if (!channels)
		return -EINVAL;
	if (!frames)
		return -EINVAL;
	width = snd_pcm_format_physical_width(format);
	while (channels > 0) {
		snd_pcm_area_copy(dst_areas, dst_offset,
				  src_areas, src_offset,
				  frames, format);
		src_areas++;
		dst_areas++;
		channels--;
	}
	return 0;
}


/*
 * MMAP
 */

static size_t page_align(size_t size)
{
	size_t r;
	long psz = sysconf(_SC_PAGE_SIZE);
	r = size % psz;
	if (r)
		return size + psz - r;
	return size;
}

int _snd_pcm_mmap(snd_pcm_t *pcm)
{
	unsigned int c;

	/* clear first */
	_snd_pcm_munmap(pcm);

	pcm->mmap_channels = calloc(pcm->channels, sizeof(*pcm->mmap_channels));
	if (!pcm->mmap_channels)
		return -ENOMEM;
	pcm->running_areas = calloc(pcm->channels, sizeof(*pcm->running_areas));
	if (!pcm->running_areas) {
		free(pcm->mmap_channels);
		pcm->mmap_channels = NULL;
		return -ENOMEM;
	}
	for (c = 0; c < pcm->channels; ++c) {
		snd_pcm_channel_info_t *i = &pcm->mmap_channels[c];
		i->info.channel = c;
		if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_CHANNEL_INFO, &i->info) < 0)
			return -errno;
	}
	for (c = 0; c < pcm->channels; ++c) {
		snd_pcm_channel_info_t *i = &pcm->mmap_channels[c];
		snd_pcm_channel_area_t *a = &pcm->running_areas[c];
		char *ptr;
		size_t size;
		unsigned int c1;
		if (i->addr)
			goto copy;
                size = i->info.first + i->info.step * (pcm->buffer_size - 1) +
			pcm->sample_bits;
		for (c1 = c + 1; c1 < pcm->channels; ++c1) {
			snd_pcm_channel_info_t *i1 = &pcm->mmap_channels[c1];
			size_t s;
			if (i1->info.offset != i->info.offset)
				continue;
			s = i1->info.first + i1->info.step *
				(pcm->buffer_size - 1) + pcm->sample_bits;
			if (s > size)
				size = s;
		}
		size = (size + 7) / 8;
		size = page_align(size);
		ptr = mmap(NULL, size, PROT_READ|PROT_WRITE,
			   MAP_FILE|MAP_SHARED, pcm->fd, i->info.offset);
		if (ptr == MAP_FAILED)
			return -errno;
		i->addr = ptr;
		for (c1 = c + 1; c1 < pcm->channels; ++c1) {
			snd_pcm_channel_info_t *i1 = &pcm->mmap_channels[c1];
			if (i1->info.offset != i->info.offset)
				continue;
			i1->addr = i->addr;
		}
	copy:
		a->addr = i->addr;
		a->first = i->info.first;
		a->step = i->info.step;
	}
	return 0;
}

int _snd_pcm_munmap(snd_pcm_t *pcm)
{
	int err;
	unsigned int c;

	if (!pcm->mmap_channels)
		return 0;

	for (c = 0; c < pcm->channels; ++c) {
		snd_pcm_channel_info_t *i = &pcm->mmap_channels[c];
		unsigned int c1;
		size_t size;
		if (!i->addr)
			continue;
		size = i->info.first + i->info.step *
			(pcm->buffer_size - 1) + pcm->sample_bits;
		for (c1 = c + 1; c1 < pcm->channels; ++c1) {
			snd_pcm_channel_info_t *i1 = &pcm->mmap_channels[c1];
			size_t s;
			if (i1->addr != i->addr)
				continue;
			i1->addr = NULL;
			s = i1->info.first + i1->info.step *
				(pcm->buffer_size - 1) + pcm->sample_bits;
			if (s > size)
				size = s;
		}
		size = (size + 7) / 8;
		size = page_align(size);
		err = munmap(i->addr, size);
		if (err < 0)
			return -errno;
		i->addr = NULL;
	}
	free(pcm->mmap_channels);
	free(pcm->running_areas);
	pcm->mmap_channels = NULL;
	pcm->running_areas = NULL;
	return 0;
}

static int snd_pcm_hw_mmap_status(snd_pcm_t *pcm)
{
	if (pcm->sync_ptr)
		return 0;

	pcm->mmap_status =
		mmap(NULL, page_align(sizeof(struct sndrv_pcm_mmap_status)),
		     PROT_READ, MAP_FILE|MAP_SHARED,
		     pcm->fd, SNDRV_PCM_MMAP_OFFSET_STATUS);
	if (pcm->mmap_status == MAP_FAILED)
		pcm->mmap_status = NULL;
	if (!pcm->mmap_status)
		goto no_mmap;
	pcm->mmap_control =
		mmap(NULL, page_align(sizeof(struct sndrv_pcm_mmap_control)),
		     PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED,
		     pcm->fd, SNDRV_PCM_MMAP_OFFSET_CONTROL);
	if (pcm->mmap_control == MAP_FAILED)
		pcm->mmap_control = NULL;
	if (!pcm->mmap_control) {
		munmap(pcm->mmap_status,
		       page_align(sizeof(struct sndrv_pcm_mmap_status)));
		pcm->mmap_status = NULL;
		goto no_mmap;
	}
	pcm->mmap_control->avail_min = 1;
	return 0;

 no_mmap:
	pcm->sync_ptr = calloc(1, sizeof(*pcm->sync_ptr));
	if (!pcm->sync_ptr)
		return -ENOMEM;
	pcm->mmap_status = &pcm->sync_ptr->s.status;
	pcm->mmap_control = &pcm->sync_ptr->c.control;
	pcm->mmap_control->avail_min = 1;
	_snd_pcm_sync_ptr(pcm, 0);
	return 0;
}

static void snd_pcm_hw_munmap_status(snd_pcm_t *pcm)
{
	if (pcm->sync_ptr) {
		free(pcm->sync_ptr);
		pcm->sync_ptr = NULL;
	} else {
		munmap(pcm->mmap_status,
		       page_align(sizeof(struct sndrv_pcm_mmap_status)));
		munmap(pcm->mmap_control,
		       page_align(sizeof(struct sndrv_pcm_mmap_control)));
	}
	pcm->mmap_status = NULL;
	pcm->mmap_control = NULL;
}

#define get_hw_ptr(pcm)		((pcm)->mmap_status->hw_ptr)
#define get_appl_ptr(pcm)	((pcm)->mmap_control->appl_ptr)

static snd_pcm_uframes_t snd_pcm_mmap_playback_avail(snd_pcm_t *pcm)
{
	snd_pcm_sframes_t avail;
	avail = get_hw_ptr(pcm) + pcm->buffer_size - get_appl_ptr(pcm);
	if (avail < 0)
		avail += pcm->boundary;
	else if ((snd_pcm_uframes_t) avail >= pcm->boundary)
		avail -= pcm->boundary;
	return avail;
}

static snd_pcm_uframes_t snd_pcm_mmap_capture_avail(snd_pcm_t *pcm)
{
	snd_pcm_sframes_t avail;
	avail = get_hw_ptr(pcm) - get_appl_ptr(pcm);
	if (avail < 0)
		avail += pcm->boundary;
	return avail;
}

static snd_pcm_uframes_t snd_pcm_mmap_avail(snd_pcm_t *pcm)
{
	if (pcm->stream == SND_PCM_STREAM_PLAYBACK)
		return snd_pcm_mmap_playback_avail(pcm);
	else
		return snd_pcm_mmap_capture_avail(pcm);
}

snd_pcm_sframes_t snd_pcm_avail_update(snd_pcm_t *pcm)
{
	snd_pcm_uframes_t avail;

	_snd_pcm_sync_ptr(pcm, 0);
	avail = snd_pcm_mmap_avail(pcm);
	switch (snd_pcm_state(pcm)) {
	case SNDRV_PCM_STATE_RUNNING:
		if (avail >= pcm->sw_params.stop_threshold) {
			if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_XRUN) < 0)
				return -errno;
			/* everything is ok,
			 * state == SND_PCM_STATE_XRUN at the moment
			 */
			return -EPIPE;
		}
		break;
	case SNDRV_PCM_STATE_XRUN:
		return -EPIPE;
	default:
		break;
	}
	return avail;
}

int snd_pcm_avail_delay(snd_pcm_t *pcm,
			snd_pcm_sframes_t *availp,
			snd_pcm_sframes_t *delayp)
{
	int err;
	snd_pcm_sframes_t val;

	err = snd_pcm_delay(pcm, delayp);
	if (err < 0)
		return err;
	val = snd_pcm_avail_update(pcm);
	if (val < 0)
		return (int)val;
	*availp = val;
	return 0;
}

int snd_pcm_mmap_begin(snd_pcm_t *pcm,
		       const snd_pcm_channel_area_t **areas,
		       snd_pcm_uframes_t *offset,
		       snd_pcm_uframes_t *frames)
{
	snd_pcm_uframes_t cont;
	snd_pcm_uframes_t f;
	snd_pcm_uframes_t avail;

	*areas = pcm->running_areas;
	*offset = pcm->mmap_control->appl_ptr % pcm->buffer_size;
	avail = snd_pcm_mmap_avail(pcm);
	if (avail > pcm->buffer_size)
		avail = pcm->buffer_size;
	cont = pcm->buffer_size - *offset;
	f = *frames;
	if (f > avail)
		f = avail;
	if (f > cont)
		f = cont;
	*frames = f;
	return 0;
}

snd_pcm_sframes_t snd_pcm_mmap_commit(snd_pcm_t *pcm,
				      snd_pcm_uframes_t offset,
				      snd_pcm_uframes_t frames)
{
	snd_pcm_uframes_t appl_ptr = pcm->mmap_control->appl_ptr;
	appl_ptr += frames;
	if (appl_ptr >= pcm->boundary)
		appl_ptr -= pcm->boundary;
	pcm->mmap_control->appl_ptr = appl_ptr;
	_snd_pcm_sync_ptr(pcm, 0);
	return frames;
}


#if SALSA_HAS_ASYNC_SUPPORT
/*
 * async handler
 */
int snd_async_add_pcm_handler(snd_async_handler_t **handlerp, snd_pcm_t *pcm,
			      snd_async_callback_t callback,
			      void *private_data)
{
	int err;

	if (pcm->async)
		return -EBUSY;
	err = snd_async_add_handler(&pcm->async, pcm->fd,
				    callback, private_data);
	if (err < 0)
		return err;
	pcm->async->rec = pcm;
	pcm->async->pointer = &pcm->async;
	return 0;
}
#endif /* SALSA_HAS_ASYNC_SUPPORT */

/*
 * HELPERS
 */

int snd_pcm_wait(snd_pcm_t *pcm, int timeout)
{
	struct pollfd pfd;
	int err;

#if 0 /* FIXME: NEEDED? */
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	if (snd_pcm_mmap_avail(pcm) >= pcm->sw_params.avail_min)
		return correct_pcm_error(pcm, 1);
#endif

	pfd = pcm->pollfd;
	for (;;) {
		err = poll(&pfd, 1, timeout);
		if (err < 0) {
			if (errno == EINTR)
				continue;
			return -errno;
                }
		if (!err)
			return 0;
		if (pfd.revents & (POLLERR | POLLNVAL))
			return correct_pcm_error(pcm, -EIO);
		if (pfd.revents & (POLLIN | POLLOUT))
			return 1;
	}
}

int snd_pcm_recover(snd_pcm_t *pcm, int err, int silent)
{
        if (err > 0)
                err = -err;
        if (err == -EINTR)	/* nothing to do, continue */
                return 0;
        if (err == -EPIPE) {
                err = snd_pcm_prepare(pcm);
                if (err < 0)
                        return err;
                return 0;
        }
        if (err == -ESTRPIPE) {
                while ((err = snd_pcm_resume(pcm)) == -EAGAIN)
                        /* wait until suspend flag is released */
                        poll(NULL, 0, 1000);
                if (err < 0) {
                        err = snd_pcm_prepare(pcm);
                        if (err < 0)
                                return err;
                }
                return 0;
        }
        return err;
}

