/** HwDep DSP status container */
typedef struct sndrv_hwdep_dsp_status snd_hwdep_dsp_status_t;

/** HwDep DSP image container */
typedef struct sndrv_hwdep_dsp_image snd_hwdep_dsp_image_t;

/** HwDep interface */
typedef enum _snd_hwdep_iface {
	SND_HWDEP_IFACE_OPL2 = 0,	/**< OPL2 raw driver */
	SND_HWDEP_IFACE_OPL3,		/**< OPL3 raw driver */
	SND_HWDEP_IFACE_OPL4,		/**< OPL4 raw driver */
	SND_HWDEP_IFACE_SB16CSP,	/**< SB16CSP driver */
	SND_HWDEP_IFACE_EMU10K1,	/**< EMU10K1 driver */
	SND_HWDEP_IFACE_YSS225,		/**< YSS225 driver */
	SND_HWDEP_IFACE_ICS2115,	/**< ICS2115 driver */
	SND_HWDEP_IFACE_SSCAPE,		/**< Ensoniq SoundScape ISA card (MC68EC000) */
	SND_HWDEP_IFACE_VX,		/**< Digigram VX cards */
	SND_HWDEP_IFACE_MIXART,		/**< Digigram miXart cards */
	SND_HWDEP_IFACE_USX2Y,		/**< Tascam US122, US224 & US428 usb */
	SND_HWDEP_IFACE_EMUX_WAVETABLE,	/**< EmuX wavetable */
	SND_HWDEP_IFACE_BLUETOOTH,	/**< Bluetooth audio */
	SND_HWDEP_IFACE_USX2Y_PCM,	/**< Tascam US122, US224 & US428 raw USB PCM */
	SND_HWDEP_IFACE_PCXHR,		/**< Digigram PCXHR */
	SND_HWDEP_IFACE_SB_RC,		/**< SB Extigy/Audigy2NX remote control */

	SND_HWDEP_IFACE_LAST = SND_HWDEP_IFACE_SB_RC  /**< last known hwdep interface */
} snd_hwdep_iface_t;

/** open for reading */
#define SND_HWDEP_OPEN_READ		(O_RDONLY)
/** open for writing */
#define SND_HWDEP_OPEN_WRITE		(O_WRONLY)
/** open for reading and writing */
#define SND_HWDEP_OPEN_DUPLEX		(O_RDWR)
/** open mode flag: open in nonblock mode */
#define SND_HWDEP_OPEN_NONBLOCK		(O_NONBLOCK)

/** HwDep handle type */
typedef enum _snd_hwdep_type {
	/** Kernel level HwDep */
	SND_HWDEP_TYPE_HW,
	/** Shared memory client HwDep (not yet implemented) */
	SND_HWDEP_TYPE_SHM,
	/** INET client HwDep (not yet implemented) */
	SND_HWDEP_TYPE_INET
} snd_hwdep_type_t;


