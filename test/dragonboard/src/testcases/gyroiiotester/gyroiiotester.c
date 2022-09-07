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
#define IIO_BUFFER_LENGTH   480
#define IIO_BUFFER_EN   1
//currently, we depends on mpu6500's iio sysfs.
#define MPU6500_DEVICE_NODE                   "/dev/iio:device0"
#define MPU6500_ATTRIBUTE_FIFO_SAMPLE_RATE			"/sys/bus/iio/devices/iio:device0/sampling_frequency"
#define MPU6500_ATTRIBUTE_TEMPERATURE			"/sys/bus/iio/devices/iio:device0/temperature"
#define MPU6500_ATTRIBUTE_NODE_DMP_ON     "/sys/bus/iio/devices/iio:device0/dmp_on"
#define MPU6500_ATTRIBUTE_NODE_DMP_INT_ON     "/sys/bus/iio/devices/iio:device0/dmp_int_on"
#define MPU6500_ATTRIBUTE_NODE_CHIP_ENABLE     "/sys/bus/iio/devices/iio:device0/buffer/enable"
#define MPU6500_ATTRIBUTE_NODE_BUF_LENGTH     "/sys/bus/iio/devices/iio:device0/buffer/length"
#define MPU6500_ATTRIBUTE_NODE_POWER_STATE     "/sys/bus/iio/devices/iio:device0/power_state"
#define MPU6500_ATTRIBUTE_NODE_GYRO_SCALE     "/sys/bus/iio/devices/iio:device0/in_anglvel_scale"
#define MPU6500_ATTRIBUTE_NODE_GYRO_ENABLE			"/sys/bus/iio/devices/iio:device0/gyro_enable"
#define MPU6500_ATTRIBUTE_NODE_GYRO_MATRIX			"/sys/bus/iio/devices/iio:device0/gyro_matrix"
#define MPU6500_ATTRIBUTE_NODE_GYRO_X_FIFO_ENABLE			"/sys/bus/iio/devices/iio:device0/scan_elements/in_anglvel_x_en"
#define MPU6500_ATTRIBUTE_NODE_GYRO_X_BIAS			"/sys/bus/iio/devices/iio:device0/scan_elements/in_anglvel_x_calibbias"
#define MPU6500_ATTRIBUTE_NODE_GYRO_Y_BIAS			"/sys/bus/iio/devices/iio:device0/scan_elements/in_anglvel_y_calibbias"
#define MPU6500_ATTRIBUTE_NODE_GYRO_Z_BIAS			"/sys/bus/iio/devices/iio:device0/scan_elements/in_anglvel_z_calibbias"
#define MPU6500_ATTRIBUTE_NODE_GYRO_Y_FIFO_ENABLE			"/sys/bus/iio/devices/iio:device0/scan_elements/in_anglvel_y_en"
#define MPU6500_ATTRIBUTE_NODE_GYRO_Z_FIFO_ENABLE			"/sys/bus/iio/devices/iio:device0/scan_elements/in_anglvel_z_en"
#define MPU6500_ATTRIBUTE_NODE_ACCEL_SCALE			"/sys/bus/iio/devices/iio:device0/in_accel_scale"
#define MPU6500_ATTRIBUTE_NODE_ACCEL_ENABLE			"/sys/bus/iio/devices/iio:device0/accl_enable"
#define MPU6500_ATTRIBUTE_NODE_ACCEL_X_FIFO_ENABLE			"/sys/bus/iio/devices/iio:device0/scan_elements/in_accel_x_en"
#define MPU6500_ATTRIBUTE_NODE_ACCEL_Y_FIFO_ENABLE			"/sys/bus/iio/devices/iio:device0/scan_elements/in_accel_y_en"
#define MPU6500_ATTRIBUTE_NODE_ACCEL_Z_FIFO_ENABLE			"/sys/bus/iio/devices/iio:device0/scan_elements/in_accel_z_en"
#define MPU6500_ATTRIBUTE_NODE_ACCEL_MATRIX			"/sys/bus/iio/devices/iio:device0/accl_matrix"

struct sysfs_attrbs {
	  char chip_enable[256];
	  char power_state[256];
	  char gyro_enable[256];
	  char gyro_x_fifo_enable[256];
	  char gyro_y_fifo_enable[256];
	  char gyro_z_fifo_enable[256];

