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

#include "script_parser.h"
#include "dragonboard.h"

#define ITEM_NAME_MAX_LEN               32

#define KEY_MAX_LEN                     32
#define VALUE_MAX_LEN                   128

#define ITEM_MAX_COUNT                  128

#define LINE_MAX_LEN                    512

#define LINE_ERROR                      -1
#define LINE_COMMENT                    0
#define LINE_NULL                       1
#define LINE_MAINKEY                    2
#define LINE_SUBKEY                     3

#define DATA_TYPE_SINGLE_WORD           1
#define DATA_TYPE_STRING                2
#define DATA_TYPE_MULTI_WORD            3
#define DATA_TYPE_GPIO                  4

struct script_head
{
    int mainkey_cnt;
    int version[3];
};

struct script_item
{
    char name[32];
    int length;
    int offset;
};

/*
 * get value data type.
 */
static int __get_str2int(char *buf, int value[])
{
    char *src;
    char ch;
    unsigned int temp;
    int sign;
    int i;
    int idx;
    char str[128];

    src = buf;

    if (strncasecmp(src, "port:p", 6) == 0) {
        /* handle gpio */
        src += 6;
        if (strncasecmp(src, "ower", 4) == 0) {
            value[0] = 0xffff;
            src += 4;
        }
        else {
            ch = *src++;
            if (islower(ch))
                value[0] = ch - 'a' + 1;
            else if (isupper(ch))
                value[0] = ch - 'A' + 1;
            else
                return -1;
        }

        temp = 0;
        ch = *src++;
        while (ch != '<') {
            if (isdigit(ch)) {
                temp = temp * 10 + (ch - '0');
                ch = *src++;
            }
            else if (ch == '\0') {
                src--;
                break;
            }
            else {
                return -1;
            }
        }
        value[1] = temp;

        idx = 2;
        ch = *src++;
        while (ch != '\0') {
            i = 0;
            memset(str, 0, sizeof(str));
            while (ch != '>') {
                if (isupper(ch))
                    ch = tolower(ch);
                str[i++] = ch;
                ch = *src++;
            }

            if (strcmp(str, "default") == 0 ||
                strcmp(str, "none") == 0 ||
                strcmp(str, "null") == 0 ||
                strcmp(str, "-1") == 0) {
                value[idx] = -1;
            }
            else {
                i = 0;
                ch = str[i++];
                temp = 0;
                if (ch == '-') {
                    sign = -1;
                    ch = str[i++];
                }
                else {
                    sign = 1;
                }

                while (ch != '\0') {
                    if (isdigit(ch))
                        temp = temp * 10 + (ch - '0');
                    else
                        return -1;

                    ch = str[i++];
                }

                value[idx] = temp * sign;
            }

            idx++;
            ch = *src++;
            if (ch == '<') {
                ch = *src++;
            }
            else if (ch == '\0') {
                ;
            }
            else {
                return -1;
            }
        }

        switch (idx) {
            case 3:
                value[3] = -1;
            case 4:
                value[4] = -1;
            case 5:
                value[5] = -1;
            case 6:
                break;
            default:
                return -1;
        }

        return DATA_TYPE_GPIO;
    }
    else if (strncasecmp(src, "string:", 7) == 0) {
        src += 7;
        idx = 0;
        while (src[idx] != '\0') {
            idx++;
            if (idx > 127)
                break;
        }

        if (idx & 0x03)
            idx = (idx & (~0x03)) + 4;

        value[0] = idx >> 2;
        value[1] = 1;

        return DATA_TYPE_STRING;
    }
    else if (src[0] == '"') {
        src += 1;
        idx = 0;
        while (src[idx] != '"') {
            idx++;
            if (idx > 127)
                break;
        }

        src[idx] = '\0';
        if (idx & 0x03)
            idx = (idx & (~0x03)) + 4;

        value[0] = idx >> 2;
        value[1] = 2;

        return DATA_TYPE_STRING;
    }
    else if (isdigit(src[0]) && (src[1] == 'x' || src[2] == 'X')) {
        temp = 0;
        ch = *src++;
        while (ch != '\0') {
            if (isdigit(ch)) {
                temp = temp * 16 + (ch - '0');
                ch = *src++;
            }
            else if (isupper(ch)) {
                temp = temp * 16 + (ch - 'A' + 10);
                ch = *src++;
            }
            else if (islower(ch)) {
                temp = temp * 16 + (ch - 'a' + 10);
                ch = *src++;
            }
            else {
                break;
            }
        }
        value[0] = temp;

        return DATA_TYPE_SINGLE_WORD;
    }
    else if (isdigit(src[0]) ||
            (isdigit(src[1]) && src[0] == '-')) {
        if (src[0] == '-') {
            sign = -1;
            ch = *src++;
        }
        else {
            sign = 1;
        }

        temp = 0;
        ch = *src++;
        while (ch != '\0') {
            if (isdigit(ch)) {
                temp = temp * 10 + (ch - '0');
                ch = *src++;
            }
            else {
                break;
            }
        }
        value[0] = temp * sign;

        return DATA_TYPE_SINGLE_WORD;
    }
    else {
        idx = 0;
        while (src[idx] != '\0') {
            idx++;
            if (idx > 127)
                break;
        }

        if (idx & 0x03)
            idx = (idx & (~0x03)) + 4;

        value[0] = idx >> 2;

        return DATA_TYPE_STRING;
    }
}

