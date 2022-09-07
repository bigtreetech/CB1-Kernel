/*
 * mixer privates and macros
 */

#ifndef __ALSA_MIXER_MACROS_H
#define __ALSA_MIXER_MACROS_H

#include "asound.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>

struct _snd_mixer {
	snd_hctl_t *hctl;
	int count;
	int alloc;
	snd_mixer_elem_t **pelems;
	snd_mixer_compare_t compare;
	unsigned int events;
	snd_mixer_callback_t callback;
	void *callback_private;
};

typedef struct _snd_selem_item_head {
	snd_hctl_elem_t *helem;
	unsigned int numid;
	unsigned int channels;
} snd_selem_item_head_t;

typedef struct _snd_selem_vol_item {
	struct _snd_selem_item_head head;
	long min, max;
	long raw_min, raw_max;
	unsigned int *db_info;
	long vol[0];
} snd_selem_vol_item_t;

typedef struct _snd_selem_sw_item {
	struct _snd_selem_item_head head;
	unsigned int sw;
} snd_selem_sw_item_t;

typedef struct _snd_selem_enum_item {
	struct _snd_selem_item_head head;
	unsigned int items;
	unsigned int item[0];
} snd_selem_enum_item_t;

/* Item types */
enum {
	SND_SELEM_ITEM_PVOLUME,
	SND_SELEM_ITEM_CVOLUME,
	SND_SELEM_ITEM_PSWITCH,
	SND_SELEM_ITEM_CSWITCH,
	SND_SELEM_ITEM_ENUM,
	SND_SELEM_ITEMS,
};

/* CAPS bits */
#define SND_SM_CAP_VOLUME_SHIFT		0
#define SND_SM_CAP_SWITCH_SHIFT		4
#define SND_SM_CAP_ENUM_SHIFT		8
#define SND_SM_CAP_PVOLUME	(1 << SND_SM_CAP_VOLUME_SHIFT)
#define SND_SM_CAP_CVOLUME	(2 << SND_SM_CAP_VOLUME_SHIFT)
#define SND_SM_CAP_GVOLUME	(4 << SND_SM_CAP_VOLUME_SHIFT)
#define SND_SM_CAP_PSWITCH	(1 << SND_SM_CAP_SWITCH_SHIFT)
#define SND_SM_CAP_CSWITCH	(2 << SND_SM_CAP_SWITCH_SHIFT)
#define SND_SM_CAP_GSWITCH	(4 << SND_SM_CAP_SWITCH_SHIFT)
#define SND_SM_CAP_PENUM	(1 << SND_SM_CAP_ENUM_SHIFT)
#define SND_SM_CAP_CENUM	(2 << SND_SM_CAP_ENUM_SHIFT)

struct _snd_mixer_selem_id {
	char name[60];
	unsigned int index;
};

struct _snd_mixer_elem {
	snd_mixer_selem_id_t sid;
	snd_mixer_t *mixer;
	unsigned int caps;
	unsigned int inactive;
	unsigned int channels[2];
	void *items[SND_SELEM_ITEMS];
	unsigned int index;
	snd_mixer_elem_callback_t callback;
	void *callback_private;
};


static inline
int snd_mixer_poll_descriptors_count(snd_mixer_t *mixer)
{
	return 1;
}

static inline
int snd_mixer_poll_descriptors(snd_mixer_t *mixer, struct pollfd *pfds,
			       unsigned int space)
{
	return snd_hctl_poll_descriptors(mixer->hctl, pfds, space);
}

static inline
int snd_mixer_poll_descriptors_revents(snd_mixer_t *mixer, struct pollfd *pfds,
				       unsigned int nfds,
				       unsigned short *revents)
{
	return snd_hctl_poll_descriptors_revents(mixer->hctl, pfds, nfds, revents);
}

static inline
snd_mixer_elem_t *snd_mixer_first_elem(snd_mixer_t *mixer)
{
	return mixer->pelems[0];
}

static inline
snd_mixer_elem_t *snd_mixer_last_elem(snd_mixer_t *mixer)
{
	return mixer->pelems[mixer->count - 1];
}

static inline
snd_mixer_elem_t *snd_mixer_elem_next(snd_mixer_elem_t *elem)
{
	if (elem->index == (unsigned int)elem->mixer->count - 1)
		return NULL;
	return elem->mixer->pelems[elem->index + 1];
}

