/*
 * PCM privates and macros
 */

#ifndef __ALSA_PCM_MACROS_H
#define __ALSA_PCM_MACROS_H

#include "asound.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

typedef struct sndrv_interval snd_interval_t;
typedef struct {
	struct sndrv_pcm_channel_info info;
	void *addr;
} snd_pcm_channel_info_t;

struct _snd_pcm {
	char *name;
	snd_pcm_type_t type;
	snd_pcm_stream_t stream;
	int mode;
	int setup;

	int card;
	int device;
	int subdevice;
	int protocol;

	int fd;
	struct pollfd pollfd;

	snd_pcm_hw_params_t hw_params;
	snd_pcm_sw_params_t sw_params;

	snd_pcm_access_t access;	/* access mode */
	snd_pcm_format_t format;	/* SND_PCM_FORMAT_* */
	snd_pcm_subformat_t subformat;	/* subformat */
	unsigned int channels;		/* channels */
	unsigned int rate;		/* rate in Hz */
	snd_pcm_uframes_t period_size;
	unsigned int period_time;	/* period duration */
	snd_interval_t periods;
	snd_pcm_uframes_t boundary;	/* pointers wrap point */
	unsigned int info;		/* Info for returned setup */
	unsigned int msbits;		/* used most significant bits */
	unsigned int rate_num;		/* rate numerator */
	unsigned int rate_den;		/* rate denominator */
	unsigned int hw_flags;		/* actual hardware flags */
	snd_pcm_uframes_t fifo_size;	/* chip FIFO size in frames */
	snd_pcm_uframes_t buffer_size;
	snd_interval_t buffer_time;
	unsigned int sample_bits;
	unsigned int frame_bits;
	snd_pcm_uframes_t min_align;

	struct sndrv_pcm_mmap_status *mmap_status;
	struct sndrv_pcm_mmap_control *mmap_control;
	struct sndrv_pcm_sync_ptr *sync_ptr;

	snd_pcm_channel_info_t *mmap_channels;
	snd_pcm_channel_area_t *running_areas;
#if SALSA_HAS_ASYNC_SUPPORT
	snd_async_handler_t *async;
#endif
};

/*
 * Macros
 */

static inline
int snd_pcm_nonblock(snd_pcm_t *pcm, int nonblock)
{
	return _snd_set_nonblock(pcm->fd, nonblock);
}

static inline
const char *snd_pcm_name(snd_pcm_t *pcm)
{
	return pcm->name;
}

static inline
snd_pcm_type_t snd_pcm_type(snd_pcm_t *pcm)
{
	return pcm->type;
}

static inline
snd_pcm_stream_t snd_pcm_stream(snd_pcm_t *pcm)
{
	return pcm->stream;
}

static inline
int snd_pcm_poll_descriptors_count(snd_pcm_t *pcm)
{
	return 1;
}

static inline
int snd_pcm_poll_descriptors(snd_pcm_t *pcm, struct pollfd *pfds,
			     unsigned int space)
{
	*pfds = pcm->pollfd;
	return 1;
}

static inline
int snd_pcm_poll_descriptors_revents(snd_pcm_t *pcm, struct pollfd *pfds,
				     unsigned int nfds,
				     unsigned short *revents)
{
	*revents = pfds->revents;
	return 0;
}

static inline
int snd_pcm_info(snd_pcm_t *pcm, snd_pcm_info_t *info)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_INFO, info) < 0)
		return -errno;
	return 0;
}

static inline
int snd_pcm_hw_params_current(snd_pcm_t *pcm, snd_pcm_hw_params_t *params)
{
	if (!pcm->setup)
		return -EBADFD;
	*params = pcm->hw_params;
	return 0;
}

static inline
int snd_pcm_status(snd_pcm_t *pcm, snd_pcm_status_t *status)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_STATUS, status) < 0)
		return -errno;
	return 0;
}

static inline
int _snd_pcm_sync_ptr(snd_pcm_t *pcm, int flags)
{
	if (pcm->sync_ptr) {
		pcm->sync_ptr->flags = flags;
		if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_SYNC_PTR, pcm->sync_ptr) < 0)
			return -errno;
	}
	return 0;
}


static inline
snd_pcm_state_t snd_pcm_state(snd_pcm_t *pcm)
{
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	return (snd_pcm_state_t) pcm->mmap_status->state;
}

/* for internal use only */
static inline
int _snd_pcm_hwsync(snd_pcm_t *pcm)
{
	if (pcm->sync_ptr)
		return _snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_HWSYNC);
	else if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_HWSYNC) < 0)
		return -errno;
	return 0;
}

/* exported: snd_pcm_hwsync() is now deprecated */
static inline
__attribute__ ((deprecated))
int snd_pcm_hwsync(snd_pcm_t *pcm)
{
	return _snd_pcm_hwsync(pcm);
}

static inline
snd_pcm_sframes_t snd_pcm_avail(snd_pcm_t *pcm)
{
	int err = _snd_pcm_hwsync(pcm);
	if (err < 0)
		return err;
	return snd_pcm_avail_update(pcm);
}

static inline
int snd_pcm_delay(snd_pcm_t *pcm, snd_pcm_sframes_t *delayp)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_DELAY, delayp) < 0)
		return -errno;
	return 0;
}

static inline
int snd_pcm_resume(snd_pcm_t *pcm)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_RESUME) < 0)
		return -errno;
	return 0;
}

static inline
int snd_pcm_prepare(snd_pcm_t *pcm)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_PREPARE) < 0)
		return -errno;
	return _snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
}

static inline
int snd_pcm_reset(snd_pcm_t *pcm)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_RESET) < 0)
		return -errno;
	return _snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
}

static inline
int snd_pcm_start(snd_pcm_t *pcm)
{
	_snd_pcm_sync_ptr(pcm, 0);
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_START) < 0)
		return -errno;
	return 0;
}

static inline
int snd_pcm_drop(snd_pcm_t *pcm)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_DROP) < 0)
		return -errno;
	return 0;
}

static inline
int snd_pcm_drain(snd_pcm_t *pcm)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_DRAIN) < 0)
		return -errno;
	return 0;
}

static inline
int snd_pcm_pause(snd_pcm_t *pcm, int enable)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_PAUSE, enable) < 0)
		return -errno;
	return 0;
}

static inline
snd_pcm_sframes_t snd_pcm_rewind(snd_pcm_t *pcm, snd_pcm_uframes_t frames)
{
	if (frames == 0)
		return 0;
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_REWIND, &frames) < 0)
		return -errno;
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	return frames;
}

static inline
snd_pcm_sframes_t snd_pcm_forward(snd_pcm_t *pcm, snd_pcm_uframes_t frames)
{
	if (frames == 0)
		return 0;
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_FORWARD, &frames) < 0)
		return -errno;
	_snd_pcm_sync_ptr(pcm, SNDRV_PCM_SYNC_PTR_APPL);
	return frames;
}

static inline
int snd_pcm_link(snd_pcm_t *pcm1, snd_pcm_t *pcm2)
{
	if (ioctl(pcm1->fd, SNDRV_PCM_IOCTL_LINK, pcm2->fd) < 0)
		return -errno;
	return 0;
}

static inline
int snd_pcm_unlink(snd_pcm_t *pcm)
{
	if (ioctl(pcm->fd, SNDRV_PCM_IOCTL_UNLINK) < 0)
		return -errno;
	return 0;
}


