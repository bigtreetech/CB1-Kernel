#ifndef __ALSA_TIMER_TYPES_H
#define __ALSA_TIMER_TYPES_H

/** timer identification structure */
typedef struct sndrv_timer_id snd_timer_id_t;
/** timer global info structure */
typedef struct sndrv_timer_ginfo snd_timer_ginfo_t;
/** timer global params structure */
typedef struct sndrv_timer_gparams snd_timer_gparams_t;
/** timer global status structure */
typedef struct sndrv_timer_gstatus snd_timer_gstatus_t;
/** timer info structure */
typedef struct sndrv_timer_info snd_timer_info_t;
/** timer params structure */
typedef struct sndrv_timer_params snd_timer_params_t;
/** timer status structure */
typedef struct sndrv_timer_status snd_timer_status_t;
/** timer master class */
typedef enum _snd_timer_class {
	SND_TIMER_CLASS_NONE = -1,	/**< invalid */
	SND_TIMER_CLASS_SLAVE = 0,	/**< slave timer */
	SND_TIMER_CLASS_GLOBAL,		/**< global timer */
	SND_TIMER_CLASS_CARD,		/**< card timer */
	SND_TIMER_CLASS_PCM,		/**< PCM timer */
	SND_TIMER_CLASS_LAST = SND_TIMER_CLASS_PCM	/**< last timer */
} snd_timer_class_t;

/** timer slave class */
typedef enum _snd_timer_slave_class {
	SND_TIMER_SCLASS_NONE = 0,		/**< none */
	SND_TIMER_SCLASS_APPLICATION,		/**< for internal use */
	SND_TIMER_SCLASS_SEQUENCER,		/**< sequencer timer */
	SND_TIMER_SCLASS_OSS_SEQUENCER,		/**< OSS sequencer timer */
	SND_TIMER_SCLASS_LAST = SND_TIMER_SCLASS_OSS_SEQUENCER	/**< last slave timer */
} snd_timer_slave_class_t;

/** timer read event identification */
typedef enum _snd_timer_event {
	SND_TIMER_EVENT_RESOLUTION = 0,	/* val = resolution in ns */
	SND_TIMER_EVENT_TICK,		/* val = ticks */
	SND_TIMER_EVENT_START,		/* val = resolution in ns */
	SND_TIMER_EVENT_STOP,		/* val = 0 */
	SND_TIMER_EVENT_CONTINUE,	/* val = resolution in ns */
	SND_TIMER_EVENT_PAUSE,		/* val = 0 */
	SND_TIMER_EVENT_EARLY,		/* val = 0 */
	SND_TIMER_EVENT_SUSPEND,	/* val = 0 */
	SND_TIMER_EVENT_RESUME,		/* val = resolution in ns */
	/* master timer events for slave timer instances */
	SND_TIMER_EVENT_MSTART = SND_TIMER_EVENT_START + 10,
	SND_TIMER_EVENT_MSTOP = SND_TIMER_EVENT_STOP + 10,
	SND_TIMER_EVENT_MCONTINUE = SND_TIMER_EVENT_CONTINUE + 10,
	SND_TIMER_EVENT_MPAUSE = SND_TIMER_EVENT_PAUSE + 10,
	SND_TIMER_EVENT_MSUSPEND = SND_TIMER_EVENT_SUSPEND + 10,
	SND_TIMER_EVENT_MRESUME = SND_TIMER_EVENT_RESUME + 10
} snd_timer_event_t;

/** timer read structure */
typedef struct _snd_timer_read {
	unsigned int resolution;	/**< tick resolution in nanoseconds */
        unsigned int ticks;		/**< count of happened ticks */
} snd_timer_read_t;

/** timer tstamp + event read structure */
typedef struct _snd_timer_tread {
	snd_timer_event_t event;	/**< Timer event */
	snd_htimestamp_t tstamp;	/**< Time stamp of each event */
	unsigned int val;		/**< Event value */
} snd_timer_tread_t;

/** global timer - system */
#define SND_TIMER_GLOBAL_SYSTEM 0
/** global timer - RTC */
#define SND_TIMER_GLOBAL_RTC 	1
/** global timer - HPET */
#define SND_TIMER_GLOBAL_HPET	2

/** timer open mode flag - non-blocking behaviour */
#define SND_TIMER_OPEN_NONBLOCK		(1<<0)
/** use timestamps and event notification - enhanced read */
#define SND_TIMER_OPEN_TREAD		(1<<1)

/** timer handle type */
typedef enum _snd_timer_type {
	/** Kernel level HwDep */
	SND_TIMER_TYPE_HW = 0,
	/** Shared memory client timer (not yet implemented) */
	SND_TIMER_TYPE_SHM,
	/** INET client timer (not yet implemented) */
	SND_TIMER_TYPE_INET
} snd_timer_type_t;

/** timer query handle */
typedef struct _snd_timer_query snd_timer_query_t;
/** timer handle */
typedef struct _snd_timer snd_timer_t;

#endif /* __ALSA_TIMER_TYPES_H */
