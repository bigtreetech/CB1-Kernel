/*
 *  SALSA-Lib - Mixer abstraction layer
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
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/poll.h>
#include "mixer.h"
#include "local.h"

static int hctl_event_handler(snd_hctl_t *hctl, unsigned int mask,
			      snd_hctl_elem_t *elem);

int snd_mixer_open(snd_mixer_t **mixerp, int mode)
{
	snd_mixer_t *mixer;
	mixer = calloc(1, sizeof(*mixer));
	*mixerp = mixer;
	if (mixer == NULL)
		return -ENOMEM;
	return 0;
}

int snd_mixer_attach(snd_mixer_t *mixer, const char *name)
{
	snd_hctl_t *hctl;
	int err;

	err = snd_hctl_open(&hctl, name, 0);
	if (err < 0)
		return err;
	err = snd_mixer_attach_hctl(mixer, hctl);
	if (err < 0) {
		snd_hctl_close(hctl);
		return err;
	}
	return 0;
}

int snd_mixer_attach_hctl(snd_mixer_t *mixer, snd_hctl_t *hctl)
{
	int err;

	if (mixer->hctl)
		return -EBUSY;
	err = snd_hctl_nonblock(hctl, 1);
	if (err < 0) {
		snd_hctl_close(hctl);
		return err;
	}
	snd_hctl_set_callback(hctl, hctl_event_handler);
	snd_hctl_set_callback_private(hctl, mixer);
	mixer->hctl = hctl;
	return 0;
}

int snd_mixer_detach(snd_mixer_t *mixer, const char *name)
{
	return snd_mixer_detach_hctl(mixer, mixer->hctl);
}

int snd_mixer_detach_hctl(snd_mixer_t *mixer, snd_hctl_t *hctl)
{
	if (mixer->hctl != hctl)
		return -ENOENT;
	snd_hctl_close(mixer->hctl);
	mixer->hctl = NULL;
	return 0;
}

int snd_mixer_load(snd_mixer_t *mixer)
{
	if (mixer->hctl)
		return snd_hctl_load(mixer->hctl);
	return 0;
}

void snd_mixer_free(snd_mixer_t *mixer)
{
	if (mixer->hctl)
		snd_hctl_free(mixer->hctl);
}

int snd_mixer_close(snd_mixer_t *mixer)
{
	if (mixer->hctl)
		snd_hctl_close(mixer->hctl);
	free(mixer->pelems);
	free(mixer);
	return 0;
}

int snd_mixer_handle_events(snd_mixer_t *mixer)
{
	int err;

	if (!mixer->hctl)
		return 0;
	mixer->events = 0;
	err = snd_hctl_handle_events(mixer->hctl);
	if (err < 0)
		return err;
	return mixer->events;
}

/*
 */
static int snd_mixer_throw_event(snd_mixer_t *mixer, unsigned int mask,
				 snd_mixer_elem_t *elem)
{
	mixer->events++;
	if (mixer->callback)
		return mixer->callback(mixer, mask, elem);
	return 0;
}

int _snd_mixer_elem_throw_event(snd_mixer_elem_t *elem, unsigned int mask)
{
	elem->mixer->events++;
	if (elem->callback)
		return elem->callback(elem, mask);
	return 0;
}

/*
 */

static int snd_mixer_sort(snd_mixer_t *mixer)
{
	int i;
	if (mixer->compare)
		qsort(mixer->pelems, mixer->count, sizeof(snd_mixer_elem_t *),
		      (int (*)(const void*, const void *))mixer->compare);
	for (i = 0; i < mixer->count; i++)
		mixer->pelems[i]->index = i;
	return 0;
}

static int add_mixer_elem(snd_mixer_elem_t *elem, snd_mixer_t *mixer)
{
	if (mixer->count == mixer->alloc) {
		snd_mixer_elem_t **m;
		int num = mixer->alloc + 32;
		m = realloc(mixer->pelems, sizeof(*m) * num);
		if (!m)
			return -ENOMEM;
		mixer->pelems = m;
		mixer->alloc = num;
	}
	mixer->pelems[mixer->count] = elem;
	mixer->count++;
	snd_mixer_sort(mixer);

	return snd_mixer_throw_event(mixer, SND_CTL_EVENT_MASK_ADD, elem);
}

