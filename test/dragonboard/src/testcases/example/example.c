/*
 * \file        example.c
 * \brief       just an example.
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

/* include system header */
#include <string.h>

/* include "dragonboard_inc.h" */
#include "dragonboard_inc.h"

/* C entry.
 *
 * \param argc the number of arguments.
 * \param argv the arguments.
 *
 * DO NOT CHANGES THE NAME OF PARAMETERS, otherwise your program will get
 * a compile error if you are using INIT_CMD_PIPE macro.
 */
int main(int argc, char *argv[])
{
    char binary[16];
    int len = 0;

    /* init cmd pipe after local variable declaration */
    INIT_CMD_PIPE();
    len = strlen(argv[0]);
    if (len < 16)
    	strcpy(binary, argv[0]);
    else {
	db_msg("buffer overrun!\n");
	return -1;
    }
    db_msg("%s: here is an example.\n", binary);

    /* send OK to core if test OK, otherwise send FAIL
     * by using SEND_CMD_PIPE_FAIL().
     */
    SEND_CMD_PIPE_OK();

    return 0;
}