static inline
snd_mixer_elem_t *snd_mixer_elem_prev(snd_mixer_elem_t *elem)
{
	if (!elem->index)
		return NULL;
	return elem->mixer->pelems[elem->index - 1];
}

static inline
void snd_mixer_set_callback(snd_mixer_t *obj, snd_mixer_callback_t val)
{
	obj->callback = val;
}

static inline
void snd_mixer_set_callback_private(snd_mixer_t *mixer, void * val)
{
	mixer->callback_private = val;
}

static inline
void * snd_mixer_get_callback_private(const snd_mixer_t *mixer)
{
	return mixer->callback_private;
}

static inline
unsigned int snd_mixer_get_count(const snd_mixer_t *mixer)
{
	return mixer->count;
}

static inline
void snd_mixer_elem_set_callback(snd_mixer_elem_t *mixer,
				 snd_mixer_elem_callback_t val)
{
	mixer->callback = val;
}

static inline
void snd_mixer_elem_set_callback_private(snd_mixer_elem_t *mixer, void * val)
{
	mixer->callback_private = val;
}

static inline
void * snd_mixer_elem_get_callback_private(const snd_mixer_elem_t *mixer)
{
	return mixer->callback_private;
}

static inline
snd_mixer_elem_type_t snd_mixer_elem_get_type(const snd_mixer_elem_t *mixer)
{
	return SND_MIXER_ELEM_SIMPLE;
}

static inline
int snd_mixer_set_compare(snd_mixer_t *mixer, snd_mixer_compare_t compare)
{
	mixer->compare = compare;
	return 0;
}

static inline
int snd_mixer_wait(snd_mixer_t *mixer, int timeout)
{
	struct pollfd pfd;
	snd_mixer_poll_descriptors(mixer, &pfd, 1);
	if (poll(&pfd, 1, timeout) < 0)
		return -errno;
	return 0;
}

int _snd_mixer_elem_throw_event(snd_mixer_elem_t *elem, unsigned int mask);

static inline
int snd_mixer_elem_info(snd_mixer_elem_t *elem)
{
	return _snd_mixer_elem_throw_event(elem, SND_CTL_EVENT_MASK_INFO);
}

static inline
int snd_mixer_elem_value(snd_mixer_elem_t *elem)
{
	return _snd_mixer_elem_throw_event(elem, SND_CTL_EVENT_MASK_VALUE);
}

static inline
int snd_mixer_get_hctl(snd_mixer_t *mixer, const char *name, snd_hctl_t **hctl)
{
	if (mixer->hctl) {
		*hctl = mixer->hctl;
		return 0;
	}
	return -ENOENT;
}

static inline
void snd_mixer_selem_get_id(snd_mixer_elem_t *elem,
			    snd_mixer_selem_id_t *id)
{
	*id = elem->sid;
}

static inline
const char *snd_mixer_selem_get_name(snd_mixer_elem_t *elem)
{
	return elem->sid.name;
}

static inline
unsigned int snd_mixer_selem_get_index(snd_mixer_elem_t *elem)
{
	return elem->sid.index;
}

static inline
int snd_mixer_selem_has_common_volume(snd_mixer_elem_t *elem)
{
	return !!(elem->caps & SND_SM_CAP_GVOLUME);
}

static inline
int snd_mixer_selem_has_common_switch(snd_mixer_elem_t *elem)
{
	return !!(elem->caps & SND_SM_CAP_GSWITCH);
}

extern const char *_snd_mixer_selem_channels[];

static inline
const char *snd_mixer_selem_channel_name(snd_mixer_selem_channel_id_t channel)
{
	const char *p;
	p = _snd_mixer_selem_channels[channel];
	return p ? p : "?";
}

static inline
int snd_mixer_selem_is_active(snd_mixer_elem_t *elem)
{
	return !elem->inactive;
}

static inline
int snd_mixer_selem_is_playback_mono(snd_mixer_elem_t *elem)
{
	return elem->channels[SND_PCM_STREAM_PLAYBACK] == 1;
}

static inline
int snd_mixer_selem_has_playback_channel(snd_mixer_elem_t *elem,
					 snd_mixer_selem_channel_id_t channel)
{
	return (unsigned int)channel < elem->channels[SND_PCM_STREAM_PLAYBACK];
}

