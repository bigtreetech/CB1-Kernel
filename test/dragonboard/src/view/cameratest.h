/*
 * \file        cameratest.h
 * \brief
 *
 * \version     1.0.0
 * \date        2012年08月08日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#ifndef __CAMERA_TEST_H__
#define __CAMERA_TEST_H__

int camera_test_quit(void);
int camera_test_init(int x,int y,int width,int height);
int get_camera_cnt(void);
int switch_camera(void);

#endif /* __CAMERA_TEST_H__ */
