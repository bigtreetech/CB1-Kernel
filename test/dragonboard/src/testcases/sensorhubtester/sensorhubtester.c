/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>

#include  <dirent.h>
#include "dragonboard_inc.h"


//#include <eventnums.h>
#include <signal.h>
#include <inttypes.h>
#include <errno.h>

#define LOG_TAG "nanoapp_cmd"
#define SENSOR_RATE_ONCHANGE    0xFFFFFF01UL
#define SENSOR_RATE_ONESHOT     0xFFFFFF02UL
#define SENSOR_HZ(_hz)          ((uint32_t)((_hz) * 1024.0f))
#define MAX_INSTALL_CNT         8
#define MAX_DOWNLOAD_RETRIES    4

#define LOGE(fmt, ...) do { \
        printf(fmt "\n", ##__VA_ARGS__); \
    } while (0)


/* These define ranges of reserved events */
// local events are 16-bit always
#define EVT_NO_FIRST_USER_EVENT          0x00000100    //all events lower than this are reserved for the OS. all of them are nondiscardable necessarily!
#define EVT_NO_FIRST_SENSOR_EVENT        0x00000200    //sensor type SENSOR_TYPE_x produces events of type EVT_NO_FIRST_SENSOR_EVENT + SENSOR_TYPE_x for all Google-defined sensors
#define EVT_NO_SENSOR_CONFIG_EVENT       0x00000300    //event to configure sensors
#define EVT_APP_START                    0x00000400    //sent when an app can actually start
#define EVT_APP_TO_HOST                  0x00000401    //app data to host. Type is struct HostHubRawPacket
#define EVT_MARSHALLED_SENSOR_DATA       0x00000402    //marshalled event data. Type is MarshalledUserEventData
#define EVT_RESET_REASON                 0x00000403    //reset reason to host.
#define EVT_APP1_TO_APP2                 0x00000404    //app1 to app2.
#define EVT_APP2_TO_APP1                 0x00000405    //app2 to app1.
#define EVT_DEBUG_LOG                    0x00007F01    // send message payload to Linux kernel log
#define EVT_MASK                         0x0000FFFF

#define EVT_APP_SENSOR_POWER             0x000000EF    //data pointer is not a pointer, it is a bool encoded as (void*)
#define EVT_APP_SENSOR_FW_UPLD           0x000000EE
#define EVT_APP_SENSOR_SET_RATE          0x000000ED    //data pointer points to a "const struct SensorSetRateEvent"
#define EVT_APP_SENSOR_FLUSH             0x000000EC
#define EVT_APP_SENSOR_TRIGGER           0x000000EB
#define EVT_APP_SENSOR_CALIBRATE         0x000000EA
#define EVT_APP_SENSOR_CFG_DATA          0x000000E9
#define EVT_APP_SENSOR_SEND_ONE_DIR_EVT  0x000000E8
#define EVT_APP_SENSOR_MARSHALL          0x000000E7    // for external sensors that send events of "user type"
#define EVT_APP_SENSOR_SELF_TEST         0x000000E6


#define SENS_TYPE_INVALID         0
#define SENS_TYPE_ACCEL           1
#define SENS_TYPE_ANY_MOTION      2 //provided by ACCEL, nondiscardable edge trigger
#define SENS_TYPE_NO_MOTION       3 //provided by ACCEL, nondiscardable edge trigger
#define SENS_TYPE_SIG_MOTION      4 //provided by ACCEL, nondiscardable edge trigger
#define SENS_TYPE_FLAT            5
#define SENS_TYPE_GYRO            6
#define SENS_TYPE_GYRO_UNCAL      7
#define SENS_TYPE_MAG             8
#define SENS_TYPE_MAG_UNCAL       9
#define SENS_TYPE_BARO            10
#define SENS_TYPE_TEMP            11
#define SENS_TYPE_ALS             12
#define SENS_TYPE_PROX            13
#define SENS_TYPE_ORIENTATION     14
#define SENS_TYPE_HEARTRATE_ECG   15
#define SENS_TYPE_HEARTRATE_PPG   16
#define SENS_TYPE_GRAVITY         17
#define SENS_TYPE_LINEAR_ACCEL    18
#define SENS_TYPE_ROTATION_VECTOR 19
#define SENS_TYPE_GEO_MAG_ROT_VEC 20
#define SENS_TYPE_GAME_ROT_VECTOR 21
#define SENS_TYPE_STEP_COUNT      22
#define SENS_TYPE_STEP_DETECT     23
#define SENS_TYPE_GESTURE         24
#define SENS_TYPE_TILT            25
#define SENS_TYPE_DOUBLE_TWIST    26
#define SENS_TYPE_DOUBLE_TAP      27
#define SENS_TYPE_WIN_ORIENTATION 28
#define SENS_TYPE_HALL            29
#define SENS_TYPE_ACTIVITY        30
#define SENS_TYPE_VSYNC           31
#define SENS_TYPE_ACCEL_RAW       32
// Values 33-37 are reserved
#define SENS_TYPE_WRIST_TILT      39

