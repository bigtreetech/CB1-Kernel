AC_PREREQ(2.59)
AC_INIT(src/control.c)
AM_INIT_AUTOMAKE(salsa-lib, 0.0.22)
AC_PREFIX_DEFAULT(/usr)

dnl AC_CONFIG_HEADERS(src/config.h)

AC_PROG_CC
AC_PROG_INSTALL
dnl AC_DISABLE_STATIC
AM_PROG_LIBTOOL
AC_HEADER_STDC

dnl Compatible version
AC_ARG_WITH(compat-version,
  AS_HELP_STRING([--with-compat-version=VERSION],
		 [specify the compatible version with ALSA-lib (default=1.0.18)]),
  compatver="$withval", compatver="1.0.18")

SND_LIB_VERSION="$compatver"
SND_LIB_MAJOR=1
SND_LIB_MINOR=0
SND_LIB_SUBMINOR=18
SND_LIB_EXTRAVER=1000000
case "$compatver" in
*.*.*)
  SND_LIB_MAJOR=`echo $SND_LIB_VERSION | cut -d . -f 1`
  SND_LIB_MINOR=`echo $SND_LIB_VERSION | cut -d . -f 2`
  SND_LIB_SUBMINOR=`echo $SND_LIB_VERSION | cut -d . -f 3`
  ;;
esac
AC_SUBST(SND_LIB_VERSION)
AC_SUBST(SND_LIB_MAJOR)
AC_SUBST(SND_LIB_MINOR)
AC_SUBST(SND_LIB_SUBMINOR)
AC_SUBST(SND_LIB_EXTRAVER)

dnl Non-standard device files directory
AC_MSG_CHECKING([for ALSA device file directory])
AC_ARG_WITH(alsa-devdir,
  AS_HELP_STRING([--with-alsa-devdir=dir],
		 [directory with ALSA device files (default /dev/snd)]),
  [alsa_dev_dir="$withval"],
  [alsa_dev_dir="/dev/snd"])
DEVPATH="$alsa_dev_dir"
AC_SUBST(DEVPATH)
AC_MSG_RESULT([$alsa_dev_dir])

dnl Component selections

AC_ARG_ENABLE(pcm,
  AS_HELP_STRING([--disable-pcm],
		 [disable PCM interface]),
  pcm="$enableval", pcm="yes")
AC_ARG_ENABLE(mixer,
  AS_HELP_STRING([--disable-mixer],
		 [disable mixer interface]),
  mixer="$enableval", mixer="yes")
AC_ARG_ENABLE(rawmidi,
  AS_HELP_STRING([--enable-rawmidi],
		 [enable rawmidi interface]),
  rawmidi="$enableval", rawmidi="no")
AC_ARG_ENABLE(hwdep,
  AS_HELP_STRING([--enable-hwdep],
		 [enable hwdep interface]),
  hwdep="$enableval", hwdep="no")
AC_ARG_ENABLE(timer,
  AS_HELP_STRING([--enable-timer],
		 [enable timer interface]),
  timer="$enableval", timer="no")
AC_ARG_ENABLE(conf,
  AS_HELP_STRING([--enable-conf],
		 [enable dummy conf functions]),
  sndconf="$enableval", sndconf="no")
AC_ARG_ENABLE(seq,
  AS_HELP_STRING([--enable-seq],
		 [enable seq functions]),
  sndseq="$enableval", sndseq="no")

AC_ARG_ENABLE(tlv,
  AS_HELP_STRING([--enable-tlv],
	 	 [enable TLV (dB) support]),
  tlv="$enableval", tlv="no")
AC_ARG_ENABLE(async,
  AS_HELP_STRING([--enable-async],
		 [enable async handler support]),
  async="$enableval", async="no")

AC_ARG_ENABLE(libasound,
  AS_HELP_STRING([--enable-libasound],
		 [build a symlink to libasound.so]),
  libasound="$enableval", libasound="no")

AC_ARG_ENABLE(everything,
  AS_HELP_STRING([--enable-everyhing],
		 [enable everyhing :)]),
  everything="$enableval", everything="no")

