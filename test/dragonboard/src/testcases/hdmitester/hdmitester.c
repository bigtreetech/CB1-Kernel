/*
 * \file        hdmitester.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年06月20日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <asoundlib.h>

#include "sun8iw6p_display.h"
#include "dragonboard_inc.h"

#define HDMI_NAME "sndhdmi"

#define COLOR_WHITE                     0xffffffff
#define COLOR_YELLOW                    0xffffff00
#define COLOR_GREEN                     0xff00ff00
#define COLOR_CYAN                      0xff00ffff
#define COLOR_MAGENTA                   0xffff00ff
#define COLOR_RED                       0xffff0000
#define COLOR_BLUE                      0xff0000ff
#define COLOR_BLACK                     0xff000000

#define ION_IOC_MAGIC		'I'
#define ION_IOC_ALLOC		_IOWR(ION_IOC_MAGIC, 0, struct ion_allocation_data)
#define ION_IOC_FREE		_IOWR(ION_IOC_MAGIC, 1, struct ion_handle_data)
#define ION_IOC_MAP		_IOWR(ION_IOC_MAGIC, 2, struct ion_fd_data)
#define ION_IOC_SHARE		_IOWR(ION_IOC_MAGIC, 4, struct ion_fd_data)
#define ION_IOC_IMPORT		_IOWR(ION_IOC_MAGIC, 5, struct ion_fd_data)
#define ION_IOC_SYNC		_IOWR(ION_IOC_MAGIC, 7, struct ion_fd_data)
#define ION_IOC_CUSTOM		_IOWR(ION_IOC_MAGIC, 6, struct ion_custom_data)

#define ION_FLAG_CACHED 1		/* mappings of this buffer should be
					   cached, ion will do cache
					   maintenance when the buffer is
					   mapped for dma */
#define ION_FLAG_CACHED_NEEDS_SYNC 2	/* mappings of this buffer will created
					   at mmap time, if this is set
					   caches must be managed manually */

typedef struct {
	long 	start;
	long 	end;
}sunxi_cache_range;

typedef struct {
	void *handle;
	unsigned int phys_addr;
	unsigned int size;
}sunxi_phys_data;

#define ION_IOC_SUNXI_FLUSH_RANGE           5
#define ION_IOC_SUNXI_FLUSH_ALL             6
#define ION_IOC_SUNXI_PHYS_ADDR             7
#define ION_IOC_SUNXI_DMA_COPY              8

struct ion_allocation_data {
	size_t len;
	size_t align;
	unsigned int heap_id_mask;
	unsigned int flags;
	void *handle;
};

struct ion_handle_data {
	void *handle;
};

struct ion_fd_data {
	void *handle;
	int fd;
};

struct ion_custom_data {
	unsigned int cmd;
	unsigned long arg;
};

enum ion_heap_type {
	ION_HEAP_TYPE_SYSTEM,
	ION_HEAP_TYPE_SYSTEM_CONTIG,
	ION_HEAP_TYPE_CARVEOUT,
	ION_HEAP_TYPE_CHUNK,
	ION_HEAP_TYPE_DMA,
	ION_HEAP_TYPE_CUSTOM, /* must be last so device specific heaps always
				 are at the end of this enum */
	ION_NUM_HEAPS = 16,
};

#define ION_HEAP_SYSTEM_MASK		(1 << ION_HEAP_TYPE_SYSTEM)
#define ION_HEAP_SYSTEM_CONTIG_MASK	(1 << ION_HEAP_TYPE_SYSTEM_CONTIG)
#define ION_HEAP_CARVEOUT_MASK		(1 << ION_HEAP_TYPE_CARVEOUT)
#define ION_HEAP_TYPE_DMA_MASK		(1 << ION_HEAP_TYPE_DMA)

static unsigned int colorbar[8] =
{
	COLOR_WHITE,
	COLOR_YELLOW,
	COLOR_CYAN,
	COLOR_GREEN,
	COLOR_MAGENTA,
	COLOR_RED,
	COLOR_BLUE,
	COLOR_BLACK
};

