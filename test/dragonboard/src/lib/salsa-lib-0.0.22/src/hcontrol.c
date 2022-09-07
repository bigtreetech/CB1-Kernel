/*
 *  SALSA-Lib - High-level Control Interface
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
#include "hcontrol.h"
#include "local.h"


/*
 * open/close
 */
int snd_hctl_open(snd_hctl_t **hctlp, const char *name, int mode)
{
	snd_ctl_t *ctl;
	int err;

	err = snd_ctl_open(&ctl, name, mode);
	if (err < 0)
		return err;
	err = snd_hctl_open_ctl(hctlp, ctl);
	if (err < 0)
		snd_ctl_close(ctl);
	return err;
}

int snd_hctl_open_ctl(snd_hctl_t **hctlp, snd_ctl_t *ctl)
{
	snd_hctl_t *hctl;

	*hctlp = hctl = calloc(1, sizeof(*hctl));
	if (!hctl)
		return -ENOMEM;
	hctl->ctl = ctl;
	return 0;
}

int snd_hctl_close(snd_hctl_t *hctl)
{
	snd_hctl_free(hctl);
	snd_ctl_close(hctl->ctl);
	free(hctl);
	return 0;
}


/*
 * event handlers
 */
static inline
int snd_hctl_throw_event(snd_hctl_t *hctl, unsigned int mask,
			 snd_hctl_elem_t *elem)
{
	if (hctl->callback)
		return hctl->callback(hctl, mask, elem);
	return 0;
}

static inline
int snd_hctl_elem_throw_event(snd_hctl_elem_t *elem,
			      unsigned int mask)
{
	if (elem->callback)
		return elem->callback(elem, mask);
	return 0;
}


/*
 * add/remove a hcontrol element
 */
static void add_elem_list(snd_hctl_t *hctl, snd_hctl_elem_t *elem)
{
	elem->next = NULL;
	elem->prev = hctl->last_elem;
	if (hctl->last_elem)
		hctl->last_elem->next = elem;
	else
		hctl->first_elem = elem;
	hctl->last_elem = elem;
	hctl->count++;
}

static void del_elem_list(snd_hctl_t *hctl, snd_hctl_elem_t *elem)
{
	hctl->count--;
	if (elem->prev)
		elem->prev->next = elem->next;
	else
		hctl->first_elem = elem->next;
	if (elem->next)
		elem->next->prev = elem->prev;
	else
		hctl->last_elem = elem->prev;
}

static int snd_hctl_elem_add(snd_hctl_t *hctl, snd_hctl_elem_t *elem)
{
	if (snd_hctl_find_elem(hctl, &elem->id))
		return -EBUSY;
	add_elem_list(hctl, elem);
	return snd_hctl_throw_event(hctl, SNDRV_CTL_EVENT_MASK_ADD, elem);
}

static void snd_hctl_elem_remove(snd_hctl_t *hctl,
				 snd_hctl_elem_t *elem)
{
	del_elem_list(hctl, elem);
	snd_hctl_elem_throw_event(elem, SNDRV_CTL_EVENT_MASK_REMOVE);
	free(elem);
}

/*
 * release all elements
 */
int snd_hctl_free(snd_hctl_t *hctl)
{
	while (hctl->last_elem)
		snd_hctl_elem_remove(hctl, hctl->last_elem);
	return 0;
}

/* placeholder */
int snd_hctl_compare_fast(const snd_hctl_elem_t *c1,
			  const snd_hctl_elem_t *c2)
{
	return 0;
}

snd_hctl_elem_t *snd_hctl_find_elem(snd_hctl_t *hctl,
				    const snd_ctl_elem_id_t *id)
{
	snd_hctl_elem_t *elem;
	for (elem = hctl->first_elem; elem; elem = elem->next) {
		if (elem->id.iface == id->iface &&
		    elem->id.index == id->index &&
		    !strcmp((char *)elem->id.name, (char *)id->name))
			return elem;
	}
	return NULL;
}

int snd_hctl_load(snd_hctl_t *hctl)
{
	snd_ctl_elem_list_t list;
	int err = 0;
	unsigned int idx;
	snd_hctl_elem_t *elem;

	memset(&list, 0, sizeof(list));
	if ((err = snd_ctl_elem_list(hctl->ctl, &list)) < 0)
		goto _end;
	while (list.count != list.used) {
		err = snd_ctl_elem_list_alloc_space(&list, list.count);
		if (err < 0)
			goto _end;
		if ((err = snd_ctl_elem_list(hctl->ctl, &list)) < 0)
			goto _end;
	}
	for (idx = 0; idx < list.count; idx++) {
		elem = calloc(1, sizeof(*elem));
		if (!elem) {
			snd_hctl_free(hctl);
			err = -ENOMEM;
			goto _end;
		}
		elem->id = list.pids[idx];
		elem->hctl = hctl;
		add_elem_list(hctl, elem);
	}
	for (elem = hctl->first_elem; elem; elem = elem->next) {
		int res = snd_hctl_throw_event(hctl, SNDRV_CTL_EVENT_MASK_ADD,
					       elem);
		if (res < 0)
			return res;
	}
	err = snd_ctl_subscribe_events(hctl->ctl, 1);
 _end:
	free(list.pids);
	return err;
}

static int snd_hctl_handle_event(snd_hctl_t *hctl, snd_ctl_event_t *event)
{
	snd_hctl_elem_t *elem;
	int res;

	switch (event->type) {
	case SND_CTL_EVENT_ELEM:
		break;
	default:
		return 0;
	}
	if (event->data.elem.mask == SNDRV_CTL_EVENT_MASK_REMOVE) {
		snd_hctl_elem_t *res;
		res = snd_hctl_find_elem(hctl, &event->data.elem.id);
		if (!res)
			return -ENOENT;
		snd_hctl_elem_remove(hctl, res);
		return 0;
	}
	if (event->data.elem.mask & SNDRV_CTL_EVENT_MASK_ADD) {
		elem = calloc(1, sizeof(snd_hctl_elem_t));
		if (elem == NULL)
			return -ENOMEM;
		elem->id = event->data.elem.id;
		elem->hctl = hctl;
		res = snd_hctl_elem_add(hctl, elem);
		if (res < 0)
			return res;
	}
	if (event->data.elem.mask & (SNDRV_CTL_EVENT_MASK_VALUE |
				     SNDRV_CTL_EVENT_MASK_INFO)) {
		elem = snd_hctl_find_elem(hctl, &event->data.elem.id);
		if (!elem)
			return -ENOENT;
		res = snd_hctl_elem_throw_event(elem, event->data.elem.mask &
						(SNDRV_CTL_EVENT_MASK_VALUE |
						 SNDRV_CTL_EVENT_MASK_INFO));
		if (res < 0)
			return res;
	}
	return 0;
}

int snd_hctl_handle_events(snd_hctl_t *hctl)
{
	snd_ctl_event_t event;
	int res;
	unsigned int count = 0;

	while ((res = snd_ctl_read(hctl->ctl, &event)) != 0 &&
	       res != -EAGAIN) {
		if (res < 0)
			return res;
		res = snd_hctl_handle_event(hctl, &event);
		if (res < 0)
			return res;
		count++;
	}
	return count;
}

