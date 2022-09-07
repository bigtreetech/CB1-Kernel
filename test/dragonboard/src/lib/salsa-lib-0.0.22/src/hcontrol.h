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

#ifndef __ALSA_HCONTROL_H
#define __ALSA_HCONTROL_H

#include "control.h"
#include "hctl_types.h"

int snd_hctl_open(snd_hctl_t **hctl, const char *name, int mode);
int snd_hctl_open_ctl(snd_hctl_t **hctlp, snd_ctl_t *ctl);
int snd_hctl_close(snd_hctl_t *hctl);
snd_hctl_elem_t *snd_hctl_find_elem(snd_hctl_t *hctl, const snd_ctl_elem_id_t *id);
int snd_hctl_load(snd_hctl_t *hctl);
int snd_hctl_free(snd_hctl_t *hctl);
int snd_hctl_handle_events(snd_hctl_t *hctl);

#include "hctl_macros.h"

#endif /* __ALSA_HCONTROL_H */
