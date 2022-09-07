/*
 * \file        view.h
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#ifndef __VIEW_H__
#define __VIEW_H__

#include "list.h"

struct item_data
{
    char name[32];
    char display_name[68];
    int category;
    int status;
    char exdata[68];
};

struct view_operations
{
    int (*insert_item)(int id, struct item_data *data);
    int (*update_item)(int id, struct item_data *data);
    int (*delete_item)(int id);
    void (*sync)(void);
};

struct view_info
{
    char name[16];
    struct view_operations *view_ops;
    struct list_head list;
};

int init_view(void);
void exit_view(void);

int register_view(const char *name, struct view_operations *view_ops);
void unregister_view(int view);

int view_insert_item(int id, struct item_data *data);
int view_update_item(int id, struct item_data *data);
int view_delete_item(int id);

void view_sync(void);

#endif /* __VIEW_H__ */