static int remove_mixer_elem(snd_mixer_elem_t *elem)
{
	snd_mixer_t *mixer = elem->mixer;
	int i;

	for (i = 0; i < mixer->count; i++) {
		if (mixer->pelems[i] == elem) {
			mixer->count--;
			for (; i < mixer->count; i++) {
				mixer->pelems[i] = mixer->pelems[i + 1];
				mixer->pelems[i]->index = i;
			}
			break;
		}
	}
	free(elem);
	return 0;
}

/*
 */
static int add_simple_element(snd_hctl_elem_t *hp, snd_mixer_t *mixer);
static int remove_simple_element(snd_hctl_elem_t *hp, int remove_mixer);
static int update_simple_element(snd_hctl_elem_t *hp, snd_mixer_elem_t *elem);

static int hctl_elem_event_handler(snd_hctl_elem_t *helem,
				   unsigned int mask)
{
	snd_mixer_elem_t *elem = helem->private_data;
	snd_mixer_t *mixer = elem->mixer;
	int err;

	if (!elem)
		return 0;

	if (mask == SND_CTL_EVENT_MASK_REMOVE) {
		err = _snd_mixer_elem_throw_event(elem, mask);
		if (err < 0)
			return err;
		remove_simple_element(helem, 1);
		return 0;
	}
	if (mask & SND_CTL_EVENT_MASK_INFO) {
		remove_simple_element(helem, 0);
		add_simple_element(helem, mixer);
		err = snd_mixer_elem_info(elem);
		if (err < 0)
			return err;
		snd_mixer_sort(mixer);
	}
	if (mask & SND_CTL_EVENT_MASK_VALUE) {
		err = update_simple_element(helem, elem);
		if (err < 0)
			return err;
		if (err) {
			err = snd_mixer_elem_value(elem);
			if (err < 0)
				return err;
		}
	}
	return 0;
}

static int hctl_event_handler(snd_hctl_t *hctl, unsigned int mask,
			      snd_hctl_elem_t *elem)
{
	snd_mixer_t *mixer = snd_hctl_get_callback_private(hctl);
	if (elem->id.iface != SND_CTL_ELEM_IFACE_MIXER)
		return 0;
	if (mask & SND_CTL_EVENT_MASK_ADD) {
		snd_hctl_elem_set_callback(elem, hctl_elem_event_handler);
		return add_simple_element(elem, mixer);
	}
	return 0;
}


/*
 */
static long convert_from_user(snd_selem_vol_item_t *str, long val)
{
	if (str->min == str->raw_min && str->max == str->raw_max)
		return val;
	if (str->min == str->max)
		return str->raw_min;
	val -= str->min;
	val *= str->raw_max - str->raw_min;
	val /= str->max - str->min;
	val += str->raw_min;
	return val;
}

static long convert_to_user(snd_selem_vol_item_t *str, long val)
{
	if (str->min == str->raw_min && str->max == str->raw_max)
		return val;
	if (str->raw_min == str->raw_max)
		return str->min;
	val -= str->raw_min;
	val *= str->max - str->min;
	val /= str->raw_max - str->raw_min;
	val += str->min;
	return val;
}

/*
 */

static void add_cap(snd_mixer_elem_t *elem, unsigned int caps, int type,
		    snd_selem_item_head_t *item)
{
	elem->caps |= caps;
	elem->items[type] = item;
	item->helem->private_data = elem;
	if (type == SND_SELEM_ITEM_ENUM)
		return;
	/* duplicate item for global volume and switch */
	if (caps & (SND_SM_CAP_GVOLUME | SND_SM_CAP_GSWITCH))
		elem->items[type + 1] = item;
	/* check the max channels for each direction */
	type &= 1;
	if (item->channels > elem->channels[type])
		elem->channels[type] = item->channels;
	if (caps & (SND_SM_CAP_GVOLUME | SND_SM_CAP_GSWITCH)) {
		/* global item needs check of both directions */
		type += 1;
		if (item->channels > elem->channels[type])
			elem->channels[type] = item->channels;
	}
}

static snd_mixer_elem_t *new_mixer_elem(const char *name, int index,
					snd_mixer_t *mixer)
{
	snd_mixer_elem_t *elem;

	elem = calloc(1, sizeof(*elem));
	if (!elem)
		return NULL;
	elem->mixer = mixer;
	strncpy(elem->sid.name, name, sizeof(elem->sid.name) - 1);
	elem->sid.index = index;
	return elem;
}

