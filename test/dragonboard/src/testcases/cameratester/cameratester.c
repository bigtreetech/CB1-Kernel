/*
 * \file        cameratester.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年06月26日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "sun8iw6p_display.h"
#include "dragonboard_inc.h"

struct buffer
{
    void   *start;
    size_t length;
};

struct size
{
	int width;
	int height;
};

static int fd;

static int csi_format;
static int fps;
static int req_frame_num;

static struct buffer *buffers = NULL;
static int nbuffers = 0;

static struct size input_size;
static struct size disp_size;

static int disp;
static int layer;
static __disp_layer_info_t layer_para;
static int screen_width;
static int screen_height;
static __disp_pixel_fmt_t disp_format;
static __disp_pixel_mod_t disp_seq;
static __disp_pixel_seq_t disp_mode;

static int disp_init(int width, int height)
{
    unsigned int args[4];

    if ((disp = open("/dev/disp", O_RDWR)) == -1) {
        db_error("can't open /dev/disp(%s)\n", strerror(errno));
        return -1;
    }

    args[0] = 0;
	screen_width  = ioctl(disp, DISP_CMD_SCN_GET_WIDTH, args);
	screen_height = ioctl(disp, DISP_CMD_SCN_GET_HEIGHT, args);

    args[0] = 0;
    args[1] = DISP_LAYER_WORK_MODE_SCALER;
    layer = ioctl(disp, DISP_CMD_LAYER_REQUEST, args);
    if (layer == 0) {
        db_error("request layer failed\n");
        return -1;
    }

    memset(&layer_para, 0, sizeof(__disp_layer_info_t));
    layer_para.mode            = DISP_LAYER_WORK_MODE_SCALER;
    layer_para.pipe            = 0;
    layer_para.prio            = 0;
    layer_para.alpha_en        = 1;
    layer_para.alpha_val       = 0xff;
    layer_para.ck_enable       = 0;
    layer_para.src_win.x       = 0;
    layer_para.src_win.y       = 0;
    layer_para.src_win.width   = width;
    layer_para.src_win.height  = height;
    layer_para.scn_win.x       = screen_width >> 1;
    layer_para.scn_win.y       = 0;
    layer_para.scn_win.width   = screen_width >> 1;
    layer_para.scn_win.height  = screen_height >> 1;
    layer_para.fb.addr[0]      = 0;
    layer_para.fb.addr[1]      = 0;
    layer_para.fb.addr[2]      = 0;
    layer_para.fb.size.width   = width;
    layer_para.fb.size.height  = height;
    layer_para.fb.format       = disp_format;
    layer_para.fb.seq          = disp_seq;
    layer_para.fb.mode         = disp_mode;
    layer_para.fb.br_swap      = 0;
    args[0] = 0;
    args[1] = layer;
    args[2] = (__u32)&layer_para;
    ioctl(disp, DISP_CMD_LAYER_SET_PARA, args);

    args[0] = 0;
    args[1] = layer;
    ioctl(disp, DISP_CMD_LAYER_TOP, args);

    args[0] = 0;
    args[1] = layer;
    ioctl(disp, DISP_CMD_LAYER_OPEN, args);

    return 0;
}

static int disp_start(void)
{
    unsigned int args[4];

    args[0] = 0;
    args[1] = layer;
    return ioctl(disp, DISP_CMD_VIDEO_START, args);
}

static int disp_stop(void)
{
    unsigned int args[4];

    args[0] = 0;
    args[1] = layer;
    return ioctl(disp, DISP_CMD_VIDEO_STOP, args);
}

static int disp_quit(void)
{
    unsigned int args[4];

    args[0] = 0;
    args[0] = layer;
    ioctl(disp, DISP_CMD_LAYER_CLOSE, args);
    ioctl(disp, DISP_CMD_LAYER_RELEASE, args);

    return 0;
}

static int disp_set_addr(int width, int height, unsigned int *addr)
{
    __disp_video_fb_t fb;
    unsigned int args[4];

    memset(&fb, 0, sizeof(__disp_video_fb_t));

    fb.interlace       = 0;
    fb.top_field_first = 0;
    fb.frame_rate      = 25;
    fb.addr[0]         = *addr;
    fb.id              = 0;

    switch (csi_format) {
        case V4L2_PIX_FMT_YUV422P:
        case V4L2_PIX_FMT_YUYV:
        case V4L2_PIX_FMT_YVYU:
        case V4L2_PIX_FMT_UYVY:
        case V4L2_PIX_FMT_VYUY:
            fb.addr[1] = *addr + width * height;
            fb.addr[2] = *addr + width * height * 3 / 2;
            break;

        case V4L2_PIX_FMT_YUV420:
            fb.addr[1] = *addr + width * height;
            fb.addr[2] = *addr + width * height * 5 / 4;
            break;

        case V4L2_PIX_FMT_NV16:
        case V4L2_PIX_FMT_NV12:
        case V4L2_PIX_FMT_HM12:
            fb.addr[1] = *addr + width * height;
            fb.addr[2] = layer_para.fb.addr[1];
            break;

        default:
            break;
    }

    args[0] = 0;
    args[1] = layer;
    args[2] = (__u32)&fb;
    return ioctl(disp, DISP_CMD_VIDEO_SET_FB, args);
}

static int read_frame(void)
{
    struct v4l2_buffer buf;

    memset(&buf, 0, sizeof(struct v4l2_buffer));
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ioctl(fd, VIDIOC_DQBUF, &buf);
    disp_set_addr(disp_size.width, disp_size.height, &buf.m.offset);
    ioctl(fd, VIDIOC_QBUF, &buf);
    return 1;
}

int video_mainloop(void)
{
    char dev_name[32];
    struct v4l2_input inp;
    struct v4l2_format fmt;
    struct v4l2_streamparm parms;
    struct v4l2_requestbuffers req;
    int i;
    enum v4l2_buf_type type;

    strncpy(dev_name, "/dev/video0", 32);
    db_debug("open %s\n", dev_name);
    if ((fd = open(dev_name, O_RDWR | O_NONBLOCK, 0)) < 0) {
        db_error("can't open %s(%s)\n", dev_name, strerror(errno));
        goto open_err;
    }

    inp.index = 0;
    inp.type = V4L2_INPUT_TYPE_CAMERA;

    /* set input input index */
    if (ioctl(fd, VIDIOC_S_INPUT, &inp) == -1) {
        db_error("VIDIOC_S_INPUT error\n");
        goto err;
    }

    /* set image format */
    memset(&fmt, 0, sizeof(struct v4l2_format));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = input_size.width;
    fmt.fmt.pix.height      = input_size.height;
    fmt.fmt.pix.pixelformat = csi_format;
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        db_error("set image format failed\n");
        goto err;
    }

	disp_size.width = fmt.fmt.pix.width;
	disp_size.height = fmt.fmt.pix.height;
	db_debug("image input width #%d height #%d, diplay width #%d height %d\n",
			input_size.width, input_size.height, disp_size.width, disp_size.height);

    parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parms.parm.capture.timeperframe.denominator = 1;
    parms.parm.capture.timeperframe.denominator = fps;
    if (ioctl(fd, VIDIOC_S_PARM, &parms) == -1) {
        db_error("set frequence failed\n");
        goto err;
    }

    /* request buffer */
    memset(&req, 0, sizeof(struct v4l2_requestbuffers));
    req.count  = req_frame_num;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ioctl(fd, VIDIOC_REQBUFS, &req);

    buffers = calloc(req.count, sizeof(struct buffer));
    for (nbuffers = 0; nbuffers < req.count; nbuffers++) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = nbuffers;

        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) == -1) {
            db_error("VIDIOC_QUERYBUF error\n");
            goto buffer_rel;
        }

        buffers[nbuffers].start  = mmap(NULL, buf.length,
                PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        buffers[nbuffers].length = buf.length;
        if (buffers[nbuffers].start == MAP_FAILED) {
            db_error("mmap failed\n");
            goto buffer_rel;
        }
    }

    for (i = 0; i < nbuffers; i++) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(struct v4l2_buffer));
        buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;
        if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
            db_error("VIDIOC_QBUF error\n");
            goto unmap;
        }
    }

    disp_init(disp_size.width, disp_size.height);
    disp_start();

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        db_error("VIDIOC_STREAMON error\n");
        goto disp_exit;
    }

    while (1) {
        while (1) {
            fd_set fds;
            struct timeval tv;
            int r;

            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* timeout */
            tv.tv_sec  = 2;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);
            if (r == -1) {
                if (errno == EINTR) {
                    continue;
                }

                db_error("select error\n");
            }

            if (r == 0) {
                db_error("select timeout\n");
                goto stream_off;
            }

            if (read_frame()) {
                break;
            }
        }
    }

stream_off:
    ioctl(fd, VIDIOC_STREAMOFF, &type);
disp_exit:
    disp_stop();
    disp_quit();
unmap:
    for (i = 0; i < nbuffers; i++) {
        munmap(buffers[i].start, buffers[i].length);
    }
buffer_rel:
    free(buffers);
err:
    close(fd);
open_err:
    return -1;
}

int main(int argc, char *argv[])
{
    csi_format = V4L2_PIX_FMT_NV12;
	fps = 30;
    req_frame_num = 5;

    input_size.width = 1280;
    input_size.height = 720;

    disp_format = DISP_FORMAT_YUV420;
    disp_seq = DISP_SEQ_UVUV;
    disp_mode = DISP_MOD_NON_MB_UV_COMBINED;

    video_mainloop();

    return -1;
}