extern const char *_snd_pcm_stream_names[];
extern const char *_snd_pcm_state_names[];
extern const char *_snd_pcm_access_names[];
extern const char *_snd_pcm_format_names[];
extern const char *_snd_pcm_format_descriptions[];
extern const char *_snd_pcm_type_names[];
extern const char *_snd_pcm_subformat_names[] ;
extern const char *_snd_pcm_subformat_descriptions[];
extern const char *_snd_pcm_tstamp_mode_names[];

#define snd_pcm_stream_name(stream)	_snd_pcm_stream_names[stream]
#define snd_pcm_access_name(acc)	_snd_pcm_access_names[acc]
#define snd_pcm_format_name(format)	_snd_pcm_format_names[format]
#define snd_pcm_format_description(format) _snd_pcm_format_descriptions[format]
#define snd_pcm_subformat_name(subformat) _snd_pcm_subformat_names[subformat]
#define snd_pcm_subformat_description(subformat) \
	_snd_pcm_subformat_descriptions[subformat]
#define snd_pcm_tstamp_mode_name(mode)	_snd_pcm_tstamp_mode_names[mode]
#define snd_pcm_state_name(state)	_snd_pcm_state_names[state]
#define snd_pcm_type_name(type)		_snd_pcm_type_names[type]

static inline
snd_pcm_sframes_t snd_pcm_bytes_to_frames(snd_pcm_t *pcm, ssize_t bytes)
{
	return bytes * 8 / pcm->frame_bits;
}

static inline
ssize_t snd_pcm_frames_to_bytes(snd_pcm_t *pcm, snd_pcm_sframes_t frames)
{
	return frames * pcm->frame_bits / 8;
}

static inline
long snd_pcm_bytes_to_samples(snd_pcm_t *pcm, ssize_t bytes)
{
	return bytes * 8 / pcm->sample_bits;
}

static inline
ssize_t snd_pcm_samples_to_bytes(snd_pcm_t *pcm, long samples)
{
	return samples * pcm->sample_bits / 8;
}

static inline
int snd_pcm_hw_params_can_mmap_sample_resolution(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_MMAP_VALID);
}

static inline
int snd_pcm_hw_params_is_double(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_DOUBLE);
}

static inline
int snd_pcm_hw_params_is_batch(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_BATCH);
}

static inline
int snd_pcm_hw_params_is_block_transfer(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_BLOCK_TRANSFER);
}

static inline
int snd_pcm_hw_params_can_overrange(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_OVERRANGE);
}

static inline
int snd_pcm_hw_params_can_pause(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_PAUSE);
}

static inline
int snd_pcm_hw_params_can_resume(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_RESUME);
}

static inline
int snd_pcm_hw_params_is_half_duplex(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_HALF_DUPLEX);
}

static inline
int snd_pcm_hw_params_is_joint_duplex(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_JOINT_DUPLEX);
}

static inline
int snd_pcm_hw_params_can_sync_start(const snd_pcm_hw_params_t *params)
{
	return !!(params->info & SNDRV_PCM_INFO_SYNC_START);
}

static inline
int snd_pcm_hw_params_get_rate_numden(const snd_pcm_hw_params_t *params,
				      unsigned int *rate_num,
				      unsigned int *rate_den)
{
	*rate_num = params->rate_num;
	*rate_den = params->rate_den;
	return 0;
}

static inline
int snd_pcm_hw_params_get_sbits(const snd_pcm_hw_params_t *params)
{
	return params->msbits;
}

static inline
int snd_pcm_hw_params_get_fifo_size(const snd_pcm_hw_params_t *params)
{
	return params->fifo_size;
}

/*
 */

/*
 */
extern int _snd_pcm_hw_param_get(const snd_pcm_hw_params_t *params, int type,
				 unsigned int *val, int *dir);
extern int _snd_pcm_hw_param_set(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				 int type, unsigned int val, int dir);
extern int _snd_pcm_hw_param_test(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				  int type, unsigned int val, int *dir);
extern snd_mask_t * _snd_pcm_hw_param_get_mask(snd_pcm_hw_params_t *params, int type);
extern int _snd_pcm_hw_param_set_mask(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      int type, const snd_mask_t *mask);
extern int _snd_pcm_hw_param_set_integer(snd_pcm_t *pcm,
					 snd_pcm_hw_params_t *params, int type);
extern int _snd_pcm_hw_param_get_min(const snd_pcm_hw_params_t *params,
				     int type, unsigned int *val, int *dir);
extern int _snd_pcm_hw_param_set_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				     int type, unsigned int *val, int *dir);
extern int _snd_pcm_hw_param_get_max(const snd_pcm_hw_params_t *params,
				     int type, unsigned int *val, int *dir);
extern int _snd_pcm_hw_param_set_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				     int type, unsigned int *val, int *dir);
extern int _snd_pcm_hw_param_set_minmax(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					int type, unsigned int *minval, int *mindir,
					unsigned int *maxval, int *maxdir);
extern int _snd_pcm_hw_param_set_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      int type, unsigned int *val, int *dir);
extern int _snd_pcm_hw_param_set_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       int type, unsigned int *val, int *dir);
extern int _snd_pcm_hw_param_set_last(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      int type, unsigned int *val, int *dir);

#define MASK_OFS(i)	((i) >> 5)
#define MASK_BIT(i)	(1U << ((i) & 31))

static inline
void _snd_mask_none(snd_mask_t *mask)
{
	memset(mask, 0, sizeof(*mask));
}

static inline
void _snd_mask_any(snd_mask_t *mask)
{
	memset(mask, 0xff, sizeof(*mask));
}

static inline
void _snd_mask_set(snd_mask_t *mask, unsigned int val)
{
	mask->bits[MASK_OFS(val)] |= MASK_BIT(val);
}

static inline
void _snd_mask_reset(snd_mask_t *mask, unsigned int val)
{
	mask->bits[MASK_OFS(val)] &= ~MASK_BIT(val);
}

static inline
int _snd_mask_test(const snd_mask_t *mask, unsigned int val)
{
	return mask->bits[MASK_OFS(val)] & MASK_BIT(val);
}

#define SND_MASK_MAX	SNDRV_MASK_MAX

static inline
int _snd_mask_empty(const snd_mask_t *mask)
{
	int i;
	for (i = 0; i < SND_MASK_MAX / 32; i++)
		if (mask->bits[i])
			return 0;
	return 1;
}

#undef MASK_OFS
#undef MASK_BIT


#if __WORDSIZE == 64

#define DEFINE_GET64(name) \
static inline \
int __snd_pcm_hw_param_ ## name ##64\
(const snd_pcm_hw_params_t *params, int type, unsigned long *val, int *dir)\
{\
	unsigned int _val;\
	int err = _snd_pcm_hw_param_ ## name(params, type, &_val, dir);\
	*val = _val;\
	return err;\
}

DEFINE_GET64(get);
DEFINE_GET64(get_min);
DEFINE_GET64(get_max);
#define _snd_pcm_hw_param_get64(params, type, ptr, dir) \
	__snd_pcm_hw_param_get64(params, type, (unsigned long *)(ptr), dir)
