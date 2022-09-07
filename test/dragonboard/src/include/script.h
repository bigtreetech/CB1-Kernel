/*
 * \file        script.h
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#ifndef __SCRIPT_H__
#define __SCRIPT_H__

struct script_gpio_set
{
    char name[32];
    int port;
    int port_num;
    int mul_sel;
    int pull;
    int drv_level;
    int data;
};

/*
 * init script. called before other function.
 * \param shmid.
 */
int init_script(int shmid);

/*
 * deinit script.
 */
void deinit_script(void);

/*
 * get the number of main key in script.
 */
int script_mainkey_cnt(void);

/*
 * get the name of specified index main key.
 */
int script_mainkey_name(int idx, char *name);

/*
 * fetch word, string, gpio from script.
 */
int script_fetch(char *main_name, char *sub_name, int value[], int count);

/*
 * fetch gpio set from script.
 */
int script_fetch_gpio_set(char *main_name, struct script_gpio_set *gpio_set,
        int gpio_cnt);

#endif /* __SCRIPT_H__ */