/*
 * get key[32] and value[128], end with '\0'.
 * \retval -1 empty line.
 * \retval 0 OK
 */
static int __get_key_value(char *buf, char *key, char *value)
{
    char *src;
    int key_idx;
    int value_idx;

    /* check line */
    src = buf;
    key_idx = value_idx = 0;
    while (1) {
        if (*src == ' ' || *src == '\t') {
            src++;
        }
        else if (*src == 0x0a || *src == 0x0d) {
            key[key_idx] = '\0';
            value[value_idx] = '\0';
            return -1;
        }
        else {
            break;
        }
    }

    /* get key */
    while (*src != '=') {
        key[key_idx++] = *src++;
        if (key_idx >= 31) {
            key[key_idx] = '\0';
            break;
        }
    }

    for (key_idx--; key_idx > 0; key_idx--) {
        if (key[key_idx] == ' ' || key[key_idx] == '\t') {
            key[key_idx] = '\0';
        }
        else {
            key[key_idx + 1] = '\0';
            break;
        }
    }

    for (; *src != '='; src++);

    /* check line */
    src++;
    while (1) {
        if (*src == ' ' || *src == '\t') {
            src++;
        }
        else if (*src == 0x0a || *src == 0x0d) {
            value[value_idx] = '\0';
            return 0;
        }
        else {
            break;
        }
    }

    /* get value */
    while (*src != 0x0a && *src != 0x0d) {
        value[value_idx++] = *src++;
        if (value_idx >= 127) {
            value[value_idx] = '\0';
            break;
        }
    }

    for (value_idx--; value_idx > 0; value_idx--) {
        if (value[value_idx] == ' ' || value[value_idx] == '\t') {
            value[value_idx] = '\0';
        }
        else {
            value[value_idx + 1] = '\0';
            break;
        }
    }

    return 0;
}

static int __fill_mainkey(char *buf, struct script_item *item)
{
    char *src;
    char ch;
    int i;

    src = buf + 1;
    for (ch = *src++, i = 0; ch != ']'; i++, ch = *src++) {
        item->name[i] = ch;
        if (i + 1 >= ITEM_NAME_MAX_LEN) {
            item->name[i] = '\0';
            break;
        }
    }

    if (item->name[0] == '\0')
        return -1;

    return 0;
}

static int __getline(char *buf, int len, int *flag)
{
    char *src;
    char ch;
    char prev_ch;
    int line_len;

    /* get line flag */
    src = buf;
    ch = *src++;
    switch (ch) {
        case ';':
            *flag = LINE_COMMENT;
            break;

        case 0x0a:
        case 0x0d:
            *flag = LINE_NULL;
            break;

        case '[':
            *flag = LINE_MAINKEY;
            break;

        default:
            *flag = LINE_SUBKEY;
            break;
    }

    /* get line length */
    if (*flag == LINE_NULL) {
        ch = *src++;
        if (ch == 0x0a)
            return 2;
        else
            return 1;
    }

    ch = *src++;
    line_len = 1;
    while (line_len < len) {
        if (ch == 0x0a || ch == 0x0d)
            break;

        ch = *src++;
        line_len++;
        if (line_len >= LINE_MAX_LEN) {
            *flag = LINE_ERROR;
            return 0;
        }
    }
    line_len++;

    prev_ch = ch;
    ch = *src++;
    if (prev_ch == 0x0d) {
        if (ch == 0x0a)
            line_len++;
    }

    return line_len;
}