// Activity recognition sensor types.
#define SENS_TYPE_ACTIVITY_IN_VEHICLE_START   40
#define SENS_TYPE_ACTIVITY_IN_VEHICLE_STOP    41
#define SENS_TYPE_ACTIVITY_ON_BICYCLE_START   42
#define SENS_TYPE_ACTIVITY_ON_BICYCLE_STOP    43
#define SENS_TYPE_ACTIVITY_WALKING_START      44
#define SENS_TYPE_ACTIVITY_WALKING_STOP       45
#define SENS_TYPE_ACTIVITY_RUNNING_START      46
#define SENS_TYPE_ACTIVITY_RUNNING_STOP       47
#define SENS_TYPE_ACTIVITY_STILL_START        48
#define SENS_TYPE_ACTIVITY_STILL_STOP         49
#define SENS_TYPE_ACTIVITY_TILTING            50

#define SENS_TYPE_DOUBLE_TOUCH    52

#define SENS_TYPE_FIRST_USER      64 // event type necessarily begins with UserSensorEventHdr
#define SENS_TYPE_LAST_USER       128

#define HOSTINTF_SENSOR_DATA_MAX    240

#define SENS_TYPE_TO_EVENT(_sensorType) (EVT_NO_FIRST_SENSOR_EVENT + (_sensorType))
#define ACCEL_RAW_KSCALE        (8.0f * 9.81f / 32768.0f)

enum ConfigCmds
{
    CONFIG_CMD_DISABLE      = 0,
    CONFIG_CMD_ENABLE       = 1,
    CONFIG_CMD_FLUSH        = 2,
    CONFIG_CMD_CFG_DATA     = 3,
    CONFIG_CMD_CALIBRATE    = 4,
};

struct ConfigCmd
{
    uint32_t evtType;
    uint64_t latency;
    uint32_t rate;
    uint8_t sensorType;
    uint8_t cmd;
    uint16_t flags;
} __attribute__((packed));

struct AppInfo
{
    uint32_t num;
    uint64_t id;
    uint32_t version;
    uint32_t size;
};

struct FirstSample
 {
	 uint8_t numSamples;
	 uint8_t numFlushes;
	 uint8_t highAccuracy : 1;
	 uint8_t biasPresent : 1;
	 uint8_t biasSample : 6;
	 uint8_t pad;
 };

 struct RawThreeAxisSample
 {
	 uint32_t deltaTime;
	 int16_t ix, iy, iz;
 } __attribute__((packed));

 struct ThreeAxisSample
 {
	 uint32_t deltaTime;
	 float x, y, z;
 } __attribute__((packed));

 struct OneAxisSample
 {
	 uint32_t deltaTime;
	 union
	 {
		 float fdata;
		 uint32_t idata;
	 };
 } __attribute__((packed));


struct nAxisEvent
{
	uint32_t evtType;
	union
	{
		struct
		{
			uint64_t referenceTime;
			union
			{
				struct FirstSample firstSample;
				struct OneAxisSample oneSamples[HOSTINTF_SENSOR_DATA_MAX/sizeof(struct OneAxisSample)];
				struct RawThreeAxisSample rawThreeSamples[HOSTINTF_SENSOR_DATA_MAX/sizeof(struct RawThreeAxisSample)];
				struct ThreeAxisSample threeSamples[HOSTINTF_SENSOR_DATA_MAX/sizeof(struct ThreeAxisSample)];
			};
		};
		uint8_t buffer[sizeof(uint64_t) + HOSTINTF_SENSOR_DATA_MAX];
	};
} __attribute__((packed));


typedef struct {
    union {
        float v[3];
        struct {
            float x;
            float y;
            float z;
        };
        struct {
            float azimuth;
            float pitch;
            float roll;
        };
    };
    int8_t status;
    uint8_t reserved[3];
} sensors_vec_t;