/*
 */

#define USR_IDX(i)	((i) << 1)
#define RAW_IDX(i)	(((i) << 1) + 1)

#define INVALID_DB_INFO		(unsigned int *)(-1)


/*
 * create a volume item
 */
static snd_selem_item_head_t *new_selem_vol_item(snd_ctl_elem_info_t *info,
						 snd_ctl_elem_value_t *val)
{
	snd_selem_vol_item_t *vol;
	int i;

	vol = malloc(sizeof(*vol) + sizeof(long) * 2 * info->count);
	if (!vol)
		return NULL;
	vol->raw_min = info->value.integer.min;
	vol->min = vol->raw_min;
	vol->raw_max = info->value.integer.max;
	vol->max = vol->raw_max;
	vol->db_info = NULL;
	/* set initial values */
	for (i = 0; i < info->count; i++)
		vol->vol[USR_IDX(i)] = vol->vol[RAW_IDX(i)] =
			snd_ctl_elem_value_get_integer(val, i);
	return &vol->head;
}

/*
 * create a switch item
 */
static snd_selem_item_head_t *new_selem_sw_item(snd_ctl_elem_info_t *info,
						snd_ctl_elem_value_t *val)
{
	snd_selem_sw_item_t *sw;
	int i;

	sw = malloc(sizeof(*sw));
	if (!sw)
		return NULL;
	/* set initial values */
	sw->sw = 0;
	for (i = 0; i < info->count; i++)
		if (snd_ctl_elem_value_get_boolean(val, i))
			sw->sw |= (1 << i);
	return &sw->head;
}

/*
 * create an enum item
 */
static snd_selem_item_head_t *new_selem_enum_item(snd_ctl_elem_info_t *info,
						  snd_ctl_elem_value_t *val)
{
	snd_selem_enum_item_t *eitem;
	int i;

	eitem = malloc(sizeof(*eitem) + sizeof(unsigned int) * info->count);
	if (!eitem)
		return NULL;
	eitem->items = info->value.enumerated.items;
	for (i = 0; i < info->count; i++)
		eitem->item[i] = snd_ctl_elem_value_get_enumerated(val, i);
	return &eitem->head;
}

/*
 * create a mixer item
 */
static snd_selem_item_head_t *new_selem_item(snd_hctl_elem_t *hp,
					     snd_ctl_elem_info_t *info)
{
	snd_ctl_elem_value_t *val;
	snd_selem_item_head_t *item;

	snd_ctl_elem_value_alloca(&val);
	val->id = info->id;
	if (snd_hctl_elem_read(hp, val) < 0)
		return NULL;

	switch (snd_ctl_elem_info_get_type(info)) {
	case SND_CTL_ELEM_TYPE_INTEGER:
		item = new_selem_vol_item(info, val);
		break;
	case SND_CTL_ELEM_TYPE_BOOLEAN:
		item = new_selem_sw_item(info, val);
		break;
	case SND_CTL_ELEM_TYPE_ENUMERATED:
		item = new_selem_enum_item(info, val);
		break;
	default:
		return NULL;
	}

	if (!item)
		return NULL;
	item->helem = hp;
	item->numid = info->id.numid;
	item->channels = info->count;
	return item;
}


/*
 * check the alias of element names
 */
static struct mixer_name_alias {
	const char *longname;
	const char *shortname;
	int dir;
} name_alias[] = {
	{"Tone Control - Switch", "Tone", 0},
	{"Tone Control - Bass", "Bass", 0},
	{"Tone Control - Treble", "Treble", 0},
	{"Synth Tone Control - Switch", "Synth Tone", 0},
	{"Synth Tone Control - Bass", "Synth Bass", 0},
	{"Synth Tone Control - Treble", "Synth Treble", 0},
	{},
};

static int check_elem_alias(char *name, int *dirp)
{
	struct mixer_name_alias *p;

	for (p = name_alias; p->longname; p++) {
		if (!strcmp(name, p->longname)) {
			strcpy(name, p->shortname);
			*dirp = p->dir;
			return 1;
		}
	}
	return 0;
}

/*
 * remove a suffix from the given string
 */
static int remove_suffix(char *name, const char *sfx)
{
	char *p;
	p = strstr(name, sfx);
	if (!p)
		return 0;
	if (strcmp(p, sfx))
		return 0;
	*p = 0;
	return 1;
}