	  char accel_enable[256];
	  char accel_x_fifo_enable[256];
	  char accel_y_fifo_enable[256];
	  char accel_z_fifo_enable[256];
	  char buffer_length[256];
   } mpu;


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
		int mpu_fd;
		int i;
		struct pollfd         mFds[NUMS_DEVICES];//MPU only needs one.
		char ReadBuffer[8*2 + 8];
		char *rdata = ReadBuffer;
		short cachedGyroData[3];
		short cachedAccelData[3];
		long long timestamp;
		char buf[64];
		char dev_buf[64];
		int pipe_count = 0;

		char class_path[256];
		INIT_CMD_PIPE();
		if(sensor_get_class_path(class_path,argv[4]) < 0) {
	        printf("can't get the sensor class path\n");
	        goto err;
    	}
		sprintf(dev_buf,"%s%s","/dev/",argv[5]);
		sprintf(mpu.chip_enable, "%s%s", class_path, "/buffer/enable");
		sprintf(mpu.buffer_length, "%s%s", class_path, "/buffer/length");
		sprintf(mpu.power_state, "%s%s", class_path, "/power_state");
		sprintf(mpu.gyro_enable, "%s%s", class_path, "/gyro_enable");
		sprintf(mpu.gyro_x_fifo_enable, "%s%s", class_path,"/scan_elements/in_anglvel_x_en");
		sprintf(mpu.gyro_y_fifo_enable, "%s%s", class_path,"/scan_elements/in_anglvel_y_en");
		sprintf(mpu.gyro_z_fifo_enable, "%s%s", class_path,"/scan_elements/in_anglvel_z_en");
		sprintf(mpu.accel_enable, "%s%s", class_path, "/accl_enable");
		sprintf(mpu.accel_x_fifo_enable, "%s%s", class_path,"/scan_elements/in_accel_x_en");
		sprintf(mpu.accel_y_fifo_enable, "%s%s", class_path,"/scan_elements/in_accel_y_en");
		sprintf(mpu.accel_z_fifo_enable, "%s%s", class_path,"/scan_elements/in_accel_z_en");

		int res =0;
		res = write_sysfs_int(mpu.buffer_length,IIO_BUFFER_LENGTH);

		res = write_sysfs_int(mpu.gyro_x_fifo_enable,1);
		res = write_sysfs_int(mpu.gyro_y_fifo_enable,1);
		res = write_sysfs_int(mpu.gyro_z_fifo_enable,1);
		res = write_sysfs_int(mpu.gyro_enable,1);

		res = write_sysfs_int(mpu.accel_x_fifo_enable,1);
		res = write_sysfs_int(mpu.accel_y_fifo_enable,1);
		res = write_sysfs_int(mpu.accel_z_fifo_enable,1);
		res = write_sysfs_int(mpu.accel_enable,1);

		res = write_sysfs_int(mpu.power_state,1);
		res = write_sysfs_int(mpu.chip_enable,1);;
		if(res < 0)
			goto err;


		ssize_t nbyte = sizeof(ReadBuffer);
		{
			mpu_fd = open(dev_buf,O_RDONLY);
			if(mpu_fd < 0)
			{
				printf("Open mpu6500 device node[%s] failed \n",MPU6500_DEVICE_NODE);
				goto err;
			}
		}
		mFds[0].fd = mpu_fd;
		mFds[0].events  = POLLIN;
		mFds[0].revents = 0;
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
				//printf("aw=== rsize : %d\n",rsize);
				for(i = 0;i < 3; i++)
				{
					cachedGyroData[i] = *((short *) (rdata + i * 2));
        			cachedAccelData[i] = *((short *) (rdata + i * 2 + 6));
				}
				timestamp = *((long long *)(rdata + 2 * 8));
				sprintf(buf,"Gyro (%d,%d,%d),Accel (%d,%d,%d),timestamp : %lld",cachedGyroData[0],cachedGyroData[1],cachedGyroData[2],cachedAccelData[0],cachedAccelData[1],cachedAccelData[2],timestamp);
				++pipe_count;
				pipe_count %= 10;
				if(!pipe_count) {
					SEND_CMD_PIPE_OK_EX(buf);
				}
			}
		}
		close(mpu_fd);
		res = write_sysfs_int(mpu.chip_enable,0);
err:
    SEND_CMD_PIPE_FAIL();
    EXIT_CMD_PIPE();
	return 0;
}
