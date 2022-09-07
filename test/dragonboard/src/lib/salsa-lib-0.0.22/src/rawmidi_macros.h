/*
 * rawmidi privates and macros
 */

#ifndef __ALSA_RAWMIDI_MACROS_H
#define __ALSA_RAWMIDI_MACROS_H

#include "asound.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

typedef struct _snd_rawmidi_hw {
	char *name;
	int fd;
	snd_rawmidi_type_t type;
	int opened;
} snd_rawmidi_hw_t;

struct _snd_rawmidi {
	int fd;
	snd_rawmidi_stream_t stream;
	int mode;
	struct pollfd pollfd;
	snd_rawmidi_params_t params;
	snd_rawmidi_hw_t *hw;
};

/*
 */

static inline __SALSA_NOT_IMPLEMENTED
int snd_rawmidi_open_lconf(snd_rawmidi_t **in_rmidi, snd_rawmidi_t **out_rmidi,
			   const char *name, int mode, snd_config_t *lconf)
{
	return -ENXIO;
}

static inline
int snd_rawmidi_nonblock(snd_rawmidi_t *rmidi, int nonblock)
{
	return _snd_set_nonblock(rmidi->fd, nonblock);
}

static inline
int snd_rawmidi_poll_descriptors_count(snd_rawmidi_t *rmidi)
{
	return 1;
}

static inline
int snd_rawmidi_poll_descriptors(snd_rawmidi_t *rmidi, struct pollfd *pfds,
				 unsigned int space)
{
	*pfds = rmidi->pollfd;
	return 1;
}

static inline
int snd_rawmidi_poll_descriptors_revents(snd_rawmidi_t *rawmidi,
					 struct pollfd *pfds,
					 unsigned int nfds,
					 unsigned short *revent)
{
	*revent = pfds->events;
	return 0;
}

__snd_define_type(snd_rawmidi_info);

static inline
unsigned int snd_rawmidi_info_get_device(const snd_rawmidi_info_t *obj)
{
	return obj->device;
}

static inline
unsigned int snd_rawmidi_info_get_subdevice(const snd_rawmidi_info_t *obj)
{
	return obj->subdevice;
}

static inline
snd_rawmidi_stream_t snd_rawmidi_info_get_stream(const snd_rawmidi_info_t *obj)
{
	return (snd_rawmidi_stream_t) obj->stream;
}

static inline
int snd_rawmidi_info_get_card(const snd_rawmidi_info_t *obj)
{
	return obj->card;
}

static inline
unsigned int snd_rawmidi_info_get_flags(const snd_rawmidi_info_t *obj)
{
	return obj->flags;
}

static inline
const char *snd_rawmidi_info_get_id(const snd_rawmidi_info_t *obj)
{
	return (const char *)obj->id;
}

static inline
const char *snd_rawmidi_info_get_name(const snd_rawmidi_info_t *obj)
{
	return (const char *)obj->name;
}

static inline
const char *snd_rawmidi_info_get_subdevice_name(const snd_rawmidi_info_t *obj)
{
	return (const char *)obj->subname;
}

static inline
unsigned int snd_rawmidi_info_get_subdevices_count(const snd_rawmidi_info_t *obj)
{
	return obj->subdevices_count;
}

static inline
unsigned int snd_rawmidi_info_get_subdevices_avail(const snd_rawmidi_info_t *obj)
{
	return obj->subdevices_avail;
}

static inline
void snd_rawmidi_info_set_device(snd_rawmidi_info_t *obj, unsigned int val)
{
	obj->device = val;
}

static inline
void snd_rawmidi_info_set_subdevice(snd_rawmidi_info_t *obj, unsigned int val)
{
	obj->subdevice = val;
}

static inline
void snd_rawmidi_info_set_stream(snd_rawmidi_info_t *obj,
				 snd_rawmidi_stream_t val)
{
	obj->stream = val;
}

static inline
int snd_rawmidi_info(snd_rawmidi_t *rmidi, snd_rawmidi_info_t * info)
{
	info->stream = rmidi->stream;
	if (ioctl(rmidi->fd, SNDRV_RAWMIDI_IOCTL_INFO, info) < 0)
		return -errno;
	return 0;
}

