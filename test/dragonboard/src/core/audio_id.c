#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "audio_id.h"

#if defined(_SUN50IW6P1)
#define AUDIO_MAP_CNT 16
#define NAME_LEN 20
#define PATH_LEN 30

int get_audio_device_id(char *device_name,int length)
{
	char path[PATH_LEN] = {0};
	char name[NAME_LEN] = {0};
	int index = 0;
	int ret = -1;
	int fd = -1;
	for(; index < AUDIO_MAP_CNT; index++)
	{
		memset(path, 0, PATH_LEN);
		memset(name, 0, NAME_LEN);
		sprintf(path, "/proc/asound/card%d/id", index);

		fd = open(path, O_RDONLY, 0);
		if(fd < 0)
		{return -1;}

		ret = read(fd, name, NAME_LEN);
		if(ret < 0)
			return -1;
		if(!strncmp(name,device_name,length))
			return index;
	}

	return -1;
}
#endif