static int __parse_script(char *buf, int len)
{
    char *src;
    int rest_len, line_num;
    struct script_item *item_table = NULL;
    struct script_head head;
    int mainkey_idx, subkey_idx;
    int new_mainkey;
    char key[KEY_MAX_LEN];
    char value[VALUE_MAX_LEN];
    char *key_data = NULL, *key_addr;
    int *value_data = NULL, *value_addr;
    int value_idx;
    int fmt_value[8];
    int i;
    int ret;
    int shmid;
    char *script_buf, *pos;

    db_debug("the length of script is %d\n", len);

    /* allocate memory for key and value */
    item_table = malloc(sizeof(struct script_item) * ITEM_MAX_COUNT);
    if (item_table == NULL) {
        db_debug("allocate memory for main key failed(%s)\n", strerror(errno));
        ret = -1;
        goto out;
    }
    memset(item_table, 0, sizeof(struct script_item) * ITEM_MAX_COUNT);

    key_data = malloc(512 * 1024);
    if (key_data == NULL) {
        db_debug("allocate memory for key data failed(%s)\n", strerror(errno));
        ret = -1;
        goto out;
    }
    memset(key_data, 0, 512 * 1024);
    key_addr = key_data;

    value_data = malloc(512 * 1024);
    if (value_data == NULL) {
        db_debug("allocate memory for value data failed(%s)\n",
                strerror(errno));
        ret = -1;
        goto out;
    }
    memset(value_data, 0, 512 * 1024);
    value_addr = value_data;

    /* parse script main loop */
    src = buf;
    rest_len = len;
    mainkey_idx = subkey_idx = value_idx = 0;
    new_mainkey = 0;
    line_num = 0;
    while (rest_len) {
        int line_len;
        int flag;

        /* get current line */
        line_len = __getline(src, rest_len, &flag);
        rest_len -= line_len;
        line_num++;
        switch (flag) {
            case LINE_COMMENT:
            case LINE_NULL:
                break;

            case LINE_MAINKEY:
                /* get main key */
                if (__fill_mainkey(src, &item_table[mainkey_idx])) {
                    ret = -1;
                    goto out;
                }

                if (new_mainkey) {
                    item_table[mainkey_idx].offset =
                        item_table[mainkey_idx-1].offset +
                        item_table[mainkey_idx-1].length * 10;
                }
                else {
                    /* first main key */
                    new_mainkey = 1;
                    item_table[mainkey_idx].offset = 0;
                }

                mainkey_idx++;
                break;

            case LINE_SUBKEY:
                /* get key[32] and value[128] */
                memset(key, 0, KEY_MAX_LEN);
                memset(value, 0, VALUE_MAX_LEN);
                ret = __get_key_value(src, key, value);
                if (ret == -1)
                    continue;

                /* stores key[32], stores value's offset in the next 4 bytes,
                 * stores value's length[31;16] and type[0:15] in the next
                 * next 4 bytes. there are total 40 bytes for one key data.
                 *
                 * the unit of value data length is int
                 */
                strcpy(key_addr, key);
                key_addr += KEY_MAX_LEN;

                /* get value data type */
                memset(fmt_value, 0, sizeof(int) * 8);
                ret = __get_str2int(value, fmt_value);
                switch (ret) {
                    case DATA_TYPE_SINGLE_WORD:
                        *value_addr = fmt_value[0];
                        *(unsigned int *)key_addr = value_idx;
                        key_addr += 4;
                        *(unsigned int *)key_addr = (1 << 0) |
                            (DATA_TYPE_SINGLE_WORD << 16);
                        value_addr++;
                        value_idx++;
                        break;

                    case DATA_TYPE_STRING:
                        if (fmt_value[0]) {
                            if (fmt_value[1] == 1) {
                                strncpy((char *)value_addr, value +
                                        sizeof("string:") - 1, fmt_value[0] << 2);
                            }
                            else if (fmt_value[1] == 2) {
                                strncpy((char *)value_addr, value + 1,
                                        fmt_value[0] << 2);
                            }
                            else{
                                strncpy((char *)value_addr, value,
                                        fmt_value[0] << 2);
                            }
                        }

                        *(unsigned int *)key_addr = value_idx;
                        key_addr += 4;
                        *(unsigned int *)key_addr = (fmt_value[0] << 0) |
                            (DATA_TYPE_STRING << 16);
                        value_addr += fmt_value[0];
                        value_idx += fmt_value[0];
                        break;

                    case DATA_TYPE_GPIO:
                        for (i = 0; i < 6; i++)
                            *value_addr++ = fmt_value[i];
                        *(unsigned int *)key_addr = value_idx;
                        key_addr += 4;
                        *(unsigned int *)key_addr = (6 << 0) |
                            (DATA_TYPE_GPIO << 16);
                        value_idx += 6;
                        break;

                    default:
                        db_debug("%s: L%d: fix me\n", __func__, __LINE__);
                }

                subkey_idx++;
                key_addr += 4;
                item_table[mainkey_idx - 1].length++;
                break;

            default:
                db_debug("script format error at line %d\n", line_num);
                ret = -1;
                goto out;
        }

        src += line_len;
    }

    if (mainkey_idx <= 0) {
        db_debug("mainkey_idx = %d\n", mainkey_idx);
        ret = -1;
        goto out;
    }

    /* recalc first subkey offset */
    for (i = 0; i < mainkey_idx; i++) {
        item_table[i].offset += (sizeof(struct script_head) >> 2) +
            ((sizeof(struct script_item) * mainkey_idx) >> 2);
    }

    /* recalc every subkey offset */
    key_addr = key_data;
    i = 0;
    while (i < subkey_idx * 10 * sizeof(int)) {
        key_addr += ITEM_NAME_MAX_LEN;
        *(unsigned int *)key_addr += (sizeof(struct script_head) >> 2) +
            ((sizeof(struct script_item) * mainkey_idx) >> 2) +
            (subkey_idx * 10);
        i += 10 * sizeof(int); /* the sizeof each subkey data */
        key_addr += 8;
    }

    /* init script head */
    head.mainkey_cnt = mainkey_idx;
    head.version[0] = SCRIPT_VERSION0;
    head.version[1] = SCRIPT_VERSION1;
    head.version[2] = SCRIPT_VERSION2;

    /* allocate share memory segment for script buffer */
    i = sizeof(struct script_head) + sizeof(struct script_item) *
        mainkey_idx + 10 * sizeof(int) * subkey_idx + sizeof(int) * value_idx;
    shmid = shmget(IPC_PRIVATE, i, IPC_CREAT | 0666);
    if (shmid == -1) {
        db_debug("allocate share memory segment for script buffer "
                "failed(%s)\n", strerror(errno));
        ret = -1;
        goto out;
    }
    db_debug("script shmid = %d\n", shmid);

    script_buf = pos = shmat(shmid, 0, 0);
    if (script_buf == (void *)-1) {
        db_debug("attach the share memory segment failed(%s)\n",
                strerror(errno));
        shmctl(shmid, IPC_RMID, 0);
        ret = -1;
        goto out;
    }

    /* stores script buffer */
    memcpy(pos, &head, sizeof(struct script_head));
    pos += sizeof(struct script_head);
    memcpy(pos, item_table, sizeof(struct script_item) * mainkey_idx);
    pos += sizeof(struct script_item) * mainkey_idx;
    memcpy(pos, key_data, 10 * sizeof(int) * subkey_idx);
    pos += 10 * sizeof(int) * subkey_idx;
    memcpy(pos, value_data, sizeof(int) * value_idx);

    shmdt(script_buf);
    ret = shmid;

out:
    /* free memory */
    if (item_table)
        free(item_table);
    if (key_data)
        free(key_data);
    if (value_data)
        free(value_data);

    return ret;
}

int parse_script(const char *name)
{
    FILE *fscript;
    int len;
    char *buf;
    int read_len;
    int shmid;

    if (name == NULL) {
        db_error("script: invalid file name(null)\n");
        return -1;
    }

    /* read script data */
    fscript = fopen(name, "rb");
    if (fscript == NULL) {
        db_error("script: can't open %s(%s)\n", name, strerror(errno));
        return -1;
    }

    fseek(fscript, 0, SEEK_END);
    len = ftell(fscript);
    fseek(fscript, 0, SEEK_SET);

    buf = malloc(len);
    if (buf == NULL) {
        db_error("script: allocate memory for script data failed(%s)\n",
                strerror(errno));
	fclose(fscript);
        return -1;
    }

    read_len = fread(buf, 1, len, fscript);
    if (read_len < len) {
        db_debug("len = %d, read_len = %d, fix me\n", len, read_len);
    }
    fclose(fscript);

    /* parse script */
    shmid = __parse_script(buf, len);
    free(buf);

    return shmid;
}

void deparse_script(int shmid)
{
    shmctl(shmid, IPC_RMID, 0);
}