struct hdmi_output_type
{
	int mode;
	int width;
	int height;
	char name[16];
};

#define TYPE_COUNT                      11

#if 0
static struct hdmi_output_type type_list[TYPE_COUNT] = {
    {DISP_TV_MOD_480I,       720,  480,  "480I"},       /* 0 */
    {DISP_TV_MOD_576I,       720,  576,  "576I"},       /* 1 */
    {DISP_TV_MOD_480P,       720,  480,  "480P"},       /* 2 */
    {DISP_TV_MOD_576P,       720,  576,  "576P"},       /* 3 */
    {DISP_TV_MOD_720P_50HZ,  1280, 720,  "720P 50HZ"},  /* 4 */
    {DISP_TV_MOD_720P_60HZ,  1280, 720,  "720P 60HZ"},  /* 5 */
    {DISP_TV_MOD_1080I_50HZ, 1920, 1080, "1080I 50HZ"}, /* 6 */
    {DISP_TV_MOD_1080I_60HZ, 1920, 1080, "1080I 60HZ"}, /* 7 */
    {DISP_TV_MOD_1080P_24HZ, 1920, 1080, "1080P 24HZ"}, /* 8 */
    {DISP_TV_MOD_1080P_50HZ, 1920, 1080, "1080P 50HZ"}, /* 9 */
    {DISP_TV_MOD_1080P_60HZ, 1920, 1080, "1080P 60HZ"}  /* 10 */
};
#endif

static int output_mode;
static int screen_width = 0;
static int screen_height = 0;
static unsigned char *buffer = NULL;

static int sound_play_stop;

#if defined (_SUN8IW6P) || (_SUN50IW1P) || (_SUN50IW3P)

static disp_layer_config layer_config;
#elif defined (_SUN8IW15P) || (_SUN50IW6P) || (_SUN8IW7P)

static disp_layer_config2 layer_config;
#endif

static int disp;

#define BUF_LEN                         4096
char *buf[BUF_LEN];

