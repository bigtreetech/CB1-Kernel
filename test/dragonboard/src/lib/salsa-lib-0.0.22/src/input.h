/*
 *  SALSA-Lib - Input Macros
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

#ifndef __ALSA_INPUT_H
#define __ALSA_INPUT_H

#include <stdio.h>
#include <errno.h>

typedef FILE snd_input_t;

/** Input type. */
typedef enum _snd_input_type {
	/** Input from a stdio stream. */
	SND_INPUT_STDIO,
} snd_input_type_t;

static inline
int snd_input_stdio_open(snd_input_t **inputp, const char *file, const char *mode)
{
	if ((*inputp = fopen(file, mode)) == NULL)
		return -errno;
	return 0;
}

static inline
int snd_input_stdio_attach(snd_input_t **inputp, FILE *fp, int _close)
{
	*inputp = fp;
	return 0;
}

#define snd_input_close(input)		fclose(input)
#define snd_input_scanf			fscanf
#define snd_input_gets(input,str,size)	fgets(str, size, input)
#define snd_input_getc(input)		getc(input)
#define snd_input_ungetc(input,c)	ungetc(c, input)

static inline __SALSA_NOT_IMPLEMENTED
int snd_input_buffer_open(snd_input_t **inputp, const char *buffer,
			  ssize_t size)
{
	return -ENXIO;
}

#endif /* __ALSA_INPUT_H */