/*
 * add an item to the mixer element
 */
static int add_simple_element(snd_hctl_elem_t *hp, snd_mixer_t *mixer)

{
	char name[64];
	int dir = 0, gflag = 0, type, i, err;
	unsigned int caps, index;
	snd_ctl_elem_info_t *info;
	snd_selem_item_head_t *item;
	snd_mixer_elem_t *elem;

	strncpy(name, (char *)hp->id.name, sizeof(name) - 1);
	name[sizeof(name)-1] = 0;

	if (check_elem_alias(name, &dir))
		goto found;

	/* remove standard suffix */
	remove_suffix(name, " Volume");
	remove_suffix(name, " Switch");
	/* check direction */
	if (remove_suffix(name, " Playback"))
		dir = 0;
	else if (remove_suffix(name, " Capture"))
		dir = 1;
	else if (!strncmp(name, "Capture", strlen("Capture")))
		dir = 1;
	else if (!strncmp(name, "Input", strlen("Input")))
		dir = 1;
	else
		gflag = 1; /* global (common): both playback and capture */
 found:
	/* acquire the element information */
	snd_ctl_elem_info_alloca(&info);
	err = snd_hctl_elem_info(hp, info);
	if (err < 0)
		return err;

	if (gflag)
		caps = 7; /* GLOBAL|PLAY|CAPT */
	else
		caps = 1 << dir; /* PLAY or CAPT */
	switch (snd_ctl_elem_info_get_type(info)) {
	case SND_CTL_ELEM_TYPE_BOOLEAN:
		caps <<= SND_SM_CAP_SWITCH_SHIFT;
		type = (dir ? SND_SELEM_ITEM_CSWITCH : SND_SELEM_ITEM_PSWITCH);
		break;
	case SND_CTL_ELEM_TYPE_INTEGER:
		caps <<= SND_SM_CAP_VOLUME_SHIFT;
		type = (dir ? SND_SELEM_ITEM_CVOLUME : SND_SELEM_ITEM_PVOLUME);
		break;
	case SND_CTL_ELEM_TYPE_ENUMERATED:
		/* grrr, enum has no global type */
		caps = 1 << (dir + SND_SM_CAP_ENUM_SHIFT);
		type = SND_SELEM_ITEM_ENUM;
		break;
	default:
		return 0; /* ignore this element */
	}

	item = new_selem_item(hp, info);
	if (!item)
		return -ENOMEM;

	/* check matching element */
	index = 0;
	for (i = 0; i < mixer->count; i++) {
		elem = mixer->pelems[i];
		if (!strcmp(elem->sid.name, name)) {
			if (index <= elem->sid.index)
				index = elem->sid.index + 1;
			/* already occupied? */
			if (elem->items[type])
				continue;
			add_cap(elem, caps, type, item);
			return 0;
		}
	}
	/* no element found, create a new one */
	elem = new_mixer_elem(name, index, mixer);
	if (!elem) {
		free(item);
		return -ENOMEM;
	}
	add_cap(elem, caps, type, item);
	return add_mixer_elem(elem, mixer);
}

/*
 * remote the corresponding item from the mixer element
 * if remove_mixer is non-zero, removes the mixer element after
 * all items have been removed.
 */
static int remove_simple_element(snd_hctl_elem_t *hp, int remove_mixer)
{
	snd_mixer_elem_t *elem = hp->private_data;
	int i;

	for (i = 0; i < SND_SELEM_ITEMS; i++) {
		snd_selem_item_head_t *head = elem->items[i];
		if (!head)
			continue;
		if (head->helem != hp)
			continue;
		elem->items[i] = NULL;
		switch (i) {
		case SND_SELEM_ITEM_PVOLUME:
			if (elem->caps & (SND_SM_CAP_GVOLUME))
				elem->items[SND_SELEM_ITEM_CVOLUME] = NULL;
			break;
		case SND_SELEM_ITEM_PSWITCH:
			if (elem->caps & (SND_SM_CAP_GSWITCH))
				elem->items[SND_SELEM_ITEM_CSWITCH] = NULL;
			break;
		}

#if SALSA_HAS_TLV_SUPPORT
		if (i <= SND_SELEM_ITEM_CVOLUME) {
			snd_selem_vol_item_t *vol =
				(snd_selem_vol_item_t *)head;
			/* release db_info entry if needed */
			if (vol->db_info && vol->db_info != INVALID_DB_INFO)
				free(vol->db_info);
		}
#endif
		free(head);

		if (!remove_mixer)
			return 0;

		/* check whether all items have been removed */
		for (i = 0; i < SND_SELEM_ITEMS; i++) {
			if (elem->items[i])  /* not yet */
				return 0;
		}
		remove_mixer_elem(elem);
		return 0;
	}
	return -ENOENT;
}