#define _snd_pcm_hw_param_get_min64(params, type, ptr, dir) \
	__snd_pcm_hw_param_get_min64(params, type, (unsigned long *)(ptr), dir)
#define _snd_pcm_hw_param_get_max64(params, type, ptr, dir) \
	__snd_pcm_hw_param_get_max64(params, type, (unsigned long *)(ptr), dir)

#define DEFINE_SET64(name) \
static inline \
int __snd_pcm_hw_param_ ## name ## 64(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,\
				    int type, unsigned long *val, int *dir)\
{\
	unsigned int _val = *val;\
	int err = _snd_pcm_hw_param_ ## name(pcm, params, type, &_val, dir);\
	*val = _val;\
	return err;\
}
DEFINE_SET64(set_min);
DEFINE_SET64(set_max);
DEFINE_SET64(set_near);
DEFINE_SET64(set_first);
DEFINE_SET64(set_last);
#define _snd_pcm_hw_param_set_min64(pcm, params, type, ptr, dir) \
	__snd_pcm_hw_param_set_min64(pcm, params, type, (unsigned long *)(ptr), dir)
#define _snd_pcm_hw_param_set_max64(pcm, params, type, ptr, dir) \
	__snd_pcm_hw_param_set_max64(pcm, params, type, (unsigned long *)(ptr), dir)
#define _snd_pcm_hw_param_set_near64(pcm, params, type, ptr, dir) \
	__snd_pcm_hw_param_set_near64(pcm, params, type, (unsigned long *)(ptr), dir)
#define _snd_pcm_hw_param_set_first64(pcm, params, type, ptr, dir) \
	__snd_pcm_hw_param_set_first64(pcm, params, type, (unsigned long *)(ptr), dir)
#define _snd_pcm_hw_param_set_last64(pcm, params, type, ptr, dir) \
	__snd_pcm_hw_param_set_last64(pcm, params, type, (unsigned long *)(ptr), dir)

static inline
int __snd_pcm_hw_param_set_minmax64(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				   int type,
				   unsigned long *minval, int *mindir,
				   unsigned long *maxval, int *maxdir)
{
	unsigned int _min = *minval;
	unsigned int _max = *maxval;
	int err = _snd_pcm_hw_param_set_minmax(pcm, params, type,
					      &_min, mindir, &_max, maxdir);
	*minval = _min;
	*maxval = _max;
	return err;
}
#define _snd_pcm_hw_param_set_minmax64(pcm, params, type, minptr, mindir, maxptr, maxdir) \
	__snd_pcm_hw_param_set_minmax64(pcm, params, type, \
				       (unsigned long *)(minptr), mindir, \
				       (unsigned long *)(maxptr), maxdir)

#undef DEFINE_GET64
#undef DEFINE_SET64

#else /* 32BIT */

#define _snd_pcm_hw_param_get64(params, type, ptr, dir) \
	_snd_pcm_hw_param_get(params, type, (unsigned int *)(ptr), dir)
#define _snd_pcm_hw_param_get_min64(params, type, ptr, dir) \
	_snd_pcm_hw_param_get_min(params, type, (unsigned int *)(ptr), dir)
#define _snd_pcm_hw_param_get_max64(params, type, ptr, dir) \
	_snd_pcm_hw_param_get_max(params, type, (unsigned int *)(ptr), dir)
#define _snd_pcm_hw_param_set_min64(pcm, params, type, ptr, dir)	\
	_snd_pcm_hw_param_set_min(pcm, params, type, (unsigned int *)(ptr), dir)
#define _snd_pcm_hw_param_set_max64(pcm, params, type, ptr, dir)	\
	_snd_pcm_hw_param_set_max(pcm, params, type, (unsigned int *)(ptr), dir)
#define _snd_pcm_hw_param_set_minmax64(pcm, params, type, minptr, mindir, maxptr, maxdir) \
	_snd_pcm_hw_param_set_minmax(pcm, params, type,			\
				    (unsigned int *)(minptr), mindir,\
				    (unsigned int *)maxptr, maxdir)
#define _snd_pcm_hw_param_set_near64(pcm, params, type, ptr, dir)	\
	_snd_pcm_hw_param_set_near(pcm, params, type, (unsigned int *)(ptr), dir)
#define _snd_pcm_hw_param_set_first64(pcm, params, type, ptr, dir)	\
	_snd_pcm_hw_param_set_first(pcm, params, type, (unsigned int *)(ptr), dir)
#define _snd_pcm_hw_param_set_last64(pcm, params, type, ptr, dir)	\
	_snd_pcm_hw_param_set_last(pcm, params, type, (unsigned int *)(ptr), dir)
#endif

__snd_define_type(snd_pcm_access_mask);

static inline
void snd_pcm_access_mask_none(snd_pcm_access_mask_t *mask)
{
	_snd_mask_none((snd_mask_t *) mask);
}

static inline
void snd_pcm_access_mask_any(snd_pcm_access_mask_t *mask)
{
	_snd_mask_any((snd_mask_t *) mask);
}

static inline
int snd_pcm_access_mask_test(const snd_pcm_access_mask_t *mask, snd_pcm_access_t val)
{
	return _snd_mask_test((const snd_mask_t *) mask, (unsigned long) val);
}

static inline
int snd_pcm_access_mask_empty(const snd_pcm_access_mask_t *mask)
{
	return _snd_mask_empty((const snd_mask_t *) mask);
}

static inline
void snd_pcm_access_mask_set(snd_pcm_access_mask_t *mask, snd_pcm_access_t val)
{
	_snd_mask_set((snd_mask_t *) mask, (unsigned long) val);
}

static inline
void snd_pcm_access_mask_reset(snd_pcm_access_mask_t *mask, snd_pcm_access_t val)
{
	_snd_mask_reset((snd_mask_t *) mask, (unsigned long) val);
}

__snd_define_type(snd_pcm_format_mask);

static inline
void snd_pcm_format_mask_none(snd_pcm_format_mask_t *mask)
{
	_snd_mask_none((snd_mask_t *) mask);
}

static inline
void snd_pcm_format_mask_any(snd_pcm_format_mask_t *mask)
{
	_snd_mask_any((snd_mask_t *) mask);
}

static inline
int snd_pcm_format_mask_test(const snd_pcm_format_mask_t *mask, snd_pcm_format_t val)
{
	return _snd_mask_test((const snd_mask_t *) mask, (unsigned long) val);
}

static inline
int snd_pcm_format_mask_empty(const snd_pcm_format_mask_t *mask)
{
	return _snd_mask_empty((const snd_mask_t *) mask);
}

static inline
void snd_pcm_format_mask_set(snd_pcm_format_mask_t *mask, snd_pcm_format_t val)
{
	_snd_mask_set((snd_mask_t *) mask, (unsigned long) val);
}

static inline
void snd_pcm_format_mask_reset(snd_pcm_format_mask_t *mask, snd_pcm_format_t val)
{
	_snd_mask_reset((snd_mask_t *) mask, (unsigned long) val);
}


__snd_define_type(snd_pcm_subformat_mask);

static inline
void snd_pcm_subformat_mask_none(snd_pcm_subformat_mask_t *mask)
{
	_snd_mask_none((snd_mask_t *) mask);
}

static inline
void snd_pcm_subformat_mask_any(snd_pcm_subformat_mask_t *mask)
{
	_snd_mask_any((snd_mask_t *) mask);
}