typedef struct sensors_event_t {
    /* must be sizeof(struct sensors_event_t) */
    int32_t version;

    /* sensor identifier */
    int32_t sensor;

    /* sensor type */
    int32_t type;

    /* reserved */
    int32_t reserved0;

    /* time is in nanosecond */
    int64_t timestamp;

	union {
            float           data[16];

            /* acceleration values are in meter per second per second (m/s^2) */
            sensors_vec_t   acceleration;

            /* magnetic vector values are in micro-Tesla (uT) */
            sensors_vec_t   magnetic;

            /* orientation values are in degrees */
            sensors_vec_t   orientation;

            /* gyroscope values are in rad/s */
            sensors_vec_t   gyro;
    };

    /* Reserved flags for internal use. Set to zero. */
    uint32_t flags;

    uint32_t reserved1[3];
} sensors_event_t;


static int setType(struct ConfigCmd *cmd, char *sensor)
{
    if (strcmp(sensor, "accel") == 0) {
        cmd->sensorType = SENS_TYPE_ACCEL;
    } else if (strcmp(sensor, "gyro") == 0) {
        cmd->sensorType = SENS_TYPE_GYRO;
    } else if (strcmp(sensor, "mag") == 0) {
        cmd->sensorType = SENS_TYPE_MAG;
    } else if (strcmp(sensor, "uncal_gyro") == 0) {
        cmd->sensorType = SENS_TYPE_GYRO;
    } else if (strcmp(sensor, "uncal_mag") == 0) {
        cmd->sensorType = SENS_TYPE_MAG;
    } else if (strcmp(sensor, "als") == 0) {
        cmd->sensorType = SENS_TYPE_ALS;
    } else if (strcmp(sensor, "prox") == 0) {
        cmd->sensorType = SENS_TYPE_PROX;
    } else if (strcmp(sensor, "baro") == 0) {
        cmd->sensorType = SENS_TYPE_BARO;
    } else if (strcmp(sensor, "temp") == 0) {
        cmd->sensorType = SENS_TYPE_TEMP;
    } else if (strcmp(sensor, "orien") == 0) {
        cmd->sensorType = SENS_TYPE_ORIENTATION;
    } else if (strcmp(sensor, "gravity") == 0) {
        cmd->sensorType = SENS_TYPE_GRAVITY;
    } else if (strcmp(sensor, "geomag") == 0) {
        cmd->sensorType = SENS_TYPE_GEO_MAG_ROT_VEC;
    } else if (strcmp(sensor, "linear_acc") == 0) {
        cmd->sensorType = SENS_TYPE_LINEAR_ACCEL;
    } else if (strcmp(sensor, "rotation") == 0) {
        cmd->sensorType = SENS_TYPE_ROTATION_VECTOR;
    } else if (strcmp(sensor, "game") == 0) {
        cmd->sensorType = SENS_TYPE_GAME_ROT_VECTOR;
    } else if (strcmp(sensor, "win_orien") == 0) {
        cmd->sensorType = SENS_TYPE_WIN_ORIENTATION;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "tilt") == 0) {
        cmd->sensorType = SENS_TYPE_TILT;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "step_det") == 0) {
        cmd->sensorType = SENS_TYPE_STEP_DETECT;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "step_cnt") == 0) {
        cmd->sensorType = SENS_TYPE_STEP_COUNT;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "double_tap") == 0) {
        cmd->sensorType = SENS_TYPE_DOUBLE_TAP;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "flat") == 0) {
        cmd->sensorType = SENS_TYPE_FLAT;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "anymo") == 0) {
        cmd->sensorType = SENS_TYPE_ANY_MOTION;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "nomo") == 0) {
        cmd->sensorType = SENS_TYPE_NO_MOTION;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "sigmo") == 0) {
        cmd->sensorType = SENS_TYPE_SIG_MOTION;
        cmd->rate = SENSOR_RATE_ONESHOT;
    } else if (strcmp(sensor, "gesture") == 0) {
        cmd->sensorType = SENS_TYPE_GESTURE;
        cmd->rate = SENSOR_RATE_ONESHOT;
    } else if (strcmp(sensor, "hall") == 0) {
        cmd->sensorType = SENS_TYPE_HALL;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "vsync") == 0) {
        cmd->sensorType = SENS_TYPE_VSYNC;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "activity") == 0) {
        cmd->sensorType = SENS_TYPE_ACTIVITY;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else if (strcmp(sensor, "twist") == 0) {
        cmd->sensorType = SENS_TYPE_DOUBLE_TWIST;
        cmd->rate = SENSOR_RATE_ONCHANGE;
    } else {
        return 1;
    }

    return 0;
}

