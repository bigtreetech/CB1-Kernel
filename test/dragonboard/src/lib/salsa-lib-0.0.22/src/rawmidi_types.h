#ifndef __ALSA_RAWMIDI_TYPES_H
#define __ALSA_RAWMIDI_TYPES_H

/** RawMidi settings container */
typedef struct sndrv_rawmidi_params snd_rawmidi_params_t;
/** RawMidi status container */
typedef struct sndrv_rawmidi_status snd_rawmidi_status_t;

/** RawMidi stream (direction) */
typedef enum _snd_rawmidi_stream {
	/** Output stream */
	SND_RAWMIDI_STREAM_OUTPUT = 0,
	/** Input stream */
	SND_RAWMIDI_STREAM_INPUT,
	SND_RAWMIDI_STREAM_LAST = SND_RAWMIDI_STREAM_INPUT
} snd_rawmidi_stream_t;

/** Append (flag to open mode) \hideinitializer */
#define SND_RAWMIDI_APPEND	0x0001
/** Non blocking mode (flag to open mode) \hideinitializer */
#define SND_RAWMIDI_NONBLOCK	0x0002
/** Write sync mode (Flag to open mode) \hideinitializer */
#define SND_RAWMIDI_SYNC	0x0004

/** RawMidi type */
typedef enum _snd_rawmidi_type {
	/** Kernel level RawMidi */
	SND_RAWMIDI_TYPE_HW,
	/** Shared memory client RawMidi (not yet implemented) */
	SND_RAWMIDI_TYPE_SHM,
	/** INET client RawMidi (not yet implemented) */
	SND_RAWMIDI_TYPE_INET,
	/** Virtual (sequencer) RawMidi */
	SND_RAWMIDI_TYPE_VIRTUAL
} snd_rawmidi_type_t;

#endif /* __ALSA_RAWMIDI_TYPES_H */