if test "$everything" = "yes"; then
  pcm="yes"
  mixer="yes"
  rawmidi="yes"
  hwdep="yes"
  timer="yes"
  sndconf="yes"
  sndseq="yes"
  tlv="yes"
  async="yes"
  libasound="yes"
fi

AM_CONDITIONAL(BUILD_PCM, test "$pcm" = "yes")
AM_CONDITIONAL(BUILD_MIXER, test "$mixer" = "yes")
AM_CONDITIONAL(BUILD_RAWMIDI, test "$rawmidi" = "yes")
AM_CONDITIONAL(BUILD_HWDEP, test "$hwdep" = "yes")
AM_CONDITIONAL(BUILD_TIMER, test "$timer" = "yes")
AM_CONDITIONAL(BUILD_CONF, test "$sndconf" = "yes")
AM_CONDITIONAL(BUILD_SEQ, test "$sndseq" = "yes")
AM_CONDITIONAL(BUILD_ASYNC, test "$async" = "yes")

if test "$tlv" = "yes"; then
  SALSA_HAS_TLV_SUPPORT=1
else
  SALSA_HAS_TLV_SUPPORT=0
fi
AC_SUBST(SALSA_HAS_TLV_SUPPORT)

if test "$async" = "yes"; then
  SALSA_HAS_ASYNC_SUPPORT=1
else
  SALSA_HAS_ASYNC_SUPPORT=0
fi
AC_SUBST(SALSA_HAS_ASYNC_SUPPORT)

if test "$sndconf" = "yes"; then
  SALSA_HAS_DUMMY_CONF=1
else
  SALSA_HAS_DUMMY_CONF=0
fi
AC_SUBST(SALSA_HAS_DUMMY_CONF)

AM_CONDITIONAL(LINK_LIBASOUND, test "$libasound" = "yes")

AC_ARG_ENABLE(deprecated,
  AS_HELP_STRING([--disable-deprecated],
		 [don't mark deprecated attribute for non-working functions]),
  markdeprecated="$enableval", markdeprecated="yes")

if test "$markdeprecated" = "yes"; then
  SALSA_MARK_DEPRECATED=1
else
  SALSA_MARK_DEPRECATED=0
fi
AC_SUBST(SALSA_MARK_DEPRECATED)

dnl OK, let's output...

AC_OUTPUT([
	Makefile
	alsa.pc
	src/Makefile
	src/recipe.h
	src/version.h
])

echo "Creating asoundlib.h..."
cp "$srcdir"/src/asoundlib-head.h src/asoundlib.h
test "$pcm" = "yes" && echo "#include \"pcm.h\"" >> src/asoundlib.h
test "$mixer" = "yes" && echo "#include \"mixer.h\"" >> src/asoundlib.h
test "$rawmidi" = "yes" && echo "#include \"rawmidi.h\"" >> src/asoundlib.h
test "$hwdep" = "yes" && echo "#include \"hwdep.h\"" >> src/asoundlib.h
test "$timer" = "yes" && echo "#include \"timer.h\"" >> src/asoundlib.h
test "$sndconf" = "yes" && echo "#include \"conf.h\"" >> src/asoundlib.h
test "$sndseq" = "yes" && echo "#include \"seq_event.h\"" >> src/asoundlib.h
test "$sndseq" = "yes" && echo "#include \"seq.h\"" >> src/asoundlib.h
test "$sndseq" = "yes" && echo "#include \"seqmid.h\"" >> src/asoundlib.h
cat "$srcdir"/src/asoundlib-tail.h >> src/asoundlib.h

echo "SALSA-Lib build configuration:"
echo "  - PCM interface: $pcm"
echo "  - Mixer interface: $mixer"
echo "  - Raw MIDI interface: $rawmidi"
echo "  - HW-dependent interface: $hwdep"
echo "  - Timer interface: $timer"
echo "  - ALSA-config dummy interface: $sndconf"
echo "  - ALSA-sequencer dummy interface: $sndseq"
echo "  - TLV (dB) support: $tlv"
echo "  - Async handler support: $async"
echo "  - Make libasound.so symlink: $libasound"
echo "  - Mark deprecated attribute: $markdeprecated"
