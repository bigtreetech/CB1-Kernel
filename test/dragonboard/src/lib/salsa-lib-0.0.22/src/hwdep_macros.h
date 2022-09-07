/*
 * hwdep privates and macros
 */

#ifndef __ALSA_HWDEP_MACROS_H
#define __ALSA_HWDEP_MACROS_H

#include "asound.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

struct _snd_hwdep {
	const char *name;
	int type;
	int mode;
	int fd;
	struct pollfd pollfd;
};


static inline
int snd_hwdep_nonblock(snd_hwdep_t *hwdep, int nonblock)
{
	return _snd_set_nonblock(hwdep->fd, nonblock);
}

static inline
int snd_hwdep_poll_descriptors_count(snd_rawmidi_t *rmidi)
{
	return 1;
}

static inline
int snd_hwdep_poll_descriptors(snd_hwdep_t *hwdep, struct pollfd *pfds,
			       unsigned int space)
{
	*pfds = hwdep->pollfd;
	return 1;
}

static inline
int snd_hwdep_poll_descriptors_revents(snd_hwdep_t *hwdep, struct pollfd *pfds,
				       unsigned int nfds,
				       unsigned short *revents)
{
	*revents = pfds->events;
	return 0;
}

static inline
int snd_hwdep_info(snd_hwdep_t *hwdep, snd_hwdep_info_t * info)
{
	if (ioctl(hwdep->fd, SNDRV_HWDEP_IOCTL_INFO, info) < 0)
		return -errno;
	return 0;
}

static inline
int snd_hwdep_ioctl(snd_hwdep_t *hwdep, unsigned int request, void * arg)
{
	if (ioctl(hwdep->fd, request, arg) < 0)
		return -errno;
	return 0;
}

static inline
int snd_hwdep_dsp_status(snd_hwdep_t *hwdep, snd_hwdep_dsp_status_t *status)
{
	return snd_hwdep_ioctl(hwdep, SNDRV_HWDEP_IOCTL_DSP_STATUS, status);
}

static inline
int snd_hwdep_dsp_load(snd_hwdep_t *hwdep, snd_hwdep_dsp_image_t *block)
{
	return snd_hwdep_ioctl(hwdep, SNDRV_HWDEP_IOCTL_DSP_LOAD, block);
}

static inline
ssize_t snd_hwdep_write(snd_hwdep_t *hwdep, const void *buffer, size_t size)
{
	ssize_t result;
	result = write(hwdep->fd, buffer, size);
	if (result < 0)
		return -errno;
	return result;
}

static inline
ssize_t snd_hwdep_read(snd_hwdep_t *hwdep, void *buffer, size_t size)
{
	ssize_t result;
	result = read(hwdep->fd, buffer, size);
	if (result < 0)
		return -errno;
	return result;
}

__snd_define_type(snd_hwdep_info);

static inline
unsigned int snd_hwdep_info_get_device(const snd_hwdep_info_t *obj)
{
	return obj->device;
}

static inline
int snd_hwdep_info_get_card(const snd_hwdep_info_t *obj)
{
	return obj->card;
}

static inline
const char *snd_hwdep_info_get_id(const snd_hwdep_info_t *obj)
{
	return (const char *)obj->id;
}

static inline
const char *snd_hwdep_info_get_name(const snd_hwdep_info_t *obj)
{
	return (const char *)obj->name;
}

static inline
snd_hwdep_iface_t snd_hwdep_info_get_iface(const snd_hwdep_info_t *obj)
{
	return (snd_hwdep_iface_t) obj->iface;
}

static inline
void snd_hwdep_info_set_device(snd_hwdep_info_t *obj, unsigned int val)
{
	obj->device = val;
}

__snd_define_type(snd_hwdep_dsp_status);

static inline
unsigned int snd_hwdep_dsp_status_get_version(const snd_hwdep_dsp_status_t *obj)
{
	return obj->version;
}

static inline
const char *snd_hwdep_dsp_status_get_id(const snd_hwdep_dsp_status_t *obj)
{
	return (const char *)obj->id;
}

static inline
unsigned int snd_hwdep_dsp_status_get_num_dsps(const snd_hwdep_dsp_status_t *obj)
{
	return obj->num_dsps;
}

static inline
unsigned int snd_hwdep_dsp_status_get_dsp_loaded(const snd_hwdep_dsp_status_t *obj)
{
	return obj->dsp_loaded;
}

static inline
unsigned int snd_hwdep_dsp_status_get_chip_ready(const snd_hwdep_dsp_status_t *obj)
{
	return obj->chip_ready;
}

__snd_define_type(snd_hwdep_dsp_image);

static inline
unsigned int snd_hwdep_dsp_image_get_index(const snd_hwdep_dsp_image_t *obj)
{
	return obj->index;
}

static inline
const char *snd_hwdep_dsp_image_get_name(const snd_hwdep_dsp_image_t *obj)
{
	return (const char *)obj->name;
}

static inline
const void *snd_hwdep_dsp_image_get_image(const snd_hwdep_dsp_image_t *obj)
{
	return (const void *)obj->image;
}

static inline
size_t snd_hwdep_dsp_image_get_length(const snd_hwdep_dsp_image_t *obj)
{
	return obj->length;
}

static inline
void snd_hwdep_dsp_image_set_index(snd_hwdep_dsp_image_t *obj, unsigned int _index)
{
	obj->index = _index;
}

static inline
void snd_hwdep_dsp_image_set_name(snd_hwdep_dsp_image_t *obj, const char *name)
{
	strncpy((char *)obj->name, name, sizeof(obj->name));
	obj->name[sizeof(obj->name)-1] = 0;
}

static inline
void snd_hwdep_dsp_image_set_image(snd_hwdep_dsp_image_t *obj, void *buffer)
{
	obj->image = (unsigned char *) buffer;
}

static inline
void snd_hwdep_dsp_image_set_length(snd_hwdep_dsp_image_t *obj, size_t length)
{
	obj->length = length;
}

#endif /* __ALSA_HWDEP_MACROS_H */