/*
 * update the values of the volume item
 */
static int update_selem_vol_item(snd_mixer_elem_t *elem,
				 snd_selem_vol_item_t *vol,
				 snd_ctl_elem_value_t *obj)
{
	int i, changed = 0;

	for (i = 0; i < vol->head.channels; i++) {
		long val;
		val = snd_ctl_elem_value_get_integer(obj, i);
		changed |= (vol->vol[RAW_IDX(i)] != val);
		vol->vol[RAW_IDX(i)] = val;
		val = convert_to_user(vol, val);
		changed |= (vol->vol[USR_IDX(i)] != val);
		vol->vol[USR_IDX(i)] = val;
	}
	return changed;
}

/*
 * update the values of the switch item
 */
static int update_selem_sw_item(snd_mixer_elem_t *elem,
				snd_selem_sw_item_t *sw,
				snd_ctl_elem_value_t *val)
{
	int i, changed;
	unsigned int swbits = 0;

	for (i = 0; i < sw->head.channels; i++) {
		if (snd_ctl_elem_value_get_boolean(val, i))
			swbits |= (1 << i);
	}
	changed = (sw->sw != swbits);
	sw->sw = swbits;
	return changed;
}

/*
 * update the values of the enum item
 */
static int update_selem_enum_item(snd_mixer_elem_t *elem,
				  snd_selem_enum_item_t *eitem,
				  snd_ctl_elem_value_t *val)
{
	int i, changed = 0;

	for (i = 0; i < eitem->head.channels; i++) {
		unsigned int item =
			snd_ctl_elem_value_get_enumerated(val, i);
		changed |= eitem->item[i] != item;
		eitem->item[i] = item;
	}
	return changed;
}

/*
 * update the values of the item
 */
static int update_selem_item(snd_mixer_elem_t *elem, int type)
{
	snd_selem_item_head_t *head;
	snd_ctl_elem_value_t *val;

	snd_ctl_elem_value_alloca(&val);
	head = elem->items[type];
	val->id.numid = head->numid;
	if (snd_hctl_elem_read(head->helem, val) < 0)
		return 0;

	switch (type) {
	case SND_SELEM_ITEM_PVOLUME:
	case SND_SELEM_ITEM_CVOLUME:
		return update_selem_vol_item(elem, elem->items[type], val);
	case SND_SELEM_ITEM_PSWITCH:
	case SND_SELEM_ITEM_CSWITCH:
		return update_selem_sw_item(elem, elem->items[type], val);
	default:
		return update_selem_enum_item(elem, elem->items[type], val);
	}
}

/*
 * update the given item of the mixer element
 */
static int update_simple_element(snd_hctl_elem_t *hp, snd_mixer_elem_t *elem)
{
	int i;

	for (i = 0; i < SND_SELEM_ITEMS; i++) {
		snd_selem_item_head_t *head = elem->items[i];
		if (!head)
			continue;
		if (head->helem != hp)
			continue;
		return update_selem_item(elem, i);
	}
	return 0;
}

/*
 */

int snd_mixer_selem_register(snd_mixer_t *mixer,
			     struct snd_mixer_selem_regopt *options,
			     snd_mixer_class_t **classp)
{
	int err;

	if (classp ||
	    (options && options->abstract != SND_MIXER_SABSTRACT_NONE))
		return -EINVAL;
	if (options && options->device) {
		err = snd_mixer_attach(mixer, options->device);
		if (err < 0)
			return err;
	}
	return 0;
}

snd_mixer_elem_t *snd_mixer_find_selem(snd_mixer_t *mixer,
				       const snd_mixer_selem_id_t *id)
{
	int i;

	for (i = 0; i < mixer->count; i++) {
		snd_mixer_elem_t *e = mixer->pelems[i];
		if (e->sid.index == id->index &&
		    !strcmp(e->sid.name, id->name))
			return e;
	}
	return NULL;
}