static inline
int snd_pcm_subformat_mask_test(const snd_pcm_subformat_mask_t *mask,
				snd_pcm_subformat_t val)
{
	return _snd_mask_test((const snd_mask_t *) mask, (unsigned long) val);
}

static inline
int snd_pcm_subformat_mask_empty(const snd_pcm_subformat_mask_t *mask)
{
	return _snd_mask_empty((const snd_mask_t *) mask);
}

static inline
void snd_pcm_subformat_mask_set(snd_pcm_subformat_mask_t *mask,
				snd_pcm_subformat_t val)
{
	_snd_mask_set((snd_mask_t *) mask, (unsigned long) val);
}

static inline
void snd_pcm_subformat_mask_reset(snd_pcm_subformat_mask_t *mask,
				  snd_pcm_subformat_t val)
{
	_snd_mask_reset((snd_mask_t *) mask, (unsigned long) val);
}


__snd_define_type(snd_pcm_hw_params);

static inline
int snd_pcm_hw_params_get_access(const snd_pcm_hw_params_t *params,
				 snd_pcm_access_t *access)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_ACCESS,
				    (unsigned int *)access, NULL);
}

static inline
int snd_pcm_hw_params_test_access(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				  snd_pcm_access_t access)
{
	return _snd_pcm_hw_param_test(pcm, params, SNDRV_PCM_HW_PARAM_ACCESS,
				      access, NULL);
}

static inline
int snd_pcm_hw_params_set_access(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				 snd_pcm_access_t access)
{
	return _snd_pcm_hw_param_set(pcm, params, SNDRV_PCM_HW_PARAM_ACCESS,
				    access, 0);
}

static inline
int snd_pcm_hw_params_set_access_first(snd_pcm_t *pcm,
				       snd_pcm_hw_params_t *params,
				       snd_pcm_access_t *access)
{
	return _snd_pcm_hw_param_set_first(pcm, params,
					   SNDRV_PCM_HW_PARAM_ACCESS,
					   (unsigned int *)access, NULL);
}

static inline
int snd_pcm_hw_params_set_access_last(snd_pcm_t *pcm,
				      snd_pcm_hw_params_t *params,
				      snd_pcm_access_t *access)
{
	return _snd_pcm_hw_param_set_last(pcm, params,
					  SNDRV_PCM_HW_PARAM_ACCESS,
					  (unsigned int *)access, NULL);
}

static inline
int snd_pcm_hw_params_set_access_mask(snd_pcm_t *pcm,
				      snd_pcm_hw_params_t *params,
				      snd_pcm_access_mask_t *mask)
{
	return _snd_pcm_hw_param_set_mask(pcm, params,
					  SNDRV_PCM_HW_PARAM_ACCESS,
					 (snd_mask_t *) mask);
}

static inline
int snd_pcm_hw_params_get_access_mask(snd_pcm_hw_params_t *params,
				      snd_pcm_access_mask_t *mask)
{
	if (params == NULL || mask == NULL)
		return -EINVAL;
	snd_pcm_access_mask_copy(mask, _snd_pcm_hw_param_get_mask(params, SNDRV_PCM_HW_PARAM_ACCESS));
	return 0;
}


static inline
int snd_pcm_hw_params_get_format(const snd_pcm_hw_params_t *params,
				 snd_pcm_format_t *format)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_FORMAT,
				    (unsigned int *)format, NULL);
}

static inline
int snd_pcm_hw_params_test_format(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				  snd_pcm_format_t format)
{
	return _snd_pcm_hw_param_test(pcm, params, SNDRV_PCM_HW_PARAM_FORMAT,
				      format, NULL);
}

static inline
int snd_pcm_hw_params_set_format(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				 snd_pcm_format_t format)
{
	return _snd_pcm_hw_param_set(pcm, params, SNDRV_PCM_HW_PARAM_FORMAT,
				    format, 0);
}

static inline
int snd_pcm_hw_params_set_format_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       snd_pcm_format_t *format)
{
	return _snd_pcm_hw_param_set_first(pcm, params, SNDRV_PCM_HW_PARAM_FORMAT,
					  (unsigned int *)format, NULL);
}

static inline
int snd_pcm_hw_params_set_format_last(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      snd_pcm_format_t *format)
{
	return _snd_pcm_hw_param_set_last(pcm, params, SNDRV_PCM_HW_PARAM_FORMAT,
					 (unsigned int *)format, NULL);
}

static inline
int snd_pcm_hw_params_set_format_mask(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      snd_pcm_format_mask_t *mask)
{
	return _snd_pcm_hw_param_set_mask(pcm, params, SNDRV_PCM_HW_PARAM_FORMAT,
					 (snd_mask_t *) mask);
}

static inline
void snd_pcm_hw_params_get_format_mask(snd_pcm_hw_params_t *params,
				       snd_pcm_format_mask_t *mask)
{
	snd_pcm_format_mask_copy(mask, _snd_pcm_hw_param_get_mask(params, SNDRV_PCM_HW_PARAM_FORMAT));
}


static inline
int snd_pcm_hw_params_get_subformat(const snd_pcm_hw_params_t *params,
				    snd_pcm_subformat_t *subformat)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_SUBFORMAT,
				     (unsigned int *)subformat, NULL);
}

static inline
int snd_pcm_hw_params_test_subformat(snd_pcm_t *pcm,
				     snd_pcm_hw_params_t *params,
				     snd_pcm_subformat_t subformat)
{
	return _snd_pcm_hw_param_test(pcm, params, SNDRV_PCM_HW_PARAM_SUBFORMAT,
				      subformat, NULL);
}

static inline
int snd_pcm_hw_params_set_subformat(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				    snd_pcm_subformat_t subformat)
{
	return _snd_pcm_hw_param_set(pcm, params, SNDRV_PCM_HW_PARAM_SUBFORMAT,
				     subformat, 0);
}

static inline
int snd_pcm_hw_params_set_subformat_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  snd_pcm_subformat_t *subformat)
{
	return _snd_pcm_hw_param_set_first(pcm, params,
					   SNDRV_PCM_HW_PARAM_SUBFORMAT,
					   (unsigned int *)subformat, NULL);
}

static inline
int snd_pcm_hw_params_set_subformat_last(snd_pcm_t *pcm,
					 snd_pcm_hw_params_t *params,
					 snd_pcm_subformat_t *subformat)
{
	return _snd_pcm_hw_param_set_last(pcm, params,
					  SNDRV_PCM_HW_PARAM_SUBFORMAT,
					  (unsigned int *)subformat, NULL);
}

static inline
int snd_pcm_hw_params_set_subformat_mask(snd_pcm_t *pcm,
					 snd_pcm_hw_params_t *params,
					 snd_pcm_subformat_mask_t *mask)
{
	return _snd_pcm_hw_param_set_mask(pcm, params,
					  SNDRV_PCM_HW_PARAM_SUBFORMAT,
					  (snd_mask_t *) mask);
}

static inline
void snd_pcm_hw_params_get_subformat_mask(snd_pcm_hw_params_t *params,
					  snd_pcm_subformat_mask_t *mask)
{
	snd_pcm_subformat_mask_copy(mask, _snd_pcm_hw_param_get_mask(params, SNDRV_PCM_HW_PARAM_SUBFORMAT));
}


