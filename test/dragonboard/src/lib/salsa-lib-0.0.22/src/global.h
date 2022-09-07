/*
 *  SALSA-Lib - Global defines
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

#ifndef __ALSA_GLOBAL_H
#define __ALSA_GLOBAL_H

#include <errno.h>

typedef struct _snd_config snd_config_t;

typedef struct timeval snd_timestamp_t;
typedef struct timespec snd_htimestamp_t;

typedef struct sndrv_pcm_info snd_pcm_info_t;
typedef struct sndrv_hwdep_info snd_hwdep_info_t;
typedef struct sndrv_rawmidi_info snd_rawmidi_info_t;

typedef struct _snd_ctl snd_ctl_t;
typedef struct _snd_pcm snd_pcm_t;
typedef struct _snd_rawmidi snd_rawmidi_t;
typedef struct _snd_hwdep snd_hwdep_t;
typedef struct _snd_seq snd_seq_t;

#ifndef ATTRIBUTE_UNUSED
/** do not print warning (gcc) when function parameter is not used */
#define ATTRIBUTE_UNUSED __attribute__ ((__unused__))
#endif

#if SALSA_MARK_DEPRECATED
#define __SALSA_NOT_IMPLEMENTED	__attribute__ ((deprecated))
#else
#define __SALSA_NOT_IMPLEMENTED
#endif

/* async helpers */
typedef struct _snd_async_handler snd_async_handler_t;
typedef void (*snd_async_callback_t)(snd_async_handler_t *handler);

#if SALSA_HAS_ASYNC_SUPPORT

struct _snd_async_handler {
	int fd;
	snd_async_callback_t callback;
	void *private_data;
	void *rec;
	struct _snd_async_handler **pointer;
	struct _snd_async_handler *next;
};

#define snd_async_handler_get_signo(h)	SIGIO
#define snd_async_handler_get_fd(h)	(h)->fd
#define snd_async_handler_get_callback_private(h)	(h)->private_data
int snd_async_add_handler(snd_async_handler_t **handler, int fd,
			  snd_async_callback_t callback, void *private_data);
int snd_async_del_handler(snd_async_handler_t *handler);

#else

#define snd_async_handler_get_signo(h)	-1
#define snd_async_handler_get_fd(h)	-1
#define snd_async_handler_get_callback_private(h)	NULL
static inline __SALSA_NOT_IMPLEMENTED
int snd_async_add_handler(snd_async_handler_t **handler, int fd,
			  snd_async_callback_t callback, void *private_data)
{
	return -ENXIO;
}
static inline __SALSA_NOT_IMPLEMENTED
int snd_async_del_handler(snd_async_handler_t *handler)
{
	return -ENXIO;
}

#endif /* SALSA_HAS_ASYNC_SUPPORT */

/* only for internal use */
int _snd_set_nonblock(int fd, int nonblock);

#if !SALSA_HAS_DUMMY_CONF
/* the global function defined here */
static inline
int snd_config_update_free_global(void)
{
	return 0;
}
#endif /* !SALSA_HAS_DUMMY_CONF */

/*
 * helper macros
 */
#define __snd_sizeof(type)		 \
static inline size_t type##_sizeof(void) \
{					 \
	return sizeof(type##_t);	 \
}

#define __snd_malloc(type)			\
static inline int type##_malloc(type##_t **ptr) \
{						 \
	*ptr = (type##_t *)calloc(1, sizeof(**ptr)); \
	if (!*ptr)				 \
		return -ENOMEM;			 \
	return 0;				 \
}

#define __snd_free(type)			\
static inline void type##_free(type##_t *obj)	\
{						\
	free(obj);				\
}

#define __snd_clear(type)				\
static inline void type##_clear(type##_t *obj)		\
{							\
	memset(obj, 0, sizeof(type##_t));		\
}

#define __snd_copy(type)					   \
static inline void type##_copy(type##_t *dst, const type##_t *src) \
{								   \
	*dst = *src;						   \
}

#define __snd_define_type(type)		\
__snd_sizeof(type)			\
__snd_malloc(type)			\
__snd_free(type)			\
__snd_clear(type)			\
__snd_copy(type)

#define __snd_alloca(ptr, type)					\
	do {							\
		*ptr = (type##_t*)alloca(type##_sizeof());	\
		memset(*ptr, 0, type##_sizeof());		\
	} while (0)

#endif /* __ALSA_GLOBAL_H */