const char *_snd_mixer_selem_channels[SND_MIXER_SCHN_LAST + 1] = {
	[SND_MIXER_SCHN_FRONT_LEFT] = "Front Left",
	[SND_MIXER_SCHN_FRONT_RIGHT] = "Front Right",
	[SND_MIXER_SCHN_REAR_LEFT] = "Rear Left",
	[SND_MIXER_SCHN_REAR_RIGHT] = "Rear Right",
	[SND_MIXER_SCHN_FRONT_CENTER] = "Front Center",
	[SND_MIXER_SCHN_WOOFER] = "Woofer",
	[SND_MIXER_SCHN_SIDE_LEFT] = "Side Left",
	[SND_MIXER_SCHN_SIDE_RIGHT] = "Side Right",
	[SND_MIXER_SCHN_REAR_CENTER] = "Rear Center"
};

/*
 */
static int update_volume(snd_mixer_elem_t *elem,
			 int type, int channel, long value)
{
	snd_selem_vol_item_t *str = elem->items[type];
	snd_ctl_elem_value_t *ctl;
	int err;

	if (!str)
		return -EINVAL;
	if (value < str->min || value > str->max)
		return 0;
	if (value == str->vol[USR_IDX(channel)])
		return 0;
	str->vol[USR_IDX(channel)] = value;
	value = convert_from_user(str, value);
	if (value == str->vol[RAW_IDX(channel)])
		return 0;
	str->vol[RAW_IDX(channel)] = value;
	snd_ctl_elem_value_alloca(&ctl);
	snd_ctl_elem_value_set_numid(ctl, str->head.numid);
	err = snd_hctl_elem_read(str->head.helem, ctl);
	if (err < 0)
		return err;
	snd_ctl_elem_value_set_integer(ctl, channel, value);
	return snd_hctl_elem_write(str->head.helem, ctl);
}

int snd_mixer_selem_set_playback_volume(snd_mixer_elem_t *elem,
					snd_mixer_selem_channel_id_t channel,
					long value)
{
	return update_volume(elem, SND_SELEM_ITEM_PVOLUME, channel, value);
}

static int update_volume_all(snd_mixer_elem_t *elem, int type, long value)
{
	snd_selem_vol_item_t *str = elem->items[type];
	int i, err;

	if (!str)
		return -EINVAL;
	for (i = 0; i < str->head.channels; i++) {
		err = update_volume(elem, type, i, value);
		if (err < 0)
			return err;
	}
	return 0;
}

int snd_mixer_selem_set_playback_volume_all(snd_mixer_elem_t *elem, long value)
{
	return update_volume_all(elem, SND_SELEM_ITEM_PVOLUME, value);
}

int snd_mixer_selem_set_capture_volume(snd_mixer_elem_t *elem,
				       snd_mixer_selem_channel_id_t channel,
				       long value)
{
	return update_volume(elem, SND_SELEM_ITEM_CVOLUME, channel, value);
}

int snd_mixer_selem_set_capture_volume_all(snd_mixer_elem_t *elem, long value)
{
	return update_volume_all(elem, SND_SELEM_ITEM_CVOLUME, value);
}

static int set_volume_range(snd_mixer_elem_t *elem, int type,
			    long min, long max)
{
	snd_selem_vol_item_t *str = elem->items[type];
	int i;

	if (!str)
		return -EINVAL;
	str->min = min;
	str->max = max;
	for (i = 0; i < str->head.channels; i++)
		str->vol[USR_IDX(i)] =
			convert_to_user(str, str->vol[RAW_IDX(i)]);
	return 0;
}

int snd_mixer_selem_set_playback_volume_range(snd_mixer_elem_t *elem,
					      long min, long max)
{
	return set_volume_range(elem, SND_SELEM_ITEM_PVOLUME, min, max);
}

int snd_mixer_selem_set_capture_volume_range(snd_mixer_elem_t *elem,
					     long min, long max)
{
	int type;
	if (elem->caps & SND_SM_CAP_GVOLUME)
		type = SND_SELEM_ITEM_PVOLUME;
	else
		type = SND_SELEM_ITEM_CVOLUME;
	return set_volume_range(elem, type, min, max);
}

/*
 */
