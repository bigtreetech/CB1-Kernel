#ifndef __ALSA_MIXER_TYPES_H
#define __ALSA_MIXER_TYPES_H

#include "pcm.h"

typedef struct _snd_mixer snd_mixer_t;
typedef struct _snd_mixer_elem snd_mixer_elem_t;
#define snd_mixer_class_t	snd_mixer_t

/**
 * \brief Mixer callback function
 * \param mixer Mixer handle
 * \param mask event mask
 * \param elem related mixer element (if any)
 * \return 0 on success otherwise a negative error code
 */
typedef int (*snd_mixer_callback_t)(snd_mixer_t *ctl,
				    unsigned int mask,
				    snd_mixer_elem_t *elem);

/**
 * \brief Mixer element callback function
 * \param elem Mixer element
 * \param mask event mask
 * \return 0 on success otherwise a negative error code
 */
typedef int (*snd_mixer_elem_callback_t)(snd_mixer_elem_t *elem,
					 unsigned int mask);

/**
 * \brief Compare function for sorting mixer elements
 * \param e1 First element
 * \param e2 Second element
 * \return -1 if e1 < e2, 0 if e1 == e2, 1 if e1 > e2
 */
typedef int (*snd_mixer_compare_t)(const snd_mixer_elem_t *e1,
				   const snd_mixer_elem_t *e2);

/** Mixer element type */
typedef enum _snd_mixer_elem_type {
	/* Simple (legacy) mixer elements */
	SND_MIXER_ELEM_SIMPLE,
	SND_MIXER_ELEM_LAST = SND_MIXER_ELEM_SIMPLE
} snd_mixer_elem_type_t;

/**
 *  \defgroup SimpleMixer Simple Mixer Interface
 *  \ingroup Mixer
 *  The simple mixer interface.
 *  \{
 */

/* Simple (legacy) mixer elements API */

/** Mixer simple element channel identifier */
typedef enum _snd_mixer_selem_channel_id {
	/** Unknown */
	SND_MIXER_SCHN_UNKNOWN = -1,
	/** Front left */
	SND_MIXER_SCHN_FRONT_LEFT = 0,
	/** Front right */
	SND_MIXER_SCHN_FRONT_RIGHT,
	/** Rear left */
	SND_MIXER_SCHN_REAR_LEFT,
	/** Rear right */
	SND_MIXER_SCHN_REAR_RIGHT,
	/** Front center */
	SND_MIXER_SCHN_FRONT_CENTER,
	/** Woofer */
	SND_MIXER_SCHN_WOOFER,
	/** Side Left */
	SND_MIXER_SCHN_SIDE_LEFT,
	/** Side Right */
	SND_MIXER_SCHN_SIDE_RIGHT,
	/** Rear Center */
	SND_MIXER_SCHN_REAR_CENTER,
	SND_MIXER_SCHN_LAST = 31,
	/** Mono (Front left alias) */
	SND_MIXER_SCHN_MONO = SND_MIXER_SCHN_FRONT_LEFT
} snd_mixer_selem_channel_id_t;

/** Mixer simple element - register options - abstraction level */
enum snd_mixer_selem_regopt_abstract {
	/** no abstraction - try use all universal controls from driver */
	SND_MIXER_SABSTRACT_NONE = 0,
	/** basic abstraction - Master,PCM,CD,Aux,Record-Gain etc. */
	SND_MIXER_SABSTRACT_BASIC,
};

/** Mixer simple element - register options */
struct snd_mixer_selem_regopt {
	/** structure version */
	int ver;
	/** v1: abstract layer selection */
	enum snd_mixer_selem_regopt_abstract abstract;
	/** v1: device name (must be NULL when playback_pcm or capture_pcm != NULL) */
	const char *device;
	/** v1: playback PCM connected to mixer device (NULL == none) */
	snd_pcm_t *playback_pcm;
	/** v1: capture PCM connected to mixer device (NULL == none) */
	snd_pcm_t *capture_pcm;
};

/** Mixer simple element identifier */
typedef struct _snd_mixer_selem_id snd_mixer_selem_id_t;

#endif /* __ALSA_MIXER_TYPES_H */