bool drain = false;
bool stop = false;
int nread, buf_size = 255;
struct AppInfo apps[32];
uint8_t appCount;
char appsToInstall[MAX_INSTALL_CNT][32];
static FILE *cmd_pipe;

void sig_handle(__attribute__((unused)) int sig)
{
    assert(sig == SIGINT);
    printf("Terminating...\n");
    stop = true;
}

FILE *openFile(const char *fname, const char *mode)
{
    FILE *f = fopen(fname, mode);
    if (f == NULL) {
        LOGE("Failed to open %s: err=%d [%s]", fname, errno, strerror(errno));
    }
    return f;
}

void parseInstalledAppInfo()
{
    FILE *fp;
    char *line = NULL;
    size_t len;
    ssize_t numRead;

    appCount = 0;

    fp = openFile("/sys/class/nanohub/nanohub/app_info", "r");
    if (!fp)
        return;

    while ((numRead = getline(&line, &len, fp)) != -1) {
        struct AppInfo *currApp = &apps[appCount++];
        sscanf(line, "app: %d id: %" PRIx64 " ver: %" PRIx32 " size: %" PRIx32 "\n", &currApp->num, &currApp->id, &currApp->version, &currApp->size);
    }

    fclose(fp);

    if (line)
        free(line);
}

struct AppInfo *findApp(uint64_t appId)
{
    uint8_t i;

    for (i = 0; i < appCount; i++) {
        if (apps[i].id == appId) {
            return &apps[i];
        }
    }

    return NULL;
}

int parseConfigAppInfo()
{
    FILE *fp;
    char *line = NULL;
    size_t len;
    ssize_t numRead;
    int installCnt;

    fp = openFile("/vendor/firmware/napp_list.cfg", "r");
    if (!fp)
        return -1;

    parseInstalledAppInfo();

    installCnt = 0;
    while (((numRead = getline(&line, &len, fp)) != -1) && (installCnt < MAX_INSTALL_CNT)) {
        uint64_t appId;
        uint32_t appVersion;
        struct AppInfo* installedApp;

        sscanf(line, "%32s %" PRIx64 " %" PRIx32 "\n", appsToInstall[installCnt], &appId, &appVersion);

        installedApp = findApp(appId);
        if (!installedApp || (installedApp->version < appVersion)) {
            installCnt++;
        }
    }

    fclose(fp);

    if (line)
        free(line);

    return installCnt;
}

bool fileWriteData(const char *fname, const void *data, size_t size)
{
    int fd;
    bool result;

    fd = open(fname, O_WRONLY);
    if (fd < 0) {
        LOGE("Failed to open %s: err=%d [%s]", fname, errno, strerror(errno));
        return false;
    }

    result = true;
    if ((size_t)write(fd, data, size) != size) {
        LOGE("Failed to write to %s; err=%d [%s]", fname, errno, strerror(errno));
        result = false;
    }
    close(fd);

    return result;
}

void downloadNanohub()
{
    char c = '1';

    printf("Updating nanohub OS [if required]...");
    fflush(stdout);
    if (fileWriteData("/sys/class/nanohub/nanohub/download_bl", &c, sizeof(c)))
        printf("done\n");
}

void downloadApps(int updateCnt)
{
    int i;

    for (i = 0; i < updateCnt; i++) {
        printf("Downloading \"%s.napp\"...", appsToInstall[i]);
        fflush(stdout);
        if (fileWriteData("/sys/class/nanohub/nanohub/download_app", appsToInstall[i], strlen(appsToInstall[i])))
            printf("done\n");
    }
}

void eraseSharedArea()
{
    char c = '1';

    printf("Erasing entire nanohub shared area...");
    fflush(stdout);
    if (fileWriteData("/sys/class/nanohub/nanohub/erase_shared", &c, sizeof(c)))
        printf("done\n");
}

void resetHub()
{
    char c = '1';

    printf("Resetting nanohub...");
    fflush(stdout);
    if (fileWriteData("/sys/class/nanohub/nanohub/reset", &c, sizeof(c)))
        printf("done\n");
}


