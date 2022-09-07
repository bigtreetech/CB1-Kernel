/*
 *  SALSA-Lib - Async handler helpers
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
#define __USE_GNU
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "recipe.h"
#include "global.h"


static snd_async_handler_t *async_list;

static int snd_set_async(snd_async_handler_t *h, int sig)
{
	long flags;
	int err;

	flags = fcntl(h->fd, F_GETFL);
	if (flags < 0)
		return -errno;
	if (sig >= 0)
		flags |= O_ASYNC;
	else
		flags &= ~O_ASYNC;
	if (fcntl(h->fd, F_SETFL, flags) < 0)
		return -errno;
	if (sig >= 0) {
		if (fcntl(h->fd, F_SETSIG, (long)sig) < 0 ||
		    fcntl(h->fd, F_SETOWN, (long)getpid()) < 0) {
			err = -errno;
			fcntl(h->fd, F_SETFL, flags & ~O_ASYNC);
			return err;
		}
	}
	return 0;
}

static void snd_async_handler(int signo, siginfo_t *siginfo, void *context)
{
	snd_async_handler_t *h;
	int fd = siginfo->si_fd;

	for (h = async_list; h; h = h->next) {
		if (h->fd == fd && h->callback)
			h->callback(h);
	}
}

int snd_async_add_handler(snd_async_handler_t **handler, int fd,
			  snd_async_callback_t callback, void *private_data)
{
	snd_async_handler_t *h;
	int err;

	h = calloc(1, sizeof(*h));
	if (!h)
		return -ENOMEM;
	h->fd = fd;
	h->callback = callback;
	h->private_data = private_data;
	h->next = async_list;
	err = snd_set_async(h, SIGIO);
	if (err < 0) {
		free(h);
		return err;
	}
	if (!async_list) {
		struct sigaction act;
		memset(&act, 0, sizeof(act));
		act.sa_flags = SA_RESTART | SA_SIGINFO;
		act.sa_sigaction = snd_async_handler;
		sigemptyset(&act.sa_mask);
		if (sigaction(SIGIO, &act, NULL) < 0) {
			int err = -errno;
			snd_set_async(h, -1);
			free(h);
			return err;
		}
	}
	async_list = h;
	*handler = h;
	return 0;
}

int snd_async_del_handler(snd_async_handler_t *handler)
{
	snd_async_handler_t *h, *prev;

	for (h = async_list, prev = NULL; h; prev = h, h = h->next) {
		if (h == handler) {
			if (prev)
				prev->next = h->next;
			else
				async_list = h->next;
			snd_set_async(h, -1);
			if (h->pointer)
				*h->pointer = NULL;
			free(h);
			break;
		}
	}

	if (!async_list) {
		struct sigaction act;
		memset(&act, 0, sizeof(act));
		act.sa_flags = 0;
		act.sa_handler = SIG_DFL;
		sigaction(SIGIO, &act, NULL);
	}
	return 0;
}
