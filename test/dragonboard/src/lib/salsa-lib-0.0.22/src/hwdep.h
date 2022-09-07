/*
 *  SALSA-Lib - Hardware Dependent Interface
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

#ifndef __ALSA_HWDEP_H
#define __ALSA_HWDEP_H

#include "recipe.h"
#include "global.h"
#include "hwdep_types.h"

int snd_hwdep_open(snd_hwdep_t **hwdep, const char *name, int mode);
int snd_hwdep_close(snd_hwdep_t *hwdep);

#include "hwdep_macros.h"

#define snd_hwdep_info_alloca(ptr)	__snd_alloca(ptr, snd_hwdep_info)
#define snd_hwdep_dsp_status_alloca(ptr) __snd_alloca(ptr, snd_hwdep_dsp_status)
#define snd_hwdep_dsp_image_alloca(ptr)	__snd_alloca(ptr, snd_hwdep_dsp_image)

#endif /* __ALSA_HWDEP_H */

