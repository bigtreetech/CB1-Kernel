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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/poll.h>
#include "hwdep.h"
#include "local.h"

int snd_hwdep_open(snd_hwdep_t **handlep, const char *name, int mode)
{
	int card, device, fd, ver, err;
	char filename[32];
	snd_hwdep_t *hwdep;

	*handlep = NULL;

	err = _snd_dev_get_device(name, &card, &device, NULL);
	if (err < 0)
		return err;
	if (card < 0 || card >= 32)
		return -EINVAL;
	if (device < 0 || device >= 32)
		return -EINVAL;
	snprintf(filename, sizeof(filename), "%s/hwC%dD%d",
		 SALSA_DEVPATH, card, device);
	fd = open(filename, mode);
	if (fd < 0)
		return -errno;
	if (ioctl(fd, SNDRV_HWDEP_IOCTL_PVERSION, &ver) < 0) {
		err = -errno;
		close(fd);
		return err;
	}
	hwdep = calloc(1, sizeof(*hwdep));
	if (!hwdep) {
		close(fd);
		return -ENOMEM;
	}
	if (name)
		hwdep->name = strdup(name);
	hwdep->fd = fd;
	hwdep->mode = mode;
	hwdep->type = SND_HWDEP_TYPE_HW;

	hwdep->pollfd.fd = fd;
	switch (mode & O_ACCMODE) {
	case O_WRONLY:
		hwdep->pollfd.events = POLLOUT|POLLERR|POLLNVAL;
		break;
	case O_RDONLY:
		hwdep->pollfd.events = POLLIN|POLLERR|POLLNVAL;
		break;
	case O_RDWR:
		hwdep->pollfd.events = POLLOUT|POLLIN|POLLERR|POLLNVAL;
		break;
	}

	*handlep = hwdep;
	return 0;
}

int snd_hwdep_close(snd_hwdep_t *hwdep)
{
	close(hwdep->fd);
	free(hwdep);
	return 0;
}
