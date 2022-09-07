/*
 * \file        view.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dragonboard.h"
#include "df_view.h"
#include "view.h"

typedef int (*view_init_fn)(void);
typedef int (*view_exit_fn)(void);

static view_init_fn init_fn[] =
{
    df_view_init,
    NULL
};

static view_exit_fn exit_fn[] =
{
    df_view_exit,
    NULL
};

static struct list_head view_list;

int init_view(void)
{
    view_init_fn *init;

    db_debug("view: init...\n");

    /* init view list */
    INIT_LIST_HEAD(&view_list);

    /* call each view init function */
    for (init = init_fn; *init; init++) {
        (*init)();
    }

    return 0;
}

void exit_view(void)
{
    view_exit_fn *exit;

    db_debug("view: exit...\n");

    /* call each view exit function */
    for (exit = exit_fn; *exit; exit++)
        (*exit)();
}

int register_view(const char *name, struct view_operations *view_ops)
{
    struct view_info *vi;

    if (name == NULL || view_ops == NULL)
        return 0;

    vi = malloc(sizeof(struct view_info));
    if (vi == NULL)
        return 0;
    memset(vi, 0, sizeof(struct view_info));

    strcpy(vi->name, name);
    vi->view_ops = view_ops;

    list_add(&vi->list, &view_list);

    return (int)vi;
}

void unregister_view(int view)
{
    struct view_info *vi = (struct view_info *)view;

    list_del(&vi->list);
    free(vi);
}

int view_insert_item(int id, struct item_data *data)
{
    struct list_head *pos;

    if (NULL == data)
        return -1;

    list_for_each(pos, &view_list) {
        struct view_info *vi = list_entry(pos, struct view_info, list);
        if (vi->view_ops->insert_item)
            vi->view_ops->insert_item(id, data);
    }

    return 0;
}

int view_update_item(int id, struct item_data *data)
{
    struct list_head *pos;

    if (NULL == data)
        return -1;

    list_for_each(pos, &view_list) {
        struct view_info *vi = list_entry(pos, struct view_info, list);
        if (vi->view_ops->update_item)
            vi->view_ops->update_item(id, data);
    }

    return 0;
}

int view_delete_item(int id)
{
    printf("view: %s not implement yet\n", __func__);

    return 0;
}

void view_sync(void)
{
    struct list_head *pos;

    list_for_each(pos, &view_list) {
        struct view_info *vi = list_entry(pos, struct view_info, list);
        if (vi->view_ops->sync)
            vi->view_ops->sync();
    }
}