static inline
int snd_pcm_hw_params_get_channels(const snd_pcm_hw_params_t *params,
				   unsigned int *val)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_CHANNELS, val, NULL);
}

static inline
int snd_pcm_hw_params_get_channels_min(const snd_pcm_hw_params_t *params,
				       unsigned int *val)
{
	return _snd_pcm_hw_param_get_min(params, SNDRV_PCM_HW_PARAM_CHANNELS, val, NULL);
}

static inline
int snd_pcm_hw_params_get_channels_max(const snd_pcm_hw_params_t *params,
				       unsigned int *val)
{
	return _snd_pcm_hw_param_get_max(params, SNDRV_PCM_HW_PARAM_CHANNELS, val, NULL);
}

static inline
int snd_pcm_hw_params_test_channels(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				    unsigned int val)
{
	return _snd_pcm_hw_param_test(pcm, params,
				      SNDRV_PCM_HW_PARAM_CHANNELS, val, NULL);
}

static inline
int snd_pcm_hw_params_set_channels(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				   unsigned int val)
{
	return _snd_pcm_hw_param_set(pcm, params,
				    SNDRV_PCM_HW_PARAM_CHANNELS, val, 0);
}

static inline
int snd_pcm_hw_params_set_channels_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       unsigned int *val)
{
	return _snd_pcm_hw_param_set_min(pcm, params, SNDRV_PCM_HW_PARAM_CHANNELS,
					val, NULL);
}

static inline
int snd_pcm_hw_params_set_channels_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       unsigned int *val)
{
	return _snd_pcm_hw_param_set_max(pcm, params, SNDRV_PCM_HW_PARAM_CHANNELS,
					val, NULL);
}

static inline
int snd_pcm_hw_params_set_channels_minmax(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  unsigned int *min, unsigned int *max)
{
	return _snd_pcm_hw_param_set_minmax(pcm, params, SNDRV_PCM_HW_PARAM_CHANNELS,
					   min, NULL, max, NULL);
}

static inline
int snd_pcm_hw_params_set_channels_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int *val)
{
	return _snd_pcm_hw_param_set_near(pcm, params, SNDRV_PCM_HW_PARAM_CHANNELS,
					 val, NULL);
}

static inline
int snd_pcm_hw_params_set_channels_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					 unsigned int *val)
{
	return _snd_pcm_hw_param_set_first(pcm, params, SNDRV_PCM_HW_PARAM_CHANNELS,
					  val, NULL);
}

static inline
int snd_pcm_hw_params_set_channels_last(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int *val)
{
	return _snd_pcm_hw_param_set_last(pcm, params, SNDRV_PCM_HW_PARAM_CHANNELS,
					 val, NULL);
}


static inline
int snd_pcm_hw_params_get_rate(const snd_pcm_hw_params_t *params, unsigned int *val,
			       int *dir)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_RATE, val, dir);
}

static inline
int snd_pcm_hw_params_get_rate_min(const snd_pcm_hw_params_t *params,
				   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_min(params, SNDRV_PCM_HW_PARAM_RATE, val, dir);
}

static inline
int snd_pcm_hw_params_get_rate_max(const snd_pcm_hw_params_t *params,
				   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_max(params, SNDRV_PCM_HW_PARAM_RATE, val, dir);
}

static inline
int snd_pcm_hw_params_test_rate(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				unsigned int val, int dir)
{
	return _snd_pcm_hw_param_test(pcm, params, SNDRV_PCM_HW_PARAM_RATE,
				      val, &dir);
}

static inline
int snd_pcm_hw_params_set_rate(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
			       unsigned int val, int dir)
{
	return _snd_pcm_hw_param_set(pcm, params, SNDRV_PCM_HW_PARAM_RATE,
				    val, dir);
}

static inline
int snd_pcm_hw_params_set_rate_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_min(pcm, params, SNDRV_PCM_HW_PARAM_RATE,
					val, dir);
}

static inline
int snd_pcm_hw_params_set_rate_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_max(pcm, params, SNDRV_PCM_HW_PARAM_RATE,
					val, dir);
}

static inline
int snd_pcm_hw_params_set_rate_minmax(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      unsigned int *min, int *mindir,
				      unsigned int *max, int *maxdir)
{
	return _snd_pcm_hw_param_set_minmax(pcm, params, SNDRV_PCM_HW_PARAM_RATE,
					   min, mindir, max, maxdir);
}

static inline
int snd_pcm_hw_params_set_rate_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				    unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_near(pcm, params, SNDRV_PCM_HW_PARAM_RATE, val, dir);
}

static inline
int snd_pcm_hw_params_set_rate_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				     unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_first(pcm, params, SNDRV_PCM_HW_PARAM_RATE,
					  val, dir);
}

static inline
int snd_pcm_hw_params_set_rate_last(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				    unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_last(pcm, params, SNDRV_PCM_HW_PARAM_RATE, val, dir);
}

static inline
int snd_pcm_hw_params_set_rate_resample(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int val)
{
	return val ? -EINVAL : 0;  /* don't support plug layer */
}

static inline
int snd_pcm_hw_params_get_rate_resample(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int *val)
{
	return 0;
}

static inline
int snd_pcm_hw_params_set_export_buffer(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int val)
{
	return -EINVAL;
}

static inline
int snd_pcm_hw_params_get_export_buffer(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int *val)
{
	return 0;
}

static inline
int snd_pcm_hw_params_get_period_time(const snd_pcm_hw_params_t *params,
				      unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_PERIOD_TIME, val, dir);
}

static inline
int snd_pcm_hw_params_get_period_time_min(const snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_min(params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					val, dir);
}

static inline
int snd_pcm_hw_params_get_period_time_max(const snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_max(params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					val, dir);
}

static inline
int snd_pcm_hw_params_test_period_time(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       unsigned int val, int dir)
{
	return _snd_pcm_hw_param_test(pcm, params,
				      SNDRV_PCM_HW_PARAM_PERIOD_TIME, val, &dir);
}

static inline
int snd_pcm_hw_params_set_period_time(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      unsigned int val, int dir)
{
	return _snd_pcm_hw_param_set(pcm, params,
				    SNDRV_PCM_HW_PARAM_PERIOD_TIME, val, dir);
}


static inline
int snd_pcm_hw_params_set_period_time_min(snd_pcm_t *pcm,
					  snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_min(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					val, dir);
}

static inline
int snd_pcm_hw_params_set_period_time_max(snd_pcm_t *pcm,
					  snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_max(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					val, dir);
}

static inline
int snd_pcm_hw_params_set_period_time_minmax(snd_pcm_t *pcm,
					     snd_pcm_hw_params_t *params,
					     unsigned int *min, int *mindir,
					     unsigned int *max, int *maxdir)
{
	return _snd_pcm_hw_param_set_minmax(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					   min, mindir, max, maxdir);
}

static inline
int snd_pcm_hw_params_set_period_time_near(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_near(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					 val, dir);
}

static inline
int snd_pcm_hw_params_set_period_time_first(snd_pcm_t *pcm,
					    snd_pcm_hw_params_t *params,
					    unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_first(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					  val, dir);
}

static inline
int snd_pcm_hw_params_set_period_time_last(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_last(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_TIME,
					 val, dir);
}


