/*
 * \file        nandrw.c
 * \brief
 *
 * \version     2.0.0
 * \date        2014年10月17日
 * \author      luoweijian<luoweijian@allwinnertech.com>
 *
 * Copyright (c) 2014 Allwinner Technology. All Rights Reserved.
 *
 */

#include <string.h>
#include <errno.h>
#include "dragonboard_inc.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/* just define an ioctl cmd for nand test*/
#define DRAGON_BOARD_TEST    _IO('V',55)

int main(int argc, char *argv[])
{
    char filename[256];
    int fd;
    int retval = -1;

	/* filename = /dev/nanda */
    if (argc == 2) {
        strncpy(filename, argv[1], 256);
    }
    else {
        db_error("Usage: nandrw FILE\n");
        retval = -1;
    }

    /* open file */
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        db_error("can't open %s(%s)\n", filename, strerror(errno));
        retval = -1;
    }

	/* if nand ok,return 0;otherwise,return -1 */
	retval = ioctl(fd, DRAGON_BOARD_TEST);
	if (retval < 0) {
        db_error("error in ioctl(%s)......\n", strerror(errno));
				return retval;
    }
    /* TEST OK */
    return retval;
}
