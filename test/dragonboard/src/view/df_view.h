/*
 * \file        df_view.h
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#ifndef __DF_VIEW_H__
#define __DF_VIEW_H__

#include "list.h"

#define COLOR_WHITE_IDX                 0
#define COLOR_YELLOW_IDX                1
#define COLOR_GREEN_IDX                 2
#define COLOR_CYAN_IDX                  3
#define COLOR_MAGENTA_IDX               4
#define COLOR_RED_IDX                   5
#define COLOR_BLUE_IDX                  6
#define COLOR_BLACK_IDX                 7
#define COLOR_BEAUTY_IDX                8


struct color_rgb
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
};

extern struct color_rgb color_table[];

struct df_item_data
{
    int id;
    char name[32];
    char display_name[68];
    int category;
    int status;
    char exdata[68];

    struct list_head list;

    int x;
    int y;
    int width;
    int height;

    int bgcolor;
    int fgcolor;
};

int df_view_init(void);
int df_view_exit(void);

#endif /* __DF_VIEW_H__ */
