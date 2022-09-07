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

#ifndef __ALSA_MIXER_H
#define __ALSA_MIXER_H

#include "hcontrol.h"
#include "mixer_types.h"

int snd_mixer_open(snd_mixer_t **mixer, int mode);
int snd_mixer_close(snd_mixer_t *mixer);
int snd_mixer_handle_events(snd_mixer_t *mixer);
int snd_mixer_attach(snd_mixer_t *mixer, const char *name);
int snd_mixer_attach_hctl(snd_mixer_t *mixer, snd_hctl_t *hctl);
int snd_mixer_detach(snd_mixer_t *mixer, const char *name);
int snd_mixer_detach_hctl(snd_mixer_t *mixer, snd_hctl_t *hctl);
int snd_mixer_load(snd_mixer_t *mixer);
void snd_mixer_free(snd_mixer_t *mixer);

int snd_mixer_selem_register(snd_mixer_t *mixer,
			     struct snd_mixer_selem_regopt *options,
			     snd_mixer_class_t **classp);
snd_mixer_elem_t *snd_mixer_find_selem(snd_mixer_t *mixer,
				       const snd_mixer_selem_id_t *id);

int snd_mixer_selem_set_playback_volume(snd_mixer_elem_t *elem,
					snd_mixer_selem_channel_id_t channel,
					long value);
int snd_mixer_selem_set_capture_volume(snd_mixer_elem_t *elem,
				       snd_mixer_selem_channel_id_t channel,
				       long value);
int snd_mixer_selem_set_playback_volume_all(snd_mixer_elem_t *elem, long value);
int snd_mixer_selem_set_capture_volume_all(snd_mixer_elem_t *elem, long value);
int snd_mixer_selem_set_playback_switch(snd_mixer_elem_t *elem,
					snd_mixer_selem_channel_id_t channel,
					int value);
int snd_mixer_selem_set_capture_switch(snd_mixer_elem_t *elem,
				       snd_mixer_selem_channel_id_t channel,
				       int value);
int snd_mixer_selem_set_playback_switch_all(snd_mixer_elem_t *elem, int value);
int snd_mixer_selem_set_capture_switch_all(snd_mixer_elem_t *elem, int value);
int snd_mixer_selem_set_playback_volume_range(snd_mixer_elem_t *elem,
					      long min, long max);
int snd_mixer_selem_set_capture_volume_range(snd_mixer_elem_t *elem,
					     long min, long max);

int snd_mixer_selem_set_enum_item(snd_mixer_elem_t *elem,
				  snd_mixer_selem_channel_id_t channel,
				  unsigned int idx);
int snd_mixer_selem_get_enum_item_name(snd_mixer_elem_t *elem,
				       unsigned int idx,
				       size_t maxlen, char *str);

#include "mixer_macros.h"

#define snd_mixer_selem_id_alloca(ptr)	__snd_alloca(ptr, snd_mixer_selem_id)

#endif /* __ALSA_MIXER_H */
