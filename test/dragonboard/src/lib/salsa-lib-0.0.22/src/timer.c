/*
 *  SALSA-Lib - Timer Interface
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
#include "timer.h"
#include "local.h"

int snd_timer_open(snd_timer_t **handle, const char *name, int mode)
{
	int fd, ver, tmode, err;
	snd_timer_t *tmr;
	struct sndrv_timer_select sel;
	int dev_class = SND_TIMER_CLASS_GLOBAL,
		dev_sclass = SND_TIMER_SCLASS_NONE,
		card = 0, device = 0, subdevice = 0;

	*handle = NULL;

	if (strcmp(name, "hw") &&
	    sscanf(name, "hw:CLASS=%i,SCLASS=%i,CARD=%i,DEV=%i,SUBDEV=%i",
		   &dev_class, &dev_sclass, &card, &device, &subdevice) <= 0)
		return -ENODEV;

	tmode = O_RDONLY;
	if (mode & SND_TIMER_OPEN_NONBLOCK)
		tmode |= O_NONBLOCK;
	fd = open(SALSA_DEVPATH "/timer", tmode);
	if (fd < 0)
		return -errno;
	if (ioctl(fd, SNDRV_TIMER_IOCTL_PVERSION, &ver) < 0) {
		err = -errno;
		close(fd);
		return err;
	}
	if (mode & SND_TIMER_OPEN_TREAD) {
		int arg = 1;
		if (ioctl(fd, SNDRV_TIMER_IOCTL_TREAD, &arg) < 0) {
			err = -errno;
			close(fd);
			return err;
		}
	}
	memset(&sel, 0, sizeof(sel));
	sel.id.dev_class = dev_class;
	sel.id.dev_sclass = dev_sclass;
	sel.id.card = card;
	sel.id.device = device;
	sel.id.subdevice = subdevice;
	if (ioctl(fd, SNDRV_TIMER_IOCTL_SELECT, &sel) < 0) {
		err = -errno;
		close(fd);
		return err;
	}
	tmr = calloc(1, sizeof(*tmr));
	if (tmr == NULL) {
		close(fd);
		return -ENOMEM;
	}
	tmr->type = SND_TIMER_TYPE_HW;
	tmr->version = ver;
	tmr->mode = tmode;
	if (name)
		tmr->name = strdup(name);
	tmr->fd = fd;
	tmr->pollfd.fd = fd;
	switch (mode & O_ACCMODE) {
	case O_WRONLY:
		tmr->pollfd.events = POLLOUT|POLLERR|POLLNVAL;
		break;
	case O_RDONLY:
		tmr->pollfd.events = POLLIN|POLLERR|POLLNVAL;
		break;
	case O_RDWR:
		tmr->pollfd.events = POLLOUT|POLLIN|POLLERR|POLLNVAL;
		break;
	}

	*handle = tmr;
	return 0;
}

int snd_timer_close(snd_timer_t *handle)
{
	close(handle->fd);
	free(handle);
	return 0;
}
