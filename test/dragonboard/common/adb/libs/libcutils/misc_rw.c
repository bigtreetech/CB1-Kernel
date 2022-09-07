
#define LOG_TAG "misc_rw"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#include <utils/Log.h>

static const char *MISC_DEVICE = "/dev/by-name/misc";

#define LOGE(...) fprintf(stderr, "E:" __VA_ARGS__)

/* Bootloader Message
 */
struct bootloader_message {
    char command[32];
    char status[32];
    char recovery[1024];
};


// ------------------------------------
// for misc partitions on block devices
// ------------------------------------

static int get_mtd_partition_index_byname(const char* name)
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int index = 0;
    FILE* fp;
    fp = fopen("/proc/mtd","r");
    if(fp == NULL){
        LOGE("open /proc/mtd failed(%s)\n",strerror(errno));
        return -1;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        if( strstr(line,name) == NULL )
            continue;
        index = line[3] - '0';
        break;
    }
    free(line);
    return index;
}
static int is_mmc_or_mtd()
{
    int part_index = 0;
    int is_mtd = access("/dev/mtd0", F_OK); //mode填F_OK试试
    if(is_mtd == -1)
        return 0; //mmc
    part_index = get_mtd_partition_index_byname("misc");
    return part_index;//mtd
}

int get_bootloader_message_block(struct bootloader_message *out,
                                 const char* misc)
{
    char device[50];
    FILE* f;
    int id = is_mmc_or_mtd();
    if(id == 0){
        strcpy(device,misc);
    }
    else{
        sprintf(device,"/dev/mtd%d",id);
    }

    f = fopen(device, "rb");
    if (f == NULL) {
        LOGE("Can't open %s\n(%s)\n", device, strerror(errno));
        return -1;
    }
    struct bootloader_message temp;
    int count = fread(&temp, sizeof(temp), 1, f);
    if (count != 1) {
        LOGE("Failed reading %s\n(%s)\n", device, strerror(errno));
	fclose(f);
        return -1;
    }
    if (fclose(f) != 0) {
        LOGE("Failed closing %s\n(%s)\n", device, strerror(errno));
       return -1;
    }
    memcpy(out, &temp, sizeof(temp));
    return 0;
}

int set_bootloader_message_block(const struct bootloader_message *in,
                                 const char* misc)
{
    char device[50];
    FILE* f;
    int id = is_mmc_or_mtd();
    if(id == 0){
        strcpy(device,misc);
    }
    else{
        sprintf(device,"/dev/mtd%d",id);
        system("mtd erase misc");
    }

    f = fopen(device,"wb");
    if (f == NULL) {
        LOGE("Can't open %s\n(%s)\n", device, strerror(errno));
        return -1;
    }
    int count = fwrite(in, sizeof(*in), 1, f);
    if (count != 1) {
        LOGE("Failed writing %s\n(%s)\n", device, strerror(errno));
        fclose(f);
	return -1;
    }
    fsync(f);
    if (fclose(f) != 0) {
        LOGE("Failed closing %s\n(%s)\n", device, strerror(errno));
        return -1;
    }
    return 0;
}
/* force the next boot to recovery/efex */
int write_misc(char *reason){

	if(strcmp(reason,"efex") != 0){
		return 0;
	}

	struct bootloader_message boot, temp;

	memset(&boot, 0, sizeof(boot));
	if(!strcmp("recovery",reason)){
		reason = "boot-recovery";
	}
//	strcpy(boot.command, "boot-recovery");
	strcpy(boot.command,reason);
	if (set_bootloader_message_block(&boot, MISC_DEVICE) )
		return -1;

	//read for compare
	memset(&temp, 0, sizeof(temp));
	if (get_bootloader_message_block(&temp, MISC_DEVICE))
		return -1;

	if( memcmp(&boot, &temp, sizeof(boot)) )
		return -1;

	return 0;

}

/*
 * The recovery tool communicates with the main system through /cache files.
 *   /cache/recovery/command - INPUT - command line for tool, one arg per line
 *   /cache/recovery/log - OUTPUT - combined log file from recovery run(s)
 *   /cache/recovery/intent - OUTPUT - intent that was passed in
 *
 * The arguments which may be supplied in the recovery.command file:
 *   --send_intent=anystring - write the text out to recovery.intent
 *   --update_package=path - verify install an OTA package file
 *   --wipe_data - erase user data (and cache), then reboot
 *   --wipe_cache - wipe cache (but not user data), then reboot
 *   --set_encrypted_filesystem=on|off - enables / diasables encrypted fs
 */
/*
static const char *COMMAND_FILE = "/cache/recovery/command";

int go_update_package(const char *path){


	return 0;
}
*/
