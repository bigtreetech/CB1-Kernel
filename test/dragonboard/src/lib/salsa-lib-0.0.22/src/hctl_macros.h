/*
 * h-control privates and macros
 */

#ifndef __ALSA_HCTL_MACROS_H
#define __ALSA_HCTL_MACROS_H

#include "asound.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

struct _snd_hctl {
	snd_ctl_t *ctl;
	snd_hctl_callback_t callback;
	void *callback_private;
	snd_hctl_elem_t *first_elem;
	snd_hctl_elem_t *last_elem;
	unsigned int count;
};

struct _snd_hctl_elem {
	snd_ctl_elem_id_t id;
	snd_hctl_t *hctl;
	void *private_data;
	snd_hctl_elem_callback_t callback;
	void *callback_private;
	snd_hctl_elem_t *prev;
	snd_hctl_elem_t *next;
};


static inline
int snd_hctl_wait(snd_hctl_t *hctl, int timeout)
{
	int err;
	do {
		err = snd_ctl_wait(hctl->ctl, timeout);
	} while (err == -EINTR);
	return err;
}

static inline
const char *snd_hctl_name(snd_hctl_t *hctl)
{
	return snd_ctl_name(hctl->ctl);
}

static inline
int snd_hctl_nonblock(snd_hctl_t *hctl, int nonblock)
{
	return snd_ctl_nonblock(hctl->ctl, nonblock);
}

static inline
int snd_hctl_set_compare(snd_hctl_t *hctl, snd_hctl_compare_t compare)
{
	return 0;
}

static inline
int snd_hctl_poll_descriptors_count(snd_hctl_t *hctl)
{
	return snd_ctl_poll_descriptors_count(hctl->ctl);
}

static inline
int snd_hctl_poll_descriptors(snd_hctl_t *hctl, struct pollfd *pfds, unsigned int space)
{
	return snd_ctl_poll_descriptors(hctl->ctl, pfds, space);
}

static inline
int snd_hctl_poll_descriptors_revents(snd_hctl_t *hctl, struct pollfd *pfds, unsigned int nfds, unsigned short *revents)
{
	return snd_ctl_poll_descriptors_revents(hctl->ctl, pfds, nfds, revents);
}

static inline
void snd_hctl_set_callback(snd_hctl_t *hctl, snd_hctl_callback_t callback)
{
	hctl->callback = callback;
}

static inline
void snd_hctl_set_callback_private(snd_hctl_t *hctl, void *callback_private)
{
	hctl->callback_private = callback_private;
}

static inline
void *snd_hctl_get_callback_private(snd_hctl_t *hctl)
{
	return hctl->callback_private;
}

static inline
unsigned int snd_hctl_get_count(snd_hctl_t *hctl)
{
	return hctl->count;
}

static inline
snd_ctl_t *snd_hctl_ctl(snd_hctl_t *hctl)
{
	return hctl->ctl;
}

static inline
int snd_hctl_elem_info(snd_hctl_elem_t *elem, snd_ctl_elem_info_t *info)
{
	info->id = elem->id;
	return snd_ctl_elem_info(elem->hctl->ctl, info);
}

static inline
int snd_hctl_elem_read(snd_hctl_elem_t *elem, snd_ctl_elem_value_t *value)
{
	value->id = elem->id;
	return snd_ctl_elem_read(elem->hctl->ctl, value);
}

static inline
int snd_hctl_elem_write(snd_hctl_elem_t *elem, snd_ctl_elem_value_t *value)
{
	value->id = elem->id;
	return snd_ctl_elem_write(elem->hctl->ctl, value);
}

#if SALSA_HAS_TLV_SUPPORT
static inline
int snd_hctl_elem_tlv_read(snd_hctl_elem_t *elem, unsigned int *tlv,
			   unsigned int tlv_size)
{
	return snd_ctl_elem_tlv_read(elem->hctl->ctl, &elem->id, tlv, tlv_size);
}

static inline
int snd_hctl_elem_tlv_write(snd_hctl_elem_t *elem, const unsigned int *tlv)
{
	return snd_ctl_elem_tlv_write(elem->hctl->ctl, &elem->id, tlv);
}

static inline
int snd_hctl_elem_tlv_command(snd_hctl_elem_t *elem, const unsigned int *tlv)
{
	return snd_ctl_elem_tlv_command(elem->hctl->ctl, &elem->id, tlv);
}

#else /* SALSA_HAS_TLV_SUPPORT */

static inline __SALSA_NOT_IMPLEMENTED
int snd_hctl_elem_tlv_read(snd_hctl_elem_t *elem, unsigned int *tlv,
			   unsigned int tlv_size)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_hctl_elem_tlv_write(snd_hctl_elem_t *elem, const unsigned int *tlv)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_hctl_elem_tlv_command(snd_hctl_elem_t *elem, const unsigned int *tlv)
{
	return -ENXIO;
}
#endif /* SALSA_HAS_TLV_SUPPORT */

static inline
snd_hctl_t *snd_hctl_elem_get_hctl(snd_hctl_elem_t *elem)
{
	return elem->hctl;
}

static inline
void snd_hctl_elem_get_id(const snd_hctl_elem_t *obj, snd_ctl_elem_id_t *ptr)
{
	*ptr = obj->id;
}

static inline
unsigned int snd_hctl_elem_get_numid(const snd_hctl_elem_t *obj)
{
	return obj->id.numid;
}

static inline
snd_ctl_elem_iface_t snd_hctl_elem_get_interface(const snd_hctl_elem_t *obj)
{
	return (snd_ctl_elem_iface_t) obj->id.iface;
}

static inline
unsigned int snd_hctl_elem_get_device(const snd_hctl_elem_t *obj)
{
	return obj->id.device;
}

static inline
unsigned int snd_hctl_elem_get_subdevice(const snd_hctl_elem_t *obj)
{
	return obj->id.subdevice;
}

static inline
const char *snd_hctl_elem_get_name(const snd_hctl_elem_t *obj)
{
	return (const char *)obj->id.name;
}

static inline
unsigned int snd_hctl_elem_get_index(const snd_hctl_elem_t *obj)
{
	return obj->id.index;
}

static inline
void snd_hctl_elem_set_callback(snd_hctl_elem_t *obj,
				snd_hctl_elem_callback_t val)
{
	obj->callback = val;
}

static inline
void snd_hctl_elem_set_callback_private(snd_hctl_elem_t *obj, void * val)
{
	obj->callback_private = val;
}

static inline
void *snd_hctl_elem_get_callback_private(const snd_hctl_elem_t *obj)
{
	return obj->callback_private;
}
static inline
snd_hctl_elem_t *snd_hctl_first_elem(snd_hctl_t *hctl)
{
	return hctl->first_elem;
}

static inline
snd_hctl_elem_t *snd_hctl_last_elem(snd_hctl_t *hctl)
{
	return hctl->last_elem;
}

static inline
snd_hctl_elem_t *snd_hctl_elem_next(snd_hctl_elem_t *elem)
{
	return elem->next;
}

static inline
snd_hctl_elem_t *snd_hctl_elem_prev(snd_hctl_elem_t *elem)
{
	return elem->prev;
}


/*
 * not implemented yet
 */

static inline __SALSA_NOT_IMPLEMENTED
int snd_hctl_async(snd_hctl_t *hctl, int sig, pid_t pid)
{
	return -ENXIO;
}

#endif /* __ALSA_HCTL_MACROS_H */