static int init_layer(void)
{
	struct ion_allocation_data alloc_data;
	struct ion_handle_data handle_data;
	struct ion_fd_data fd_data;
	struct ion_custom_data custom_data;
	sunxi_phys_data phys_data = {0, 0, 0};
	unsigned int arg[3];
	int fd, ret = 0;
	int i, j;
	unsigned int *p;
	(void)arg;

	/* 1. open ion device */
	fd = open("/dev/ion", O_RDONLY);
	if(fd < 0) {
		db_error("####################err open /dev/ion\n");
		return -1;
	}
	/* 2. alloc buffer */
	alloc_data.len = screen_width * screen_height * 4;
	alloc_data.align = 0;
#if defined (_SUN8IW15P) || (_SUN50IW6P) || (_SUN8IW7P)
	alloc_data.heap_id_mask = ION_HEAP_SYSTEM_MASK;
#else
	alloc_data.heap_id_mask = ION_HEAP_TYPE_DMA_MASK;
#endif

	alloc_data.flags = ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC;
	ret = ioctl(fd, ION_IOC_ALLOC, &alloc_data);
	if(ret) {
		db_error("######################ION_IOC_ALLOC err, ret %d, handle 0x%08x\n", ret, (unsigned int)alloc_data.handle);
		goto out1;
	}
	/* 3. get dmabuf fd */
	fd_data.handle = alloc_data.handle;
	ret = ioctl(fd, ION_IOC_MAP, &fd_data);
	if(ret) {
		db_error("##########################ION_IOC_MAP err, ret %d, dmabuf fd 0x%08x\n", ret, (unsigned int)fd_data.fd);
		goto out2;
	}
	/* 4. mmap to user space */
	buffer = mmap(NULL, alloc_data.len, PROT_READ|PROT_WRITE, MAP_SHARED, fd_data.fd, 0);
	if(MAP_FAILED == buffer) {
		db_error("#####################mmap err, ret %d\n", (unsigned int)buffer);
		goto out3;
	}
	memset(buffer, 0, alloc_data.len);

#if !defined (_SUN8IW15P) && !defined (_SUN50IW6P)  || !defined (_SUN8IW7P)

	/* optional: get buffer phys_addr */
	custom_data.cmd = ION_IOC_SUNXI_PHYS_ADDR;
	phys_data.handle = alloc_data.handle;
	custom_data.arg = (unsigned long)&phys_data;
	ret = ioctl(fd, ION_IOC_CUSTOM, &custom_data);
	if(ret) {
		db_error("###############################ION_IOC_SUNXI_PHYS_ADDR err, ret %d\n", ret);
		goto out4;
	}
	db_msg("ION_IOC_SUNXI_PHYS_ADDR succes, phys_addr 0x%08x, size 0x%08x\n", phys_data.phys_addr, phys_data.size);
#endif

#if defined (_SUN8IW6P) || (_SUN50IW1P) || (_SUN50IW3P)
	//设置图层参数
	memset(&layer_config, 0, sizeof(disp_layer_config));

	layer_config.info.mode	= LAYER_MODE_BUFFER;
	layer_config.info.fb.addr[0]       = (__u32)phys_data.phys_addr; //FB地址
	layer_config.info.fb.addr[1]		  = (__u32)(layer_config.info.fb.addr[0] + screen_width * screen_height / 3 * 1);
	layer_config.info.fb.addr[2]		  = (__u32)(layer_config.info.fb.addr[0] + screen_width * screen_height / 3 * 2);

	layer_config.info.fb.format        = DISP_FORMAT_ARGB_8888; //DISP_FORMAT_YUV420_P
	layer_config.info.fb.size[0].width    = screen_width;
	layer_config.info.fb.size[0].height	  = screen_height;

	layer_config.info.fb.crop.x     = 0;
	layer_config.info.fb.crop.y     = 0;
	layer_config.info.fb.crop.width = (unsigned long long)screen_width << 32;
	layer_config.info.fb.crop.height= (unsigned long long)screen_height << 32;

	layer_config.info.screen_win.x	= 0;
	layer_config.info.screen_win.y 	= 0;
	layer_config.info.screen_win.width = screen_width;
	layer_config.info.screen_win.height = screen_height;
	db_msg("########screen_width=%d\n",screen_width);
	db_msg("########screen_height=%d\n",screen_height);

	layer_config.info.alpha_mode       = 0; //pixel alpha
	layer_config.info.alpha_value      = 0;
	//layer_config.info.pipe             = 1;
	layer_config.channel = 0;
	layer_config.layer_id = 0;
	layer_config.enable = 1;
	layer_config.info.zorder = 0;

	arg[0] = 1;//screen_id
	arg[1] = (unsigned int)&layer_config;
	arg[2] = 1;
	ret = ioctl(disp, DISP_LAYER_SET_CONFIG, (void*)arg);
	if (ret != 0) {
			db_error("fail to set layer info\n");
	}
#elif defined (_SUN8IW15P) || (_SUN50IW6P) || (_SUN8IW7P)
	//设置图层参数
	memset(&layer_config, 0, sizeof(disp_layer_config2));

	layer_config.info.mode	= LAYER_MODE_BUFFER;
	layer_config.info.fb.fd       = fd_data.fd; //FB地址

	layer_config.info.fb.format        = DISP_FORMAT_ARGB_8888; //DISP_FORMAT_YUV420_P
	layer_config.info.fb.size[0].width    = screen_width;
	layer_config.info.fb.size[0].height	  = screen_height;

	layer_config.info.fb.crop.x     = 0;
	layer_config.info.fb.crop.y     = 0;
	layer_config.info.fb.crop.width = (unsigned long long)screen_width << 32;
	layer_config.info.fb.crop.height= (unsigned long long)screen_height << 32;

	layer_config.info.screen_win.x	= 0;
	layer_config.info.screen_win.y 	= 0;
	layer_config.info.screen_win.width = screen_width;
	layer_config.info.screen_win.height = screen_height;
	db_msg("########screen_width=%d\n",screen_width);
	db_msg("########screen_height=%d\n",screen_height);

	layer_config.info.alpha_mode       = 0; //pixel alpha
	layer_config.info.alpha_value      = 0;
	//layer_config.info.pipe             = 1;
	layer_config.channel = 2;
	layer_config.layer_id = 0;
	layer_config.enable = 1;
	layer_config.info.zorder = 2;

	arg[0] = 0;//screen_id
	arg[1] = (unsigned int)&layer_config;
	arg[2] = 1;
	ret = ioctl(disp, DISP_LAYER_SET_CONFIG2, (void*)arg);
	if (ret != 0) {
			db_error("fail to set layer info\n");
		}
#endif


	for(i = 0; i < screen_height; i++)
	{
		p = (unsigned int *)(buffer + screen_width * i * 4);
		for(j = 0; j < screen_width; j++)
		{
		int idx = (j << 3) / screen_width;
		*p++ = colorbar[idx];
		}
	}

	return 0;

out4:
	/* unmmap user buffer */
	ret = munmap(buffer, alloc_data.len);
	if(ret)
		db_error("munmap err, ret %d\n", ret);
out3:
	/* close dmabuf fd */
	close(fd_data.fd);
out2:
	/* free buffer */
	handle_data.handle = alloc_data.handle;
	ret = ioctl(fd, ION_IOC_FREE, &handle_data);
	if(ret)
		db_error("ION_IOC_FREE err, ret %d\n", ret);
out1:
	/* close ion device */
	close(fd);
	return ret;
}