static inline
int snd_pcm_hw_params_get_period_size(const snd_pcm_hw_params_t *params,
				      snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_get64(params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
				      val, dir);
}

static inline
int snd_pcm_hw_params_get_period_size_min(const snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_get_min64(params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					  val, dir);
}

static inline
int snd_pcm_hw_params_get_period_size_max(const snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_get_max64(params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					   val, dir);
}

static inline
int snd_pcm_hw_params_test_period_size(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       snd_pcm_uframes_t val, int dir)
{
	return _snd_pcm_hw_param_test(pcm, params,
				      SNDRV_PCM_HW_PARAM_PERIOD_SIZE, val, &dir);
}

static inline
int snd_pcm_hw_params_set_period_size(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      snd_pcm_uframes_t val, int dir)
{
	return _snd_pcm_hw_param_set(pcm, params,
				    SNDRV_PCM_HW_PARAM_PERIOD_SIZE, val, dir);
}

static inline
int snd_pcm_hw_params_set_period_size_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_set_min64(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					  val, dir);
}

static inline
int snd_pcm_hw_params_set_period_size_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_set_max64(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					val, dir);
}

static inline
int snd_pcm_hw_params_set_period_size_minmax(snd_pcm_t *pcm,
					     snd_pcm_hw_params_t *params,
					     snd_pcm_uframes_t *min, int *mindir,
					     snd_pcm_uframes_t *max, int *maxdir)
{
	return _snd_pcm_hw_param_set_minmax64(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					     min, mindir, max, maxdir);
}

static inline
int snd_pcm_hw_params_set_period_size_near(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_set_near64(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					   val, dir);
}

static inline
int snd_pcm_hw_params_set_period_size_first(snd_pcm_t *pcm,
					    snd_pcm_hw_params_t *params,
					    snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_set_first64(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					    val, dir);
}

static inline
int snd_pcm_hw_params_set_period_size_last(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   snd_pcm_uframes_t *val, int *dir)
{
	return _snd_pcm_hw_param_set_last64(pcm, params, SNDRV_PCM_HW_PARAM_PERIOD_SIZE,
					   val, dir);
}

static inline
int snd_pcm_hw_params_set_period_size_integer(snd_pcm_t *pcm,
					      snd_pcm_hw_params_t *params)
{
	return _snd_pcm_hw_param_set_integer(pcm, params,
					    SNDRV_PCM_HW_PARAM_PERIOD_SIZE);
}


static inline
int snd_pcm_hw_params_get_periods(const snd_pcm_hw_params_t *params,
				  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_PERIODS, val, dir);
}

static inline
int snd_pcm_hw_params_get_periods_min(const snd_pcm_hw_params_t *params,
				      unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_min(params, SNDRV_PCM_HW_PARAM_PERIODS, val, dir);
}

static inline
int snd_pcm_hw_params_get_periods_max(const snd_pcm_hw_params_t *params,
				      unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_max(params, SNDRV_PCM_HW_PARAM_PERIODS, val, dir);
}

static inline
int snd_pcm_hw_params_test_periods(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				   unsigned int val, int dir)
{
	return _snd_pcm_hw_param_test(pcm, params,
				      SNDRV_PCM_HW_PARAM_PERIODS, val, &dir);
}

static inline
int snd_pcm_hw_params_set_periods(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				  unsigned int val, int dir)
{
	return _snd_pcm_hw_param_set(pcm, params,
				    SNDRV_PCM_HW_PARAM_PERIODS, val, dir);
}

static inline
int snd_pcm_hw_params_set_periods_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_min(pcm, params,
					SNDRV_PCM_HW_PARAM_PERIODS, val, dir);
}

static inline
int snd_pcm_hw_params_set_periods_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_max(pcm, params,
					SNDRV_PCM_HW_PARAM_PERIODS, val, dir);
}


static inline
int snd_pcm_hw_params_set_periods_minmax(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					 unsigned int *min, int *mindir,
					 unsigned int *max, int *maxdir)
{
	return _snd_pcm_hw_param_set_minmax(pcm, params,
					   SNDRV_PCM_HW_PARAM_PERIODS,
					   min, mindir, max, maxdir);
}

static inline
int snd_pcm_hw_params_set_periods_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_near(pcm, params, SNDRV_PCM_HW_PARAM_PERIODS,
					 val, dir);
}

static inline
int snd_pcm_hw_params_set_periods_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_first(pcm, params, SNDRV_PCM_HW_PARAM_PERIODS,
					  val, dir);
}

static inline
int snd_pcm_hw_params_set_periods_last(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_last(pcm, params, SNDRV_PCM_HW_PARAM_PERIODS,
					 val, dir);
}

static inline
int snd_pcm_hw_params_set_periods_integer(snd_pcm_t *pcm, snd_pcm_hw_params_t *params)
{
	return _snd_pcm_hw_param_set_integer(pcm, params,
					    SNDRV_PCM_HW_PARAM_PERIODS);
}


static inline
int snd_pcm_hw_params_get_buffer_time(const snd_pcm_hw_params_t *params,
				      unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_BUFFER_TIME, val, dir);
}

static inline
int snd_pcm_hw_params_get_buffer_time_min(const snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_min(params, SNDRV_PCM_HW_PARAM_BUFFER_TIME,
					val, dir);
}

static inline
int snd_pcm_hw_params_get_buffer_time_max(const snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_max(params, SNDRV_PCM_HW_PARAM_BUFFER_TIME,
					val, dir);
}

static inline
int snd_pcm_hw_params_test_buffer_time(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       unsigned int val, int dir)
{
	return _snd_pcm_hw_param_test(pcm, params,
				      SNDRV_PCM_HW_PARAM_BUFFER_TIME, val, &dir);
}

static inline
int snd_pcm_hw_params_set_buffer_time(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      unsigned int val, int dir)
{
	return _snd_pcm_hw_param_set(pcm, params,
				    SNDRV_PCM_HW_PARAM_BUFFER_TIME, val, dir);
}

static inline
int snd_pcm_hw_params_set_buffer_time_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_min(pcm, params,
					SNDRV_PCM_HW_PARAM_BUFFER_TIME, val, dir);
}

static inline
int snd_pcm_hw_params_set_buffer_time_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_max(pcm, params,
					SNDRV_PCM_HW_PARAM_BUFFER_TIME, val, dir);
}

static inline
int snd_pcm_hw_params_set_buffer_time_minmax(snd_pcm_t *pcm,
					     snd_pcm_hw_params_t *params,
					     unsigned int *min, int *mindir,
					     unsigned int *max, int *maxdir)
{
	return _snd_pcm_hw_param_set_minmax(pcm, params,
					   SNDRV_PCM_HW_PARAM_BUFFER_TIME,
					   min, mindir, max, maxdir);
}

static inline
int snd_pcm_hw_params_set_buffer_time_near(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_near(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_TIME,
					 val, dir);
}

static inline
int snd_pcm_hw_params_set_buffer_time_first(snd_pcm_t *pcm,
					    snd_pcm_hw_params_t *params,
					    unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_first(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_TIME,
					  val, dir);
}

static inline
int snd_pcm_hw_params_set_buffer_time_last(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_last(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_TIME,
					 val, dir);
}


static inline
int snd_pcm_hw_params_get_buffer_size(const snd_pcm_hw_params_t *params,
				      snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_get64(params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
				      val, NULL);
}