int processBuf(uint8_t *buf, uint32_t len)
{
	struct nAxisEvent *data = (struct nAxisEvent *)buf;
	uint32_t type;
	int i, numSamples;
	ssize_t ret = 0;
	static int cnt_raw = 0;
	static int cnt = 0;

	if (len >= sizeof(data->evtType)) {
		ret = sizeof(data->evtType);
        switch (data->evtType) {
		case SENS_TYPE_TO_EVENT(SENS_TYPE_ACCEL):
			type = SENS_TYPE_ACCEL;
			break;
		case SENS_TYPE_TO_EVENT(SENS_TYPE_ACCEL_RAW):
            type = SENS_TYPE_ACCEL_RAW;
            break;
        case SENS_TYPE_TO_EVENT(SENS_TYPE_GYRO):
            type = SENS_TYPE_GYRO;
            break;
        case SENS_TYPE_TO_EVENT(SENS_TYPE_MAG):
            type = SENS_TYPE_MAG;
            break;
        case SENS_TYPE_TO_EVENT(SENS_TYPE_ALS):
            type = SENS_TYPE_ALS;
            break;
        case SENS_TYPE_TO_EVENT(SENS_TYPE_PROX):
            type = SENS_TYPE_PROX;
            break;
		default:
            printf("unknown evtType: 0x%08x\n", data->evtType);
            return -1;
		}
	} else {
        printf("too little data: len=%u\n", len);
        return -1;
    }

	 if (len >= sizeof(data->evtType) + sizeof(data->referenceTime) + sizeof(data->firstSample)) {
		ret += sizeof(data->referenceTime);
		numSamples = data->firstSample.numSamples;
		 for (i=0; i<numSamples; i++) {
		 	if (data->firstSample.biasPresent && data->firstSample.biasSample == i)
				continue;
			if(SENS_TYPE_PROX == type || SENS_TYPE_ALS == type ) {
				if (ret + sizeof(data->oneSamples[i]) > len) {
                    printf("sensor (rawThree): ret=%zd, numSamples=%d, i=%d\n",  ret, numSamples, i);
                    return -1;
                }
				// send als or prox data
				//printf("sensor type %d,value %f\n",type,data->oneSamples[i].fdata);
				ret += sizeof(data->oneSamples[i]);
			} else if (SENS_TYPE_ACCEL_RAW == type) {
				if (ret + sizeof(data->rawThreeSamples[i]) > len) {
                    printf("sensor (rawThree): ret=%zd, numSamples=%d, i=%d\n", ret, numSamples, i);
                    return -1;
                }
				// send accel raw data
				if (++cnt_raw % 30 == 0) {
				char exdata[100];
					sprintf(exdata,"x:%.3f,y:%.3f,z:%.3f",data->rawThreeSamples[i].ix * ACCEL_RAW_KSCALE,
						data->rawThreeSamples[i].iy * ACCEL_RAW_KSCALE, data->rawThreeSamples[i].iz * ACCEL_RAW_KSCALE);
					fprintf(cmd_pipe, "%d 0 %s\n",100+type, exdata);
					//printf("accel raw %s\n",exdata);
					cnt_raw = 0;
				}
				ret += sizeof(data->rawThreeSamples[i]);
			} else {
				if (ret + sizeof(data->threeSamples[i]) > len) {
                    printf("sensor(three): ret=%zd, numSamples=%d, i=%d\n",  ret, numSamples, i);
                    return -1;
                }
				//send accel gyro mag data

				if (++cnt % 30 == 0) {
					char exdata[100];
					sprintf(exdata,"x:%.3f,y:%.3f,z:%.3f",data->threeSamples[i].x,data->threeSamples[i].y,
						data->threeSamples[i].z);
					fprintf(cmd_pipe, "%d 0 %s\n",100+type, exdata);
					cnt = 0;
				//	printf("sensor type %d,%s",type,exdata);
				}
				ret += sizeof(data->threeSamples[i]);
			}
		 }
	 }
	 return ret;
}