__snd_define_type(snd_rawmidi_params);

static inline
int snd_rawmidi_params_set_buffer_size(snd_rawmidi_t *rmidi,
				       snd_rawmidi_params_t *params,
				       size_t val)
{
	params->buffer_size = val;
	return 0;
}

static inline
size_t snd_rawmidi_params_get_buffer_size(const snd_rawmidi_params_t *params)
{
	return params->buffer_size;
}

static inline
int snd_rawmidi_params_set_avail_min(snd_rawmidi_t *rmidi,
				     snd_rawmidi_params_t *params, size_t val)
{
	params->avail_min = val;
	return 0;
}

static inline
size_t snd_rawmidi_params_get_avail_min(const snd_rawmidi_params_t *params)
{
	return params->avail_min;
}

static inline
int snd_rawmidi_params_set_no_active_sensing(snd_rawmidi_t *rmidi,
					     snd_rawmidi_params_t *params,
					     int val)
{
	params->no_active_sensing = val;
	return 0;
}

static inline
int snd_rawmidi_params_get_no_active_sensing(const snd_rawmidi_params_t *params)
{
	return params->no_active_sensing;
}

static inline
int snd_rawmidi_params(snd_rawmidi_t *rmidi, snd_rawmidi_params_t * params)
{
	params->stream = rmidi->stream;
	if (ioctl(rmidi->fd, SNDRV_RAWMIDI_IOCTL_PARAMS, params) < 0)
		return -errno;
	rmidi->params = *params;
	return 0;
}

static inline
int snd_rawmidi_params_current(snd_rawmidi_t *rmidi,
			       snd_rawmidi_params_t *params)
{
	*params = rmidi->params;
	return 0;
}

__snd_define_type(snd_rawmidi_status);

static inline
void snd_rawmidi_status_get_tstamp(const snd_rawmidi_status_t *obj,
				   snd_htimestamp_t *ptr)
{
	*ptr = obj->tstamp;
}

static inline
size_t snd_rawmidi_status_get_avail(const snd_rawmidi_status_t *obj)
{
	return obj->avail;
}

static inline
size_t snd_rawmidi_status_get_xruns(const snd_rawmidi_status_t *obj)
{
	return obj->xruns;
}

static inline
int snd_rawmidi_status(snd_rawmidi_t *rmidi, snd_rawmidi_status_t * status)
{
	status->stream = rmidi->stream;
	if (ioctl(rmidi->fd, SNDRV_RAWMIDI_IOCTL_STATUS, status) < 0)
		return -errno;
	return 0;
}

static inline
int snd_rawmidi_drain(snd_rawmidi_t *rmidi)
{
	int str = rmidi->stream;
	if (ioctl(rmidi->fd, SNDRV_RAWMIDI_IOCTL_DRAIN, &str) < 0)
		return -errno;
	return 0;
}

static inline
int snd_rawmidi_drop(snd_rawmidi_t *rmidi)
{
	int str = rmidi->stream;
	if (ioctl(rmidi->fd, SNDRV_RAWMIDI_IOCTL_DROP, &str) < 0)
		return -errno;
	return 0;
}

static inline
ssize_t snd_rawmidi_write(snd_rawmidi_t *rmidi, const void *buffer,
			  size_t size)
{
	ssize_t result;
	result = write(rmidi->fd, buffer, size);
	if (result < 0)
		return -errno;
	return result;
}

static inline
ssize_t snd_rawmidi_read(snd_rawmidi_t *rmidi, void *buffer, size_t size)
{
	ssize_t result;
	result = read(rmidi->fd, buffer, size);
	if (result < 0)
		return -errno;
	return result;
}

static inline
const char *snd_rawmidi_name(snd_rawmidi_t *rmidi)
{
	return rmidi->hw->name;
}

static inline
snd_rawmidi_type_t snd_rawmidi_type(snd_rawmidi_t *rmidi)
{
	return rmidi->hw->type;
}

static inline
snd_rawmidi_stream_t snd_rawmidi_stream(snd_rawmidi_t *rmidi)
{
	return rmidi->stream;
}

#endif /* __ALSA_RAWMIDI_MACROS_H */
