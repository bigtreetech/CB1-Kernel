/*
 *  SALSA-Lib - Raw MIDI Interface
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
#include "rawmidi.h"
#include "control.h"
#include "local.h"

static int open_with_subdev(const char *filename, int fmode,
			    int card, int subdev)
{
	snd_rawmidi_info_t info;
	snd_ctl_t *ctl;
	int err, counts, fd;

	err = _snd_ctl_hw_open(&ctl, card);
	if (err < 0)
		return err;

	err = snd_ctl_rawmidi_prefer_subdevice(ctl, subdev);
	if (err < 0)
		return err;

	for (counts = 0; counts < 3; counts++) {
		fd = open(filename, fmode);
		if (fd < 0)
		        return -errno;
		memset(&info, 0, sizeof(info));
		info.stream = ((fmode & O_ACCMODE) != O_RDONLY) ?
			SNDRV_RAWMIDI_STREAM_OUTPUT : SNDRV_RAWMIDI_STREAM_INPUT;
		if (ioctl(fd, SNDRV_RAWMIDI_IOCTL_INFO, &info) >= 0 &&
		    info.subdevice == subdev) {
			snd_ctl_close(ctl);
			return fd;
		}
		close(fd);
	}
	snd_ctl_close(ctl);
	return -EBUSY;
}

static snd_rawmidi_t *new_rmidi(snd_rawmidi_hw_t *hw, int stream, int mode)
{
	snd_rawmidi_t *rmidi;

	rmidi = calloc(1, sizeof(*rmidi));
	if (!rmidi)
		return NULL;
	rmidi->stream = stream;
	rmidi->mode = mode;
	rmidi->fd = hw->fd;
	rmidi->pollfd.fd = hw->fd;
	rmidi->pollfd.events = (stream == SND_RAWMIDI_STREAM_OUTPUT) ?
		(POLLOUT|POLLERR|POLLNVAL) : (POLLIN|POLLERR|POLLNVAL);
	rmidi->hw = hw;
	hw->opened++;

	rmidi->params.buffer_size = 4096;
	rmidi->params.avail_min = 1;
	snd_rawmidi_params(rmidi, &rmidi->params);

	return rmidi;
}

int snd_rawmidi_open(snd_rawmidi_t **in_rmidi, snd_rawmidi_t **out_rmidi,
		     const char *name, int mode)
{
	int fd, err, fmode, ver;
	int card, dev, subdev;
	char filename[32];
	snd_rawmidi_hw_t *hw;

	if (in_rmidi)
		*in_rmidi = NULL;
	if (out_rmidi)
		*out_rmidi = NULL;
	err = _snd_dev_get_device(name, &card, &dev, &subdev);
	if (err < 0)
		return err;

	snprintf(filename, sizeof(filename), "%s/midiC%dD%d",
		 SALSA_DEVPATH, card, dev);

	if (!in_rmidi)
		fmode = O_WRONLY;
	else if (!out_rmidi)
		fmode = O_RDONLY;
	else
		fmode = O_RDWR;
	if (mode & SND_RAWMIDI_APPEND)
		fmode |= O_APPEND;
	if (mode & SND_RAWMIDI_NONBLOCK)
		fmode |= O_NONBLOCK;
	if (mode & SND_RAWMIDI_SYNC)
		fmode |= O_SYNC;

	if (subdev >= 0) {
		fd = open_with_subdev(filename, fmode, card, subdev);
		if (fd < 0)
			return fd;
	} else {
		fd = open(filename, fmode);
		if (fd < 0)
			return -errno;
	}
	if (ioctl(fd, SNDRV_RAWMIDI_IOCTL_PVERSION, &ver) < 0) {
		err = -errno;
		goto error;
	}

	hw = calloc(1, sizeof(*hw));
	if (!hw) {
		err = -ENOMEM;
		goto error;
	}
	if (name)
		hw->name = strdup(name);
	hw->type = SND_RAWMIDI_TYPE_HW;
	hw->fd = fd;
	if (in_rmidi) {
		*in_rmidi = new_rmidi(hw, SND_RAWMIDI_STREAM_INPUT, mode);
		if (!*in_rmidi) {
			err = -ENOMEM;
			goto error;
		}
	}
	if (out_rmidi) {
		*out_rmidi = new_rmidi(hw, SND_RAWMIDI_STREAM_OUTPUT, mode);
		if (!*out_rmidi) {
			err = -ENOMEM;
			goto error;
		}
	}
	return 0;

 error:
	if (in_rmidi && *in_rmidi) {
		snd_rawmidi_close(*in_rmidi);
		*in_rmidi = NULL;
	}
	close(fd);
	return err;
}

int snd_rawmidi_close(snd_rawmidi_t *rmidi)
{
	snd_rawmidi_hw_t *hw = rmidi->hw;

	if (hw) {
		hw->opened--;
		if (!hw->opened) {
			close(hw->fd);
			free(hw->name);
			free(hw);
		}
	}
	free(rmidi);
	return 0;
}