static __attribute__((unused)) void exit_layer(void)
{
#if 1
	munmap(buffer, screen_width * screen_height * 4);
#endif
}

#if 0
static int detect_output_mode(void)
{
	unsigned int args[4];
	int ret;
	int support_mode;

	args[0] = 1;
	args[1] = DISP_TV_MOD_1080P_50HZ;
	ret = ioctl(disp, DISP_HDMI_SUPPORT_MODE, args);
	if (ret == 1) {
		db_msg("hdmitester: your device support 1080p 50Hz\n");
		output_mode = DISP_TV_MOD_1080P_50HZ;
		screen_width  = 1920;
		screen_height = 1080;
	}
	else {
		args[0] = 1;
		args[1] = DISP_TV_MOD_720P_50HZ;
		ret = ioctl(disp, DISP_HDMI_SUPPORT_MODE, args);
		if (ret == 1) {
			db_msg("hdmitester: your device support 720p 50Hz\n");
			output_mode = DISP_TV_MOD_720P_50HZ;
			screen_width  = 1280;
			screen_height = 720;
		}
		else {
			db_msg("hdmitester: your device do not support neither 1080p nor 720p (50Hz)\n");
			if (script_fetch("hdmi", "support_mode", &support_mode, 1)) {
				support_mode = 2;
				db_msg("hdmitester: can't fetch user config mode, use default mode: %s\n",
				type_list[support_mode].name);
			}
			else if (support_mode < 0 || support_mode >= TYPE_COUNT) {
				support_mode = 2;
				db_msg("hdmitester: user config mode invalid, use default mode: %s\n",
				type_list[support_mode].name);
			}
		db_msg("hdmitester: use user config mode: %s\n", type_list[support_mode].name);
		args[0] = 1;
		args[1] = type_list[support_mode].mode;
		ret = ioctl(disp, DISP_HDMI_SUPPORT_MODE, args);
		if (ret == 1) {
			db_msg("hdmitester: you device support %s\n", type_list[support_mode].name);
			output_mode = type_list[support_mode].mode;
			screen_width  = type_list[support_mode].width;
			screen_height = type_list[support_mode].height;
		}
		else {
			db_msg("hdmitester: you device do not support %s\n",
			type_list[support_mode].name);
			return -1;
		}
		}
	}

	return 0;
}
#endif

