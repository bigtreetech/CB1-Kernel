#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include  <dirent.h>
#include <poll.h>
#include "dragonboard_inc.h"

#define NUMS_DEVICES  1

//TODO this define compass attributes
#define AKM09911_DEVICE_NODE        				"/dev/iio:device1"
#define AKM09911_ATTRIBUTE_NODE_SAMPLE_RATE			"/sys/bus/iio/devices/iio:device1/sampling_frequency"
#define AKM09911_ATTRIBUTE_NODE_RATE				"/sys/bus/iio/devices/iio:device1/rate"
#define AKM09911_ATTRIBUTE_NODE_COMPASS_MATRIX		"/sys/bus/iio/devices/iio:device1/compass_matrix"
#define AKM09911_ATTRIBUTE_NODE_IN_MAGN_SCALE		"/sys/bus/iio/devices/iio:device1/in_magn_scale"
#define AKM09911_ATTRIBUTE_NODE_FILO_ENABLE			"/sys/bus/iio/devices/iio:device1/buffer/enable"
#define AKM09911_ATTRIBUTE_NODE_FILO_LENGTH			"/sys/bus/iio/devices/iio:device1/buffer/length"
#define AKM09911_ATTRIBUTE_NODE_MAGN_X_EN			"/sys/bus/iio/devices/iio:device1/scan_elements/in_magn_x_en"
#define AKM09911_ATTRIBUTE_NODE_MAGN_Y_EN			"/sys/bus/iio/devices/iio:device1/scan_elements/in_magn_y_en"
#define AKM09911_ATTRIBUTE_NODE_MAGN_Z_EN			"/sys/bus/iio/devices/iio:device1/scan_elements/in_magn_z_en"
#define AKM09911_ATTRIBUTE_NODE_TIMESTAMO_EN		"/sys/bus/iio/devices/iio:device1/scan_elements/in_timestamp_en"


struct sysfs_attrbs {
       char chip_enable[256];
       char in_timestamp_en[256];
       char trigger_name[256];
       char current_trigger[256];
       char buffer_length[256];

       char compass_enable[256];
       char compass_x_fifo_enable[256];
       char compass_y_fifo_enable[256];
       char compass_z_fifo_enable[256];
       char compass_rate[256];
       char compass_scale[256];
       char compass_orient[256];
    } compassSysFs;


static int write_sysfs_int(char *filename, int data)
{
    int res=0;
    FILE  *fp;



    fp = fopen(filename, "w");
    if (fp != NULL) {
        fprintf(fp, "%d\n", data);
        fclose(fp);
    } else {
        printf("ERR open file to write: %s\n", filename);
        res= -1;
    }
    return res;
}

/* This one DOES NOT close FDs for you */
int read_attribute_sensor(int fd, char* data, unsigned int size)
{

    int count = 0;
    if (fd > 0) {
        count = pread(fd, data, size, 0);
        if(count < 1) {
            printf("HAL:read fails with error code=%d", count);
        }
    }
    return count;
}


int inv_read_data(char *fname, long *data)
{
    char buf[sizeof(long) * 4];
    int count, fd;

    fd = open(fname, O_RDONLY);
    if(fd < 0) {
        printf("HAL:Error opening %s", fname);
        return -1;
    }
    memset(buf, 0, sizeof(buf));
    count = read_attribute_sensor(fd, buf, sizeof(buf));
    if(count < 1) {
        close(fd);
        return -1;
    } else {
        count = sscanf(buf, "%ld", data);
        if(count)
            printf("HAL:Data= %ld", *data);
    }
    close(fd);

    return 0;
}

static int sensor_get_class_path(char *class_path,char * sensor_name)
{
    char *dirname = "/sys/bus/iio/devices";
    char buf[256];
    int res;
    DIR *dir;
    struct dirent *de;
    int fd = -1;
    int found = 0;

    dir = opendir(dirname);
    if (dir == NULL)
        return -1;

    while((de = readdir(dir))) {
        if (strncmp(de->d_name, "iio:device", strlen("iio:device")) != 0) {
            continue;
        }

        sprintf(class_path, "%s/%s", dirname, de->d_name);
        snprintf(buf, sizeof(buf), "%s/name", class_path);

        fd = open(buf, O_RDONLY);
        if (fd < 0) {
            continue;
        }
        if ((res = read(fd, buf, sizeof(buf))) < 0) {
            close(fd);
            continue;
        }
        buf[res - 1] = '\0';
        if (strcmp(buf, sensor_name) == 0) {
            found = 1;
            db_msg("sensor:find sensor %s\n",sensor_name);
            close(fd);
            break;
        }

        close(fd);
        fd = -1;
    }
    closedir(dir);

    if (found) {
        return 0;
    }else {
        class_path = '\0';
        return -1;
    }

}