static inline
int snd_pcm_hw_params_get_buffer_size_min(const snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_get_min64(params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					  val, NULL);
}

static inline
int snd_pcm_hw_params_get_buffer_size_max(const snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_get_max64(params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					  val, NULL);
}

static inline
int snd_pcm_hw_params_test_buffer_size(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				       snd_pcm_uframes_t val)
{
	return _snd_pcm_hw_param_test(pcm, params,
				      SNDRV_PCM_HW_PARAM_BUFFER_SIZE, val, 0);
}

static inline
int snd_pcm_hw_params_set_buffer_size(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				      snd_pcm_uframes_t val)
{
	return _snd_pcm_hw_param_set(pcm, params,
				    SNDRV_PCM_HW_PARAM_BUFFER_SIZE, val, 0);
}

static inline
int snd_pcm_hw_params_set_buffer_size_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_set_min64(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					  val, NULL);
}

static inline
int snd_pcm_hw_params_set_buffer_size_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_set_max64(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					  val, NULL);
}

static inline
int snd_pcm_hw_params_set_buffer_size_minmax64(snd_pcm_t *pcm,
					       snd_pcm_hw_params_t *params,
					       snd_pcm_uframes_t *min,
					       snd_pcm_uframes_t *max)
{
	return _snd_pcm_hw_param_set_minmax64(pcm, params,
					     SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					     min, NULL, max, NULL);
}

static inline
int snd_pcm_hw_params_set_buffer_size_near(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_set_near64(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					   val, NULL);
}

static inline
int snd_pcm_hw_params_set_buffer_size_first(snd_pcm_t *pcm,
					    snd_pcm_hw_params_t *params,
					    snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_set_first64(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					    val, NULL);
}

static inline
int snd_pcm_hw_params_set_buffer_size_last(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   snd_pcm_uframes_t *val)
{
	return _snd_pcm_hw_param_set_last64(pcm, params, SNDRV_PCM_HW_PARAM_BUFFER_SIZE,
					    val, NULL);
}