int main(int argc, char *argv[])
{
    struct ConfigCmd mConfigCmd;
	struct pollfd mPollFds[1];
    int fd;
    int i,mNumPollFds;
    int ret;

    cmd_pipe = fopen(CMD_PIPE_NAME, "w");
    setlinebuf(cmd_pipe);
    if (argc < 3 && (argc < 2 || strcmp(argv[1], "download") != 0)) {
        printf("usage: %s <action> <sensor> <data> -d\n", argv[0]);
        printf("       action: config|calibrate|flush|download\n");
        printf("       sensor: accel|(uncal_)gyro|(uncal_)mag|als|prox|baro|temp|orien\n");
        printf("               gravity|geomag|linear_acc|rotation|game\n");
        printf("               win_orien|tilt|step_det|step_cnt|double_tap\n");
        printf("               flat|anymo|nomo|sigmo|gesture|hall|vsync\n");
        printf("               activity|twist\n");
        printf("       data: config: <true|false> <rate in Hz> <latency in u-sec>\n");
        printf("             calibrate: [N.A.]\n");
        printf("             flush: [N.A.]\n");
        printf("       -d: if specified, %s will keep draining /dev/nanohub until cancelled.\n", argv[0]);

        return 1;
    }

    if (strcmp(argv[1], "config") == 0) {
        if (argc != 6 && argc != 7) {
            printf("Wrong arg number\n");
            return 1;
        }
        if (argc == 7) {
            if(strcmp(argv[6], "-d") == 0) {
                drain = true;
            } else {
                printf("Last arg unsupported, ignored.\n");
            }
        }
        if (strcmp(argv[3], "true") == 0)
            mConfigCmd.cmd = CONFIG_CMD_ENABLE;
        else if (strcmp(argv[3], "false") == 0) {
            mConfigCmd.cmd = CONFIG_CMD_DISABLE;
        } else {
            printf("Unsupported data: %s For action: %s\n", argv[3], argv[1]);
            return 1;
        }
        mConfigCmd.evtType = EVT_NO_SENSOR_CONFIG_EVENT;
        mConfigCmd.rate = SENSOR_HZ((float)atoi(argv[4]));
        mConfigCmd.latency = atoi(argv[5]) * 1000ull;
        if (setType(&mConfigCmd, argv[2])) {
            printf("Unsupported sensor: %s For action: %s\n", argv[2], argv[1]);
            return 1;
        }
    } else if (strcmp(argv[1], "calibrate") == 0) {
        if (argc != 3) {
            printf("Wrong arg number\n");
            return 1;
        }
        mConfigCmd.evtType = EVT_NO_SENSOR_CONFIG_EVENT;
        mConfigCmd.rate = 0;
        mConfigCmd.latency = 0;
        mConfigCmd.cmd = CONFIG_CMD_CALIBRATE;
        if (setType(&mConfigCmd, argv[2])) {
            printf("Unsupported sensor: %s For action: %s\n", argv[2], argv[1]);
            return 1;
        }
    } else if (strcmp(argv[1], "flush") == 0) {
        if (argc != 3) {
            printf("Wrong arg number\n");
            return 1;
        }
        mConfigCmd.evtType = EVT_NO_SENSOR_CONFIG_EVENT;
        mConfigCmd.rate = 0;
        mConfigCmd.latency = 0;
        mConfigCmd.cmd = CONFIG_CMD_FLUSH;
        if (setType(&mConfigCmd, argv[2])) {
            printf("Unsupported sensor: %s For action: %s\n", argv[2], argv[1]);
            return 1;
        }
    } else if (strcmp(argv[1], "download") == 0) {
        if (argc != 2) {
            printf("Wrong arg number\n");
            return 1;
        }
        downloadNanohub();
        for (i = 0; i < MAX_DOWNLOAD_RETRIES; i++) {
            int updateCnt = parseConfigAppInfo();
            if (updateCnt > 0) {
                if (i == MAX_DOWNLOAD_RETRIES - 1) {
                    LOGE("Download failed after %d retries; erasing all apps "
                         "before final attempt", i);
                    eraseSharedArea();
                }
                downloadApps(updateCnt);
                resetHub();
            } else if (!updateCnt){
                return 0;
            }
        }

        if (parseConfigAppInfo() != 0) {
            LOGE("Failed to download all apps!");
        }
        return 1;
    } else {
        printf("Unsupported action: %s\n", argv[1]);
        return 1;
    }

    while (!fileWriteData("/dev/nanohub", &mConfigCmd, sizeof(mConfigCmd)))
        continue;

    if (drain) {
        signal(SIGINT, sig_handle);

        fd = open("/dev/nanohub", O_RDONLY);
		mPollFds[0].fd = fd;
		mPollFds[0].events = POLLIN;
		mPollFds[0].revents = 0;
		mNumPollFds = 1;
		while(!stop) {
		do {
            ret = poll(mPollFds, mNumPollFds, -1);
        } while (ret < 0 && errno == EINTR);
			uint8_t recv[256];
			uint32_t offset;
            ssize_t len = read(fd, recv, sizeof(recv));
			if (len >= 0) {
                for (offset = 0; offset < len;) {
                    ret = processBuf(recv + offset, len - offset);
                    if (ret >0)
                        offset += ret;
                    else
                        break;
                }
            } else {
                printf("read -1: errno=%d\n", errno);
            }
        }
        close(fd);
    }
    return 0;
}