#define WRITE_FILE   1
int main(int argc, char *argv[])
{
		int compass_fd;
		int i;
		struct pollfd         mFds[NUMS_DEVICES];//MPU only needs one.
		char ReadBuffer[16];
		char *rdata = ReadBuffer;
		short CompassData[3];
		char buf[64];
		char class_path[256];
		char dev_buf[64];
		int pipe_count = 0;
		INIT_CMD_PIPE();
		if(sensor_get_class_path(class_path,argv[4]) < 0) {
	        printf("can't get the sensor class path\n");
	        goto err;
    	}
		sprintf(dev_buf,"%s%s","/dev/",argv[5]);
		sprintf(compassSysFs.chip_enable, "%s%s", class_path, "/buffer/enable");
		sprintf(compassSysFs.in_timestamp_en, "%s%s", class_path, "/scan_elements/in_timestamp_en");
		sprintf(compassSysFs.current_trigger, "%s%s", class_path, "/trigger/current_trigger");
		sprintf(compassSysFs.buffer_length, "%s%s", class_path, "/buffer/length");

		sprintf(compassSysFs.compass_x_fifo_enable, "%s%s", class_path, "/scan_elements/in_magn_x_en");
		sprintf(compassSysFs.compass_y_fifo_enable, "%s%s", class_path, "/scan_elements/in_magn_y_en");
		sprintf(compassSysFs.compass_z_fifo_enable, "%s%s", class_path, "/scan_elements/in_magn_z_en");
		sprintf(compassSysFs.compass_rate, "%s%s", class_path, "/sampling_frequency");
		sprintf(compassSysFs.compass_scale, "%s%s", class_path, "/in_magn_scale");
		sprintf(compassSysFs.compass_orient, "%s%s", class_path, "/compass_matrix");
		printf("argv[4] : %s\n",argv[4]);


		ssize_t nbyte = sizeof(ReadBuffer);
		{
			compass_fd = open(dev_buf,O_RDONLY);
			if(compass_fd < 0)
			{
				printf("Open akm09911 device node[%s] failed \n",AKM09911_DEVICE_NODE);
				goto err;
			}
		}
		mFds[0].fd = compass_fd;
		mFds[0].events  = POLLIN;
		mFds[0].revents = 0;
		int res =0;
		res = write_sysfs_int(compassSysFs.buffer_length,16);
		res = write_sysfs_int(compassSysFs.compass_x_fifo_enable,1);
		res = write_sysfs_int(compassSysFs.compass_y_fifo_enable,1);
		res = write_sysfs_int(compassSysFs.compass_z_fifo_enable,1);
		res = write_sysfs_int(compassSysFs.chip_enable,1);
		if(res < 0)
			goto err;

		while(1)
		{
			int n = poll(mFds, 1, -1);
			if(n > 0)
			{

				ssize_t rsize = read(mFds[0].fd,rdata,nbyte);
				if(0 > rsize)
				{
						printf("read failed\n");
						goto err;
				}
				//printf("rsize : %d\n",rsize);
				for(i = 0;i < 3; i++)
				{
					CompassData[i] = *((short *)(rdata + i * 2));
				}
				sprintf(buf,"(%d,%d,%d)",CompassData[0],CompassData[1],CompassData[2]);
				++pipe_count;
				pipe_count %= 10;
				if (!pipe_count) {
					SEND_CMD_PIPE_OK_EX(buf);
				}
			}
		}
		close(compass_fd);
		res = write_sysfs_int(compassSysFs.chip_enable,0);
err:
    SEND_CMD_PIPE_FAIL();
    EXIT_CMD_PIPE();
	return 0;
}
