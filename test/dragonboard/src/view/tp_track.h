/*
 * \file        tp_track.h
 * \brief
 *
 * \version     1.0.0
 * \date        2012年06月27日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#ifndef __TP_TRACK_H__
#define __TP_TRACK_H__

int tp_track_init(void);
int tp_track_draw(int x, int y, int press);
int tp_track_clear(void);

#endif /* __TP_TRACK_H__ */
