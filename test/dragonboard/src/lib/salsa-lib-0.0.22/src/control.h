/*
 *  SALSA-Lib - Control Interface
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

#ifndef __ALSA_CONTROL_H
#define __ALSA_CONTROL_H

#include "recipe.h"
#include "global.h"
#include "ctl_types.h"

int snd_card_load(int card);
int snd_card_next(int *card);
int snd_card_get_index(const char *name);
int snd_card_get_name(int card, char **name);
int snd_card_get_longname(int card, char **name);

int snd_ctl_open(snd_ctl_t **ctl, const char *name, int mode);
int snd_ctl_close(snd_ctl_t *ctl);
int snd_ctl_wait(snd_ctl_t *ctl, int timeout);

#if SALSA_HAS_TLV_SUPPORT
int snd_ctl_elem_tlv_read(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			  unsigned int *tlv, unsigned int tlv_size);
int snd_ctl_elem_tlv_write(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			   const unsigned int *tlv);
int snd_ctl_elem_tlv_command(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			     const unsigned int *tlv);
#endif

#if SALSA_HAS_ASYNC_SUPPORT
int snd_async_add_ctl_handler(snd_async_handler_t **handler, snd_ctl_t *ctl,
			      snd_async_callback_t callback,
			      void *private_data);
#endif

#include "ctl_macros.h"

#define snd_ctl_elem_id_alloca(ptr)	__snd_alloca(ptr, snd_ctl_elem_id)
#define snd_ctl_card_info_alloca(ptr)	__snd_alloca(ptr, snd_ctl_card_info)
#define snd_ctl_event_alloca(ptr)	__snd_alloca(ptr, snd_ctl_event)
#define snd_ctl_elem_list_alloca(ptr)	__snd_alloca(ptr, snd_ctl_elem_list)
#define snd_ctl_elem_info_alloca(ptr)	__snd_alloca(ptr, snd_ctl_elem_info)
#define snd_ctl_elem_value_alloca(ptr)	__snd_alloca(ptr, snd_ctl_elem_value)

int snd_ctl_elem_add_integer(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			     unsigned int count, long min, long max, long step);
int snd_ctl_elem_add_integer64(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			       unsigned int count, long long min, long long max,
			       long long step);
int snd_ctl_elem_add_boolean(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			     unsigned int count);
int snd_ctl_elem_add_iec958(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id);

#if SALSA_HAS_TLV_SUPPORT
static inline
int snd_tlv_parse_dB_info(unsigned int *tlv, unsigned int tlv_size,
			  unsigned int **db_tlvp)
{
	/* just for simplicity */
	*db_tlvp = tlv;
	return 0;
}

int snd_tlv_get_dB_range(unsigned int *tlv, long rangemin, long rangemax,
			 long *min, long *max);
int snd_tlv_convert_to_dB(unsigned int *tlv, long rangemin, long rangemax,
			  long volume, long *db_gain);
int snd_tlv_convert_from_dB(unsigned int *tlv, long rangemin, long rangemax,
			    long db_gain, long *value, int xdir);
int snd_ctl_get_dB_range(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			 long *min, long *max);
int snd_ctl_convert_to_dB(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			  long volume, long *db_gain);
int snd_ctl_convert_from_dB(snd_ctl_t *ctl, const snd_ctl_elem_id_t *id,
			    long db_gain, long *value, int xdir);
#endif

#endif /* __ALSA_CONTROL_H */
