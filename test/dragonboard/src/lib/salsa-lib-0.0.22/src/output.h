/*
 *  SALSA-Lib - Output Macros
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

#ifndef __ALSA_OUTPUT_H
#define __ALSA_OUTPUT_H

#include <stdio.h>
#include <errno.h>

typedef FILE snd_output_t;

typedef enum _snd_output_type {
	SND_OUTPUT_STDIO,
	/* SND_OUTPUT_BUFFER */
} snd_output_type_t;

static inline
int snd_output_stdio_open(snd_output_t **outputp, const char *file, const char *mode)
{
	if ((*outputp = fopen(file, mode)) == NULL)
		return -errno;
	return 0;
}

static inline
int snd_output_stdio_attach(snd_output_t **outputp, FILE *fp, int _close)
{
	*outputp = fp;
	return 0;
}

#define snd_output_close(out)			fclose(out)
#define snd_output_printf			fprintf
#define snd_output_puts(out, str)		fputs(str, out)
#define snd_output_putc(out, c)			putc(c, out)
#define snd_output_flush(out)			fflush(out)

static inline __SALSA_NOT_IMPLEMENTED
int snd_output_buffer_open(snd_output_t **outputp)
{
	return -ENXIO;
}

static inline __SALSA_NOT_IMPLEMENTED
size_t snd_output_buffer_string(snd_output_t *output, char **buf)
{
	return 0;
}

#endif /* __ALSA_OUTPUT_H */
