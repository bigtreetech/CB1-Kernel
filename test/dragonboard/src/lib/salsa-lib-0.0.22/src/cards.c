/*
 *  SALSA-Lib - Global card functions
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
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "control.h"
#include "local.h"

int _snd_set_nonblock(int fd, int nonblock)
{
	long flags;
	flags = fcntl(fd, F_GETFL);
	if (flags < 0)
		return -errno;
	if (nonblock)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0)
		return -errno;
	return 0;
}

#define SND_FILE_CONTROL	SALSA_DEVPATH "/controlC%i"

int snd_card_load(int card)
{
	char control[sizeof(SND_FILE_CONTROL) + 10];
	sprintf(control, SND_FILE_CONTROL, card);
	return !access(control, R_OK);
}

int snd_card_next(int *rcard)
{
	int card;

	if (rcard == NULL)
		return -EINVAL;
	card = *rcard;
	card = card < 0 ? 0 : card + 1;
	for (; card < 32; card++) {
		if (snd_card_load(card)) {
			*rcard = card;
			return 0;
		}
	}
	*rcard = -1;
	return 0;
}

int _snd_ctl_hw_open(snd_ctl_t **ctlp, int card)
{
	char name[16];
	sprintf(name, "hw:%d", card);
	return snd_ctl_open(ctlp, name, 0);
}

static int get_card_info(int card, snd_ctl_card_info_t *info)
{
	int err;
	snd_ctl_t *handle;

	memset(info, 0, sizeof(*info));
	err = _snd_ctl_hw_open(&handle, card);
	if (err < 0)
		return err;
	err = snd_ctl_card_info(handle, info);
	snd_ctl_close(handle);
	return err;
}

int snd_card_get_index(const char *string)
{
	int card;
	snd_ctl_card_info_t info;

	if (!string || *string == '\0')
		return -EINVAL;
	if (sscanf(string, "%i", &card) == 1) {
		if (card < 0 || card > 31)
			return -EINVAL;
		if (snd_card_load(card))
			return card;
		return -ENODEV;
	}
	for (card = 0; card < 32; card++) {
		if (!snd_card_load(card))
			continue;
		if (get_card_info(card, &info) < 0)
			continue;
		if (!strcmp((const char *)info.id, string))
			return card;
	}
	return -ENODEV;
}

int snd_card_get_name(int card, char **name)
{
	snd_ctl_card_info_t info;
	int err;

	err = get_card_info(card, &info);
	if (err < 0)
		return err;
	*name = strdup((const char *)info.name);
	if (*name == NULL)
		return -ENOMEM;
	return 0;
}

int snd_card_get_longname(int card, char **name)
{
	snd_ctl_card_info_t info;
	int err;

	err = get_card_info(card, &info);
	if (err < 0)
		return err;
	*name = strdup((const char *)info.longname);
	if (*name == NULL)
		return -ENOMEM;
	return 0;
}

int _snd_dev_get_device(const char *name, int *cardp, int *devp, int *subdevp)
{
	int card;
	*cardp = 0;
	if (devp)
		*devp = 0;
	if (subdevp)
		*subdevp = -1;
	if (!strcmp(name, "hw") || !strcmp(name, "default"))
		return 0;
	if (!strncmp(name, "hw:", 3))
		name += 3;
	else if (!strncmp(name, "default:", 8))
		name += 8;
	else
		return -EINVAL;
	card = snd_card_get_index(name);
	if (card < 0)
		return card;
	*cardp = card;
	if (devp && subdevp) {
		/* parse the secondary and third arguments (if any) */
		name = strchr(name, ',');
		if (name)
			sscanf(name, ",%d,%d",  devp, subdevp);
	}
	return 0;
}

snd_config_t *snd_config; /* placeholder */
