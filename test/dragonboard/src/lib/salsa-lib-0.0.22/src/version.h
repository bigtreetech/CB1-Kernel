#ifndef __ALSA_VERSION_H
#define __ALSA_VERSION_H

#define SND_LIB_MAJOR		1
#define SND_LIB_MINOR		0
#define SND_LIB_SUBMINOR	18
#define SND_LIB_EXTRAVER	1000000
/** library version */
#define SND_LIB_VERSION		((SND_LIB_MAJOR<<16)|\
				 (SND_LIB_MINOR<<8)|\
				  SND_LIB_SUBMINOR)
/** library version (string) */
#define SND_LIB_VERSION_STR	"1.0.18"

static inline
const char *snd_asoundlib_version(void)
{
	return SND_LIB_VERSION_STR;
}

#endif /* __ALSA_VERSION_H */