static int update_switch(snd_mixer_elem_t *elem, int type, int channel,
			 int value)
{
	snd_selem_sw_item_t *str = elem->items[type];
	snd_ctl_elem_value_t *ctl;
	unsigned int sw;
	int err;

	if (!str)
		return -EINVAL;
	sw = str->sw;
	if (value)
		sw |= (1 << channel);
	else
		sw &= ~(1 << channel);
	if (str->sw == sw)
		return 0;
	str->sw = sw;
	snd_ctl_elem_value_alloca(&ctl);
	snd_ctl_elem_value_set_numid(ctl, str->head.numid);
	err = snd_hctl_elem_read(str->head.helem, ctl);
	if (err < 0)
		return err;
	snd_ctl_elem_value_set_integer(ctl, channel, !!value);
	return snd_hctl_elem_write(str->head.helem, ctl);
}

int snd_mixer_selem_set_playback_switch(snd_mixer_elem_t *elem,
					snd_mixer_selem_channel_id_t channel,
					int value)
{
	return update_switch(elem, SND_SELEM_ITEM_PSWITCH, channel, value);
}

static int update_switch_all(snd_mixer_elem_t *elem, int type, int value)
{
	snd_selem_sw_item_t *str = elem->items[type];
	int i, err;

	if (!str)
		return -EINVAL;
	for (i = 0; i < str->head.channels; i++) {
		err = update_switch(elem, type, i, value);
		if (err < 0)
			return err;
	}
	return 0;
}

int snd_mixer_selem_set_playback_switch_all(snd_mixer_elem_t *elem, int value)
{
	return update_switch_all(elem, SND_SELEM_ITEM_PSWITCH, value);
}

int snd_mixer_selem_set_capture_switch(snd_mixer_elem_t *elem,
				       snd_mixer_selem_channel_id_t channel,
				       int value)
{
	int type;
	if (elem->caps & SND_SM_CAP_GSWITCH)
		type = SND_SELEM_ITEM_PSWITCH;
	else
		type = SND_SELEM_ITEM_CSWITCH;
	return update_switch(elem, type, channel, value);
}

int snd_mixer_selem_set_capture_switch_all(snd_mixer_elem_t *elem, int value)
{
	int type;
	if (elem->caps & SND_SM_CAP_GSWITCH)
		type = SND_SELEM_ITEM_PSWITCH;
	else
		type = SND_SELEM_ITEM_CSWITCH;
	return update_switch_all(elem, type, value);
}

/*
 */
int snd_mixer_selem_get_enum_item_name(snd_mixer_elem_t *elem,
				       unsigned int item,
				       size_t maxlen, char *buf)
{
	snd_selem_enum_item_t *eitem;
	snd_ctl_elem_info_t *info;
	int err;

	eitem = elem->items[SND_SELEM_ITEM_ENUM];
	if (!eitem)
		return -EINVAL;
	if (item >= eitem->items)
		return -EINVAL;

	snd_ctl_elem_info_alloca(&info);
	snd_ctl_elem_info_set_numid(info, eitem->head.numid);
	snd_ctl_elem_info_set_item(info, item);
	err = snd_hctl_elem_info(eitem->head.helem, info);
	if (err < 0)
		return err;
	strncpy(buf, snd_ctl_elem_info_get_item_name(info),
		maxlen - 1);
	buf[maxlen - 1] = 0;
	return 0;
}

int snd_mixer_selem_set_enum_item(snd_mixer_elem_t *elem,
				  snd_mixer_selem_channel_id_t channel,
				  unsigned int item)
{
	snd_selem_enum_item_t *eitem;
	snd_ctl_elem_value_t *ctl;
	int err;

	eitem = elem->items[SND_SELEM_ITEM_ENUM];
	if (!eitem)
		return -EINVAL;

	if (eitem->item[channel] == item)
		return 0;
	eitem->item[channel] = item;
	snd_ctl_elem_value_alloca(&ctl);
	snd_ctl_elem_value_set_numid(ctl, eitem->head.numid);
	err = snd_hctl_elem_read(eitem->head.helem, ctl);
	if (err < 0)
		return err;
	snd_ctl_elem_value_set_enumerated(ctl, channel, item);
	return snd_hctl_elem_write(eitem->head.helem, ctl);
}


#if SALSA_HAS_TLV_SUPPORT