static inline
int _snd_mixer_selem_get_volume_range(snd_mixer_elem_t *elem, int type,
				      long *min, long *max)
{
	snd_selem_vol_item_t *vol = (snd_selem_vol_item_t *)elem->items[type];
	if (!vol)
		return -EINVAL;
	*min = vol->min;
	*max = vol->max;
	return 0;
}

static inline
int snd_mixer_selem_get_playback_volume_range(snd_mixer_elem_t *elem,
					      long *min, long *max)
{
	return _snd_mixer_selem_get_volume_range(elem, SND_SELEM_ITEM_PVOLUME,
						 min, max);
}

static inline
int snd_mixer_selem_has_playback_volume(snd_mixer_elem_t *elem)
{
	return elem->items[SND_SELEM_ITEM_PVOLUME] != NULL;
}

static inline
int _snd_mixer_selem_has_joined(snd_mixer_elem_t *elem, int type, int str)
{
	snd_selem_item_head_t *head = (snd_selem_item_head_t *)elem->items[type];
	return head && (head->channels < elem->channels[str]);
}

static inline
int snd_mixer_selem_has_playback_volume_joined(snd_mixer_elem_t *elem)
{
	return _snd_mixer_selem_has_joined(elem, SND_SELEM_ITEM_PVOLUME,
					   SND_PCM_STREAM_PLAYBACK);
}

static inline
int snd_mixer_selem_has_playback_switch(snd_mixer_elem_t *elem)
{
	return elem->items[SND_SELEM_ITEM_PSWITCH] != NULL;
}

static inline
int snd_mixer_selem_has_playback_switch_joined(snd_mixer_elem_t *elem)
{
	return _snd_mixer_selem_has_joined(elem, SND_SELEM_ITEM_PSWITCH,
					   SND_PCM_STREAM_PLAYBACK);
}

static inline
int snd_mixer_selem_is_capture_mono(snd_mixer_elem_t *elem)
{
	return elem->channels[SND_PCM_STREAM_CAPTURE] == 1;
}

static inline
int snd_mixer_selem_has_capture_channel(snd_mixer_elem_t *elem,
					snd_mixer_selem_channel_id_t channel)
{
	return (unsigned int)channel < elem->channels[SND_PCM_STREAM_CAPTURE];
}

static inline
int snd_mixer_selem_get_capture_volume_range(snd_mixer_elem_t *elem,
					     long *min, long *max)
{
	return _snd_mixer_selem_get_volume_range(elem, SND_SELEM_ITEM_CVOLUME,
						 min, max);
}

static inline
int snd_mixer_selem_has_capture_volume(snd_mixer_elem_t *elem)
{
	return elem->items[SND_SELEM_ITEM_CVOLUME] != NULL;
}

static inline
int snd_mixer_selem_has_capture_volume_joined(snd_mixer_elem_t *elem)
{
	return _snd_mixer_selem_has_joined(elem, SND_SELEM_ITEM_CVOLUME,
					   SND_PCM_STREAM_CAPTURE);
}

static inline
int snd_mixer_selem_has_capture_switch(snd_mixer_elem_t *elem)
{
	return elem->items[SND_SELEM_ITEM_CSWITCH] != NULL;
}

static inline
int snd_mixer_selem_has_capture_switch_joined(snd_mixer_elem_t *elem)
{
	return _snd_mixer_selem_has_joined(elem, SND_SELEM_ITEM_CSWITCH,
					   SND_PCM_STREAM_CAPTURE);
}

static inline
int snd_mixer_selem_has_capture_switch_exclusive(snd_mixer_elem_t *elem)
{
	return 0;
}

static inline
int snd_mixer_selem_get_capture_group(snd_mixer_elem_t *elem)
{
	return -EINVAL;
}

static inline
int _snd_mixer_selem_get_volume(snd_mixer_elem_t *elem, int type,
				snd_mixer_selem_channel_id_t channel,
				long *value)
{
	snd_selem_vol_item_t *vol;
	vol = (snd_selem_vol_item_t *)elem->items[type];
	if (!vol)
		return -EINVAL;
	if ((unsigned int)channel >= vol->head.channels)
		channel = SND_MIXER_SCHN_FRONT_LEFT; /* = 0 */
	*value = vol->vol[channel << 1]; /* user-volume */
	return 0;
}

static inline
int snd_mixer_selem_get_playback_volume(snd_mixer_elem_t *elem,
					snd_mixer_selem_channel_id_t channel,
					long *value)
{
	return _snd_mixer_selem_get_volume(elem, SND_SELEM_ITEM_PVOLUME,
					   channel, value);
}

