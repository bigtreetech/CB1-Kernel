AM_CFLAGS = -Wall -g

lib_LTLIBRARIES = libsalsa.la
libsalsa_la_SOURCES = control.c cards.c
if BUILD_PCM
libsalsa_la_SOURCES += pcm.c pcm_params.c pcm_misc.c
endif
if BUILD_ASYNC
libsalsa_la_SOURCES += async.c
endif
if BUILD_MIXER
libsalsa_la_SOURCES += hcontrol.c mixer.c
endif
if BUILD_RAWMIDI
libsalsa_la_SOURCES += rawmidi.c
endif
if BUILD_HWDEP
libsalsa_la_SOURCES += hwdep.c
endif
if BUILD_TIMER
libsalsa_la_SOURCES += timer.c
endif

libsalsa_la_LDFLAGS = -version-info 0:1:0

alsaincludedir = $(includedir)/alsa

alsainclude_HEADERS = asoundlib.h version.h recipe.h asound.h asoundef.h \
	global.h input.h output.h error.h \
	control.h ctl_types.h ctl_macros.h \
	pcm.h pcm_types.h pcm_macros.h

if BUILD_MIXER
alsainclude_HEADERS += \
	hcontrol.h hctl_types.h hctl_macros.h \
	mixer.h mixer_types.h mixer_macros.h
endif
if BUILD_RAWMIDI
alsainclude_HEADERS += \
	rawmidi.h rawmidi_types.h rawmidi_macros.h
endif
if BUILD_HWDEP
alsainclude_HEADERS += \
	hwdep.h hwdep_types.h hwdep_macros.h
endif
if BUILD_TIMER
alsainclude_HEADERS += \
	timer.h timer_types.h timer_macros.h
endif
if BUILD_CONF
alsainclude_HEADERS += \
	conf.h
endif
if BUILD_SEQ
alsainclude_HEADERS += \
	seq.h seq_event.h seqmid.h
endif

noinst_HEADERS = local.h 

EXTRA_DIST = asoundlib-head.h asoundlib-tail.h recipe.h.in version.h.in

DISTCLEANFILES = asoundlib.h recipe.h version.h

if LINK_LIBASOUND
install-exec-hook: libsalsa.la
	rm -f $(DESTDIR)$(libdir)/libasound.so
	$(LN_S) libsalsa.so $(DESTDIR)$(libdir)/libasound.so
endif