/* convert to index of integer array */
#define int_index(size)	(((size) + sizeof(int) - 1) / sizeof(int))

/* max size of a TLV entry for dB information (including compound one) */
#define MAX_TLV_RANGE_SIZE	256

/* parse TLV stream and retrieve dB information
 * return 0 if successly found and stored to rec,
 * return 1 if no information is found,
 * or return a negative error code
 */
static int parse_db_range(snd_selem_vol_item_t *item, unsigned int *tlv,
			  unsigned int tlv_size)
{
	unsigned int type;
	unsigned int size;
	int err;

	type = tlv[0];
	size = tlv[1];
	tlv_size -= 2 * sizeof(int);
	if (size > tlv_size)
		return -EINVAL;
	switch (type) {
	case SND_CTL_TLVT_CONTAINER:
		size = int_index(size) * sizeof(int);
		tlv += 2;
		while (size > 0) {
			unsigned int len;
			err = parse_db_range(item, tlv, size);
			if (err <= 0)
				return err; /* error or found dB */
			len = int_index(tlv[1]) + 2;
			size -= len * sizeof(int);
			tlv += len;
		}
		break;
	case SND_CTL_TLVT_DB_SCALE:
	case SND_CTL_TLVT_DB_RANGE: {
		unsigned int minsize;
		if (type == SND_CTL_TLVT_DB_RANGE)
			minsize = 4 * sizeof(int);
		else
			minsize = 2 * sizeof(int);
		if (size < minsize)
			return -EINVAL;
		if (size > MAX_TLV_RANGE_SIZE)
			return -EINVAL;
		item->db_info = malloc(size + sizeof(int) * 2);
		if (!item->db_info)
			return -ENOMEM;
		memcpy(item->db_info, tlv, size + sizeof(int) * 2);
		return 0;
	}
	default:
		break;
	}
	return -EINVAL; /* not found */
}

/*
 * dB conversion
 *
 * For simplicity, we don't support the linear - log conversion
 */

/* initialize dB range information, reading TLV via hcontrol
 */
static int init_db_info(snd_selem_vol_item_t *item)
{
	snd_ctl_elem_info_t *info;
	unsigned int *tlv = NULL;
	const unsigned int tlv_size = 4096;

	if (!item)
		return -EINVAL;
	if (item->db_info)
		return item->db_info == INVALID_DB_INFO ? -EINVAL : 0;

	snd_ctl_elem_info_alloca(&info);
	if (snd_hctl_elem_info(item->head.helem, info) < 0)
		goto error;
	if (!snd_ctl_elem_info_is_tlv_readable(info))
		goto error;
	tlv = malloc(tlv_size);
	if (!tlv)
		return -ENOMEM;
	if (snd_hctl_elem_tlv_read(item->head.helem, tlv, tlv_size) < 0)
		goto error;
	if (parse_db_range(item, tlv, tlv_size) < 0)
		goto error;
	free(tlv);
	return 0;

 error:
	free(tlv);
	item->db_info = INVALID_DB_INFO;
	return -EINVAL;
}


int _snd_selem_vol_get_dB(snd_selem_vol_item_t *item, int channel,
			  long *value)
{
	if (init_db_info(item) < 0)
		return -EINVAL;
	if (channel >= item->head.channels)
		channel = 0;
	return snd_tlv_convert_to_dB(item->db_info,
				     item->raw_min, item->raw_max,
				     item->vol[RAW_IDX(channel)], value);
}

int _snd_selem_vol_get_dB_range(snd_selem_vol_item_t *item,
				long *min, long *max)
{
	if (init_db_info(item) < 0)
		return -EINVAL;
	return snd_tlv_get_dB_range(item->db_info, item->raw_min, item->raw_max,
				    min, max);
}

int _snd_selem_vol_set_dB(snd_selem_vol_item_t *item,
			  snd_mixer_selem_channel_id_t channel,
			  long db_gain, int xdir)
{
	int err;
	long value;

	if (init_db_info(item) < 0)
		return -EINVAL;
	if (channel >= item->head.channels)
		channel = 0;
	err = snd_tlv_convert_from_dB(item->db_info,
				      item->raw_min, item->raw_max,
				      db_gain, &value, xdir);
	if (err < 0)
		return err;
	item->vol[USR_IDX(channel)] = convert_to_user(item, value);
	return 0;
}

#endif