static inline
int _snd_mixer_selem_get_switch(snd_mixer_elem_t *elem, int type,
				snd_mixer_selem_channel_id_t channel,
				int *value)
{
	snd_selem_sw_item_t *sw;
	sw = (snd_selem_sw_item_t *) elem->items[type];
	if (!sw)
		return -EINVAL;
	if ((unsigned int)channel >= sw->head.channels)
		channel = SND_MIXER_SCHN_FRONT_LEFT; /* = 0 */
	*value = !!(sw->sw & (1 << channel));
	return 0;
}

static inline
int snd_mixer_selem_get_playback_switch(snd_mixer_elem_t *elem,
					snd_mixer_selem_channel_id_t channel,
					int *value)
{
	return _snd_mixer_selem_get_switch(elem, SND_SELEM_ITEM_PSWITCH,
					   channel, value);
}

static inline
int snd_mixer_selem_get_capture_volume(snd_mixer_elem_t *elem,
				       snd_mixer_selem_channel_id_t channel,
				       long *value)
{
	return _snd_mixer_selem_get_volume(elem, SND_SELEM_ITEM_CVOLUME,
					   channel, value);
}

static inline
int snd_mixer_selem_get_capture_switch(snd_mixer_elem_t *elem,
				       snd_mixer_selem_channel_id_t channel,
				       int *value)
{
	return _snd_mixer_selem_get_switch(elem, SND_SELEM_ITEM_CSWITCH,
					   channel, value);
}

static inline
int snd_mixer_selem_is_enumerated(snd_mixer_elem_t *elem)
{
	snd_selem_enum_item_t *eitem =
		(snd_selem_enum_item_t *) elem->items[SND_SELEM_ITEM_ENUM];
	return eitem != NULL;
}

static inline
int snd_mixer_selem_is_enum_playback(snd_mixer_elem_t *elem)
{
	return !!(elem->caps & SND_SM_CAP_PENUM);
}

static inline
int snd_mixer_selem_is_enum_capture(snd_mixer_elem_t *elem)
{
	return !!(elem->caps & SND_SM_CAP_CENUM);
}

static inline
int snd_mixer_selem_get_enum_items(snd_mixer_elem_t *elem)
{
	snd_selem_enum_item_t *eitem =
		(snd_selem_enum_item_t *) elem->items[SND_SELEM_ITEM_ENUM];
	if (!eitem)
		return -EINVAL;
	return eitem->items;
}

static inline
int snd_mixer_selem_get_enum_item(snd_mixer_elem_t *elem,
				  snd_mixer_selem_channel_id_t channel,
				  unsigned int *itemp)
{
	snd_selem_enum_item_t *eitem =
		(snd_selem_enum_item_t *) elem->items[SND_SELEM_ITEM_ENUM];
	if (!eitem || (unsigned int)channel >= eitem->head.channels)
		return -EINVAL;
	*itemp =  eitem->item[channel];
	return 0;
}


/*
 */

__snd_define_type(snd_mixer_selem_id);

static inline
const char *snd_mixer_selem_id_get_name(const snd_mixer_selem_id_t *obj)
{
	return obj->name;
}

static inline
unsigned int snd_mixer_selem_id_get_index(const snd_mixer_selem_id_t *obj)
{
	return obj->index;
}

static inline
void snd_mixer_selem_id_set_name(snd_mixer_selem_id_t *obj, const char *val)
{
	strncpy(obj->name, val, sizeof(obj->name));
	obj->name[sizeof(obj->name)-1] = '\0';
}

static inline
void snd_mixer_selem_id_set_index(snd_mixer_selem_id_t *obj, unsigned int val)
{
	obj->index = val;
}


/*
 * dB handler
 */
#if SALSA_HAS_TLV_SUPPORT
extern int _snd_selem_vol_get_dB_range(snd_selem_vol_item_t *item,
				       long *min, long *max);
extern int _snd_selem_vol_get_dB(snd_selem_vol_item_t *item, int channel,
				 long *value);
extern int _snd_selem_vol_set_dB(snd_selem_vol_item_t *item,
				 snd_mixer_selem_channel_id_t channel,
				 long db_gain, int xdir);