static int xrun_recovery(snd_pcm_t *handle, int err)
{
	if (err == -EPIPE) {
		err = snd_pcm_prepare(handle);
	}

	if (err < 0) {
		db_warn("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
	}
	else if (err == -ESTRPIPE) {
		while ((err = snd_pcm_resume(handle)) == -EAGAIN) {
			sleep(1);

			if (err < 0) {
				err = snd_pcm_prepare(handle);
			}
			if (err < 0) {
				db_warn("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
			}
		}

	return 0;
	}

	return err;
}

static int get_card(const char *card_name)
{
	int ret;
	int fd;
	int i;
	char path[128];
	char name[64];

	for (i = 0; i < 10; i++) {
		sprintf(path, "/sys/class/sound/card%d/id", i);
		ret = access(path, F_OK);
		if (ret) {
			db_warn("can't find node %s, use card1\n", path);
			return 1;
		}

	fd = open(path, O_RDONLY);
	if (fd <= 0) {
		db_warn("can't open %s, use card1\n", path);
		return 1;
	}

	ret = read(fd, name, sizeof(name));
	close(fd);
	if (ret > 0) {
		name[ret-1] = '\0';
		if (NULL != strstr(name, card_name))
			return i;
	}
	}

	db_warn("can't find card:%s, use card1", card_name);
	return 1;
}

static __attribute__((unused)) void *sound_play(void *args)
{
	char path[256];
	int samplerate;
	int err;
	snd_pcm_t *playback_handle;
	snd_pcm_hw_params_t *hw_params;
	char snd_dev[10];
	FILE *fp;

	db_msg("prepare play sound...\n");
	if (script_fetch("hdmi", "sound_file", (int *)path, sizeof(path) / 4)) {
		db_warn("unknown sound file, use default\n");
		strcpy(path, "/dragonboard/data/test48000.pcm");
	}
	if (script_fetch("hdmi", "samplerate", &samplerate, 1)) {
		db_warn("unknown samplerate, use default #48000\n");
		samplerate = 48000;
	}
	db_msg("samplerate #%d\n", samplerate);

	sprintf(snd_dev, "hw:%d,0", get_card(HDMI_NAME));
	db_msg("hdmi snd dev:%s\n", snd_dev);
	err = snd_pcm_open(&playback_handle, snd_dev, SND_PCM_STREAM_PLAYBACK, 0);
	if (err < 0) {
		db_error("cannot open audio device (%s)\n", snd_strerror(err));
		pthread_exit((void *)-1);
	}

	err = snd_pcm_hw_params_malloc(&hw_params);
	if (err < 0) {
		db_error("cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
		pthread_exit((void *)-1);
	}

	err = snd_pcm_hw_params_any(playback_handle, hw_params);
	if (err < 0) {
		db_error("cannot initialize hardware parameter structure (%s)\n", snd_strerror(err));
		pthread_exit((void *)-1);
	}

	err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if (err < 0) {
		db_error("cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
		pthread_exit((void *)-1);
	}

	err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE);
	if (err < 0) {
		db_error("cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
		pthread_exit((void *)-1);
	}

	err = snd_pcm_hw_params_set_rate(playback_handle, hw_params, samplerate, 0);
	if (err < 0) {
		db_error("cannot set sample rate (%s)\n", snd_strerror(err));
		pthread_exit((void *)-1);
	}

	err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2);
	if (err < 0) {
		db_error("cannot set channel count (%s), err = %d\n", snd_strerror(err), err);
		pthread_exit((void *)-1);
	}

	err = snd_pcm_hw_params(playback_handle, hw_params);
	if (err < 0) {
		db_error("cannot set parameters (%s)\n", snd_strerror(err));
		pthread_exit((void *)-1);
	}

	snd_pcm_hw_params_free(hw_params);

	db_msg("open test pcm file: %s\n", path);
	fp = fopen(path, "r");
	if (fp == NULL) {
		db_error("cannot open test pcm file(%s)\n", strerror(errno));
		pthread_exit((void *)-1);
	}

	db_msg("play it...\n");
	while (1) {
		while (!feof(fp)) {
			if (sound_play_stop) {
				goto out;
			}

			err = fread(buf, 1, BUF_LEN, fp);
			if (err < 0) {
				db_warn("read test pcm failed(%s)\n", strerror(errno));
			}

			err = snd_pcm_writei(playback_handle, buf, BUF_LEN/4);
			if (err < 0) {
				err = xrun_recovery(playback_handle, err);
				if (err < 0) {
					db_warn("write error: %s\n", snd_strerror(err));
				}
			}

			if (err == -EBADFD) {
				db_warn("PCM is not in the right state (SND_PCM_STATE_PREPARED or SND_PCM_STATE_RUNNING)\n");
			}
			if (err == -EPIPE) {
				db_warn("an underrun occurred\n");
			}
			if (err == -ESTRPIPE) {
				db_warn("a suspend event occurred (stream is suspended and waiting for an application recovery)\n");
			}

			if (feof(fp)) {
				fseek(fp, 0L, SEEK_SET);
			}
		}
	}

out:
	db_msg("play end...\n");
	fclose(fp);
	snd_pcm_close(playback_handle);
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	unsigned int args[4];
	int status = 0;
	int ret;
	pthread_t tid;
	disp_output_type output_type;
	int mic_activated;
	int fd_hdmi = -1;
	char val[6];
	(void)tid;

	INIT_CMD_PIPE();

	init_script(atoi(argv[2]));
	disp = open("/dev/disp",O_RDWR);
	if (disp == -1) {
		db_error("hdmitester: open /dev/disp failed(%s)\n", strerror(errno));
		goto err;
	}
	args[0] = 0;
	output_type = ioctl(disp, DISP_GET_OUTPUT_TYPE,(void*)args);
	if(script_fetch("mic", "activated", &mic_activated,1)){
		mic_activated=0;
	}

   /* test main loop */
	while (1) {
	if (output_type==DISP_OUTPUT_TYPE_LCD)
		args[0] = 1;
	else
		args[0] = 0;

	if (status == 1) {
		sleep(1);
		continue;
	}
	sleep(1);
	fd_hdmi = open("/sys/class/extcon/hdmi/state", O_RDONLY);
	if (fd_hdmi < 0){
		db_error("hdmitester: open /sys/class/extcon/hdmi/state failed\n");
	}
	ret = read(fd_hdmi, val, 6);
	close(fd_hdmi);
	if (val[5] == '1') {
#if 0
		ret = detect_output_mode();
		if (ret < 0) {
			goto err;
		}
#else
		output_mode = DISP_TV_MOD_1080P_60HZ;
		screen_width  = 1920;
		screen_height = 1080;
#endif
		/*
		db_msg("hdmitester: read /sys/class/extcon/hdmi/state 1\n");
		 */
		args[0] = 1;
		args[1] = DISP_OUTPUT_TYPE_HDMI;
		args[2] = output_mode;

		if (output_type==DISP_OUTPUT_TYPE_LCD) {
			ret = ioctl(disp, DISP_DEVICE_SWITCH, (unsigned long)args);
			if (ret < 0) {
				db_error("hdmitester:switch to hdmimode faile!\n");
			}
		}

		if (output_type==DISP_OUTPUT_TYPE_LCD) {
			ret = init_layer();
			if (ret < 0) {
				db_error("hdmitester: init layer failed\n");
				goto err;
			}
		}
		/* create sound play thread */
/*		sound_play_stop = 0;
		ret = pthread_create(&tid, NULL, sound_play, NULL);
		if (ret != 0) {
			db_error("hdmitester: create sound play thread failed\n");
			exit_layer();
			goto err;
		}
		status = 1;
*/
		SEND_CMD_PIPE_OK();
	}
	}

err:
	SEND_CMD_PIPE_FAIL();
	close(disp);
	close(fd_hdmi);
	deinit_script();
	return -1;
}
