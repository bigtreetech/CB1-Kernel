/*
 * \file        script.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "script.h"
#include "dragonboard.h"

#define DATA_TYPE_SINGLE_WORD           1
#define DATA_TYPE_STRING                2
#define DATA_TYPE_MULTI_WORD            3
#define DATA_TYPE_GPIO                  4

struct script_head
{
    int mainkey_cnt;
    int version[3];
};

struct script_mainkey
{
    char name[32];
    int length;
    int offset;
};

struct script_subkey
{
    char name[32];
    int offset;
    int pattern;
};

static char *script_buf = NULL;
static int mainkey_cnt = 0;

int init_script(int shmid)
{
    script_buf = shmat(shmid, 0, 0);
    if (script_buf == (void *)-1) {
        db_error("script: attach the share memory segment failed(%s)\n",
                strerror(errno));
        return -1;
    }

    struct script_head *head = (struct script_head *)script_buf;

    mainkey_cnt = head->mainkey_cnt;
    db_debug("script: main key count #%d\n", mainkey_cnt);
    db_msg("script: V%d.%d.%d\n", head->version[0],
            head->version[1], head->version[2]);

    return 0;
}

void deinit_script(void)
{
    if (script_buf) {
        shmdt(script_buf);
        script_buf = NULL;
    }

    mainkey_cnt = 0;
}

int script_mainkey_cnt(void)
{
    return mainkey_cnt;
}

int script_mainkey_name(int idx, char *name)
{
    struct script_mainkey *mainkey;

    if (script_buf == NULL) {
        return -1;
    }

    mainkey = (struct script_mainkey *)(script_buf + sizeof(struct script_head) +
            idx * sizeof(struct script_mainkey));

    strncpy(name, mainkey->name, 32);

    return 0;
}

int script_fetch(char *main_name, char *sub_name, int value[], int count)
{
    int i, j;
    struct script_mainkey *mainkey;
    struct script_subkey *subkey;
    int pattern, wordcnt;

    /* check parameter */
    if (script_buf == NULL) {
        return -1;
    }

    if (main_name == NULL || sub_name == NULL) {
        return -1;
    }

    if (value == NULL) {
        return -1;
    }

    memset(value, 0, sizeof(int) * count);

    /* search main key */
    for (i = 0; i < mainkey_cnt; i++) {
        mainkey = (struct script_mainkey *)(script_buf +
                sizeof(struct script_head) + i * sizeof(struct script_mainkey));
        if (strncmp(mainkey->name, main_name, 31)) {
            continue;
        }

        /* search sub key */
        for (j = 0; j < mainkey->length; j++) {
            subkey = (struct script_subkey *)(script_buf +
                     (mainkey->offset << 2) +
                     (j * sizeof(struct script_subkey)));
            if (strncmp(subkey->name, sub_name, 31)) {
                continue;
            }

            pattern = (subkey->pattern >> 16) & 0xffff;
            wordcnt = subkey->pattern & 0xffff;

            switch (pattern) {
                case DATA_TYPE_SINGLE_WORD:
                    value[0] = *(int *)(script_buf + (subkey->offset << 2));
                    break;

                case DATA_TYPE_STRING:
                    if (wordcnt == 0)
                        break;

                    if (wordcnt > count)
                        wordcnt = count;

                    memcpy((char *)value, script_buf + (subkey->offset << 2),
                            wordcnt << 2);
                    break;

                case DATA_TYPE_GPIO:
                    db_debug("%s: L%d fix me\n", __func__, __LINE__);
                    break;

                default:
                    db_debug("%s: L%d fix me\n", __func__, __LINE__);
                    break;
            }

            return 0;
        }
    }

    return -1;
}