static inline
int snd_mixer_selem_get_playback_dB_range(snd_mixer_elem_t *elem,
					  long *min, long *max)
{
	return _snd_selem_vol_get_dB_range((snd_selem_vol_item_t *)elem->items[SND_SELEM_ITEM_PVOLUME],
					   min, max);
}

static inline
int snd_mixer_selem_get_playback_dB(snd_mixer_elem_t *elem,
				    snd_mixer_selem_channel_id_t channel,
				    long *value)
{
	return _snd_selem_vol_get_dB((snd_selem_vol_item_t *)elem->items[SND_SELEM_ITEM_PVOLUME],
				     channel, value);
}

static inline
int snd_mixer_selem_set_playback_dB(snd_mixer_elem_t *elem,
				    snd_mixer_selem_channel_id_t channel,
				    long value, int dir)
{
	return _snd_selem_vol_set_dB((snd_selem_vol_item_t *)elem->items[SND_SELEM_ITEM_PVOLUME],
				     channel, value, dir);
}

static inline
int snd_mixer_selem_set_playback_dB_all(snd_mixer_elem_t *elem, long value,
					int dir)
{
	unsigned int i;
	int err;
	snd_selem_vol_item_t *vol =
		(snd_selem_vol_item_t *) elem->items[SND_SELEM_ITEM_PVOLUME];
	if (!vol)
		return -EINVAL;
	for (i = 0; i < vol->head.channels; i++) {
		err = _snd_selem_vol_set_dB(vol,
					    (snd_mixer_selem_channel_id_t)i,
					    value, dir);
		if (err < 0)
			return err;
	}
	return 0;
}

static inline
int snd_mixer_selem_get_capture_dB_range(snd_mixer_elem_t *elem,
					 long *min, long *max)
{
	return _snd_selem_vol_get_dB_range((snd_selem_vol_item_t *)elem->items[SND_SELEM_ITEM_CVOLUME],
					   min, max);
}

static inline
int snd_mixer_selem_get_capture_dB(snd_mixer_elem_t *elem,
				   snd_mixer_selem_channel_id_t channel,
				   long *value)
{
	return _snd_selem_vol_get_dB((snd_selem_vol_item_t *)elem->items[SND_SELEM_ITEM_CVOLUME],
				     channel, value);
}

static inline
int snd_mixer_selem_set_capture_dB(snd_mixer_elem_t *elem,
				   snd_mixer_selem_channel_id_t channel,
				   long value, int dir)
{
	return _snd_selem_vol_set_dB((snd_selem_vol_item_t *)elem->items[SND_SELEM_ITEM_CVOLUME],
				     channel, value, dir);
}

static inline
int snd_mixer_selem_set_capture_dB_all(snd_mixer_elem_t *elem, long value,
				       int dir)
{
	unsigned int i;
	int err;
	snd_selem_vol_item_t *vol =
		(snd_selem_vol_item_t *) elem->items[SND_SELEM_ITEM_CVOLUME];
	if (!vol)
		return -EINVAL;
	for (i = 0; i < vol->head.channels; i++) {
		err = _snd_selem_vol_set_dB(vol,
					    (snd_mixer_selem_channel_id_t)i,
					    value, dir);
		if (err < 0)
			return err;
	}
	return 0;
}

#else /* SALSA_HAS_TLV_SUPPORT */
static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_get_playback_dB_range(snd_mixer_elem_t *elem,
					  long *min, long *max)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_get_playback_dB(snd_mixer_elem_t *elem,
				    snd_mixer_selem_channel_id_t channel,
				    long *value)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_set_playback_dB(snd_mixer_elem_t *elem,
				    snd_mixer_selem_channel_id_t channel,
				    long value, int dir)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_set_playback_dB_all(snd_mixer_elem_t *elem, long value,
					int dir)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_get_capture_dB_range(snd_mixer_elem_t *elem,
					 long *min, long *max)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_get_capture_dB(snd_mixer_elem_t *elem,
				   snd_mixer_selem_channel_id_t channel,
				   long *value)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_set_capture_dB(snd_mixer_elem_t *elem,
				   snd_mixer_selem_channel_id_t channel,
				   long value, int dir)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
int snd_mixer_selem_set_capture_dB_all(snd_mixer_elem_t *elem, long value,
				       int dir)
{
	return -ENXIO;
}
#endif /* SALSA_HAS_TLV_SUPPORT */

#endif /* __ALSA_MIXER_MACROS_H */