static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_get_tick_time(const snd_pcm_hw_params_t *params,
				    unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get(params, SNDRV_PCM_HW_PARAM_TICK_TIME, val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_get_tick_time_min(const snd_pcm_hw_params_t *params,
					unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_min(params, SNDRV_PCM_HW_PARAM_TICK_TIME, val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_get_tick_time_max(const snd_pcm_hw_params_t *params,
					unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_get_max(params, SNDRV_PCM_HW_PARAM_TICK_TIME, val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_test_tick_time(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				     unsigned int val, int dir)
{
	return _snd_pcm_hw_param_test(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
				      val, &dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_set_tick_time(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
				    unsigned int val, int dir)
{
	return _snd_pcm_hw_param_set(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
				    val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_set_tick_time_min(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_min(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
					val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_set_tick_time_max(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_max(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
					val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_set_tick_time_minmax(snd_pcm_t *pcm,
					   snd_pcm_hw_params_t *params,
					   unsigned int *min, int *mindir,
					   unsigned int *max, int *maxdir)
{
	return _snd_pcm_hw_param_set_minmax(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
					   min, mindir, max, maxdir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_set_tick_time_near(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					 unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_near(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
					 val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_set_tick_time_first(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					  unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_first(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
					  val, dir);
}

static inline
__attribute__ ((deprecated))
int snd_pcm_hw_params_set_tick_time_last(snd_pcm_t *pcm, snd_pcm_hw_params_t *params,
					 unsigned int *val, int *dir)
{
	return _snd_pcm_hw_param_set_last(pcm, params, SNDRV_PCM_HW_PARAM_TICK_TIME,
					 val, dir);
}


static inline
int snd_pcm_sw_params_current(snd_pcm_t *pcm, snd_pcm_sw_params_t *params)
{
	*params = pcm->sw_params;
	return 0;
}

__snd_define_type(snd_pcm_sw_params);

static inline
int snd_pcm_sw_params_get_boundary(const snd_pcm_sw_params_t *params,
				   snd_pcm_uframes_t *val)
{
	*val = params->boundary;
	return 0;
}

static inline
int snd_pcm_sw_params_set_tstamp_mode(snd_pcm_t *pcm, snd_pcm_sw_params_t *params,
				      snd_pcm_tstamp_t val)
{
	params->tstamp_mode = val;
	return 0;
}

static inline
int snd_pcm_sw_params_get_tstamp_mode(const snd_pcm_sw_params_t *params,
				      snd_pcm_tstamp_t *val)
{
	*val = (snd_pcm_tstamp_t) params->tstamp_mode;
	return 0;
}

static inline
__attribute__ ((deprecated))
int snd_pcm_sw_params_set_sleep_min(snd_pcm_t *pcm, snd_pcm_sw_params_t *params,
				    unsigned int val)
{
	params->sleep_min = val;
	return 0;
}

static inline
__attribute__ ((deprecated))
int snd_pcm_sw_params_get_sleep_min(const snd_pcm_sw_params_t *params,
				    unsigned int *val)
{
	*val = params->sleep_min;
	return 0;
}

static inline
int snd_pcm_sw_params_set_avail_min(snd_pcm_t *pcm, snd_pcm_sw_params_t *params,
				    snd_pcm_uframes_t val)
{
	params->avail_min = val;
	return 0;
}

static inline
int snd_pcm_sw_params_get_avail_min(const snd_pcm_sw_params_t *params,
				    snd_pcm_uframes_t *val)
{
	*val = params->avail_min;
	return 0;
}


static inline
__attribute__ ((deprecated))
int snd_pcm_sw_params_set_xfer_align(snd_pcm_t *pcm, snd_pcm_sw_params_t *params,
				     snd_pcm_uframes_t val)
{
	params->xfer_align = val;
	return 0;
}

static inline
__attribute__ ((deprecated))
int snd_pcm_sw_params_get_xfer_align(const snd_pcm_sw_params_t *params,
				     snd_pcm_uframes_t *val)
{
	*val = params->xfer_align;
	return 0;
}


static inline
int snd_pcm_sw_params_set_start_threshold(snd_pcm_t *pcm, snd_pcm_sw_params_t *params,
					  snd_pcm_uframes_t val)
{
	params->start_threshold = val;
	return 0;
}

static inline
int snd_pcm_sw_params_get_start_threshold(const snd_pcm_sw_params_t *params,
					  snd_pcm_uframes_t *val)
{
	*val = params->start_threshold;
	return 0;
}


static inline
int snd_pcm_sw_params_set_stop_threshold(snd_pcm_t *pcm, snd_pcm_sw_params_t *params,
					 snd_pcm_uframes_t val)
{
	params->stop_threshold = val;
	return 0;
}

static inline
int snd_pcm_sw_params_get_stop_threshold(const snd_pcm_sw_params_t *params,
					 snd_pcm_uframes_t *val)
{
	*val = params->stop_threshold;
	return 0;
}


static inline
int snd_pcm_sw_params_set_silence_threshold(snd_pcm_t *pcm,
					    snd_pcm_sw_params_t *params,
					    snd_pcm_uframes_t val)
{
	params->silence_threshold = val;
	return 0;
}

static inline
int snd_pcm_sw_params_get_silence_threshold(const snd_pcm_sw_params_t *params,
					    snd_pcm_uframes_t *val)
{
	*val = params->silence_threshold;
	return 0;
}


static inline
int snd_pcm_sw_params_set_silence_size(snd_pcm_t *pcm, snd_pcm_sw_params_t *params,
				       snd_pcm_uframes_t val)
{
	params->silence_size = val;
	return 0;
}

static inline
int snd_pcm_sw_params_get_silence_size(const snd_pcm_sw_params_t *params,
				       snd_pcm_uframes_t *val)
{
	*val = params->silence_size;
	return 0;
}


__snd_define_type(snd_pcm_status);

static inline
snd_pcm_state_t snd_pcm_status_get_state(const snd_pcm_status_t *obj)
{
	return (snd_pcm_state_t) obj->state;
}

static inline
void snd_pcm_status_get_trigger_tstamp(const snd_pcm_status_t *obj,
				       snd_timestamp_t *ptr)
{
	ptr->tv_sec = obj->trigger_tstamp.tv_sec;
	ptr->tv_usec = obj->trigger_tstamp.tv_nsec / 1000L;
}

static inline
void snd_pcm_status_get_trigger_htstamp(const snd_pcm_status_t *obj,
					snd_htimestamp_t *ptr)
{
	*ptr = obj->trigger_tstamp;
}

static inline
void snd_pcm_status_get_tstamp(const snd_pcm_status_t *obj, snd_timestamp_t *ptr)
{
	ptr->tv_sec = obj->tstamp.tv_sec;
	ptr->tv_usec = obj->tstamp.tv_nsec / 1000L;
}

static inline
void snd_pcm_status_get_htstamp(const snd_pcm_status_t *obj, snd_htimestamp_t *ptr)
{
	*ptr = obj->tstamp;
}

static inline
snd_pcm_sframes_t snd_pcm_status_get_delay(const snd_pcm_status_t *obj)
{
	return obj->delay;
}

static inline
snd_pcm_uframes_t snd_pcm_status_get_avail(const snd_pcm_status_t *obj)
{
	return obj->avail;
}

static inline
snd_pcm_uframes_t snd_pcm_status_get_avail_max(const snd_pcm_status_t *obj)
{
	return obj->avail_max;
}

static inline
snd_pcm_uframes_t snd_pcm_status_get_overrange(const snd_pcm_status_t *obj)
{
	return obj->overrange;
}

__snd_define_type(snd_pcm_info);

static inline
unsigned int snd_pcm_info_get_device(const snd_pcm_info_t *obj)
{
	return obj->device;
}

static inline
unsigned int snd_pcm_info_get_subdevice(const snd_pcm_info_t *obj)
{
	return obj->subdevice;
}

static inline
snd_pcm_stream_t snd_pcm_info_get_stream(const snd_pcm_info_t *obj)
{
	return (snd_pcm_stream_t) obj->stream;
}

static inline
int snd_pcm_info_get_card(const snd_pcm_info_t *obj)
{
	return obj->card;
}

static inline
const char *snd_pcm_info_get_id(const snd_pcm_info_t *obj)
{
	return (const char *)obj->id;
}

static inline
const char *snd_pcm_info_get_name(const snd_pcm_info_t *obj)
{
	return (const char *)obj->name;
}

static inline
const char *snd_pcm_info_get_subdevice_name(const snd_pcm_info_t *obj)
{
	return (const char *)obj->subname;
}

static inline
snd_pcm_class_t snd_pcm_info_get_class(const snd_pcm_info_t *obj)
{
	return (snd_pcm_class_t) obj->dev_class;
}

static inline
snd_pcm_subclass_t snd_pcm_info_get_subclass(const snd_pcm_info_t *obj)
{
	return (snd_pcm_subclass_t) obj->dev_subclass;
}

static inline
unsigned int snd_pcm_info_get_subdevices_count(const snd_pcm_info_t *obj)
{
	return obj->subdevices_count;
}

static inline
unsigned int snd_pcm_info_get_subdevices_avail(const snd_pcm_info_t *obj)
{
	return obj->subdevices_avail;
}

static inline
snd_pcm_sync_id_t snd_pcm_info_get_sync(const snd_pcm_info_t *obj)
{
	snd_pcm_sync_id_t res;
	memcpy(&res, &obj->sync, sizeof(res));
	return res;
}

static inline
void snd_pcm_info_set_device(snd_pcm_info_t *obj, unsigned int val)
{
	obj->device = val;
}

static inline
void snd_pcm_info_set_subdevice(snd_pcm_info_t *obj, unsigned int val)
{
	obj->subdevice = val;
}

static inline
void snd_pcm_info_set_stream(snd_pcm_info_t *obj, snd_pcm_stream_t val)
{
	obj->stream = val;
}

static inline
int snd_hw_htimestamp(snd_pcm_t *pcm, snd_pcm_uframes_t *avail,
		      snd_htimestamp_t *tstamp)
{
	*avail = snd_pcm_avail_update(pcm);
	*tstamp = pcm->mmap_status->tstamp;
	return 0;
}


/*
 * helper functions
 */
static inline
int snd_pcm_get_params(snd_pcm_t *pcm,
                       snd_pcm_uframes_t *buffer_size,
                       snd_pcm_uframes_t *period_size)
{
	if (!pcm->setup)
		return -EBADFD;
	*buffer_size = pcm->buffer_size;
	*period_size = pcm->period_size;
	return 0;
}

/*
 * not implemented yet
 */

static inline __SALSA_NOT_IMPLEMENTED
int snd_pcm_open_lconf(snd_pcm_t **pcm, const char *name,
		       snd_pcm_stream_t stream, int mode,
		       snd_config_t *lconf)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
snd_pcm_sframes_t snd_pcm_mmap_writei(snd_pcm_t *pcm, const void *buffer,
				      snd_pcm_uframes_t size)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
snd_pcm_sframes_t snd_pcm_mmap_writen(snd_pcm_t *pcm, void **bufs,
				      snd_pcm_uframes_t size)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
snd_pcm_sframes_t snd_pcm_mmap_readi(snd_pcm_t *pcm, void *buffer,
				     snd_pcm_uframes_t size)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
snd_pcm_sframes_t snd_pcm_mmap_readn(snd_pcm_t *pcm, void **bufs,
				     snd_pcm_uframes_t size)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_pcm_set_params(snd_pcm_t *pcm,
                       snd_pcm_format_t format,
                       snd_pcm_access_t access,
                       unsigned int channels,
                       unsigned int rate,
                       int soft_resample,
                       unsigned int latency)
{
	return -ENXIO;
}

#if SALSA_HAS_ASYNC_SUPPORT

static inline
snd_pcm_t *snd_async_handler_get_pcm(snd_async_handler_t *handler)
{
	return (snd_pcm_t *) handler->rec;
}

#else /* !SALSA_HAS_ASYNC_SUPPORT */

static inline __SALSA_NOT_IMPLEMENTED
int snd_async_add_pcm_handler(snd_async_handler_t **handler, snd_pcm_t *pcm,
			      snd_async_callback_t callback,
			      void *private_data)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
snd_pcm_t *snd_async_handler_get_pcm(snd_async_handler_t *handler)
{
	return NULL;
}

#endif /* SALSA_HAS_ASYNC_SUPPORT */

#endif /* __ALSA_PCM_MACROS_H */
