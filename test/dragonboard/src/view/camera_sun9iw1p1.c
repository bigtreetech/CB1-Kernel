/*
 * \file        cameratest.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年06月26日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

//#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

#include "sun8iw5p_display.h"
#include "dragonboard_inc.h"
#include "videodev2.h"
#define ALIGN_4K(x) (((x) + (4095)) & ~(4095))
#define ALIGN_32B(x) (((x) + (31)) & ~(31))
#define ALIGN_16B(x) (((x) + (15)) & ~(15))

//#define DEBUG_CAMERA
struct test_layer_info
{
	int screen_id;
	int layer_id;
	int mem_id;
	disp_layer_info layer_info;
	int addr_map;
	int width,height;//screen size
	int dispfh;//device node handle
	int fh;//picture resource file handle
	int mem;
	int clear;//is clear layer
	char filename[32];
};

struct test_layer_info test_info;

struct buffer
{
    void   *start;
    size_t length;
};

static int csi_format;
static int fps;
static int req_frame_num;

static struct buffer *buffers = NULL;
static int nbuffers = 0;

static disp_size input_size;
static disp_size display_size;

static int disp;
static int layer;

static int screen_width;
static int screen_height;


static int stop_flag;

static int dev_no;
static int dev_cnt;
static int csi_cnt;
static int sensor_type = 0;
static int camera_layer =1;
static int screen_id = 0;
static pthread_t video_tid;

static int getSensorType(int fd)
{
	int ret = -1;
	struct v4l2_control ctrl;
	struct v4l2_queryctrl qc_ctrl;

	if (fd == NULL)
	{
		return 0xFF000000;
	}

	ctrl.id = V4L2_CID_SENSOR_TYPE;
	qc_ctrl.id = V4L2_CID_SENSOR_TYPE;

	if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &qc_ctrl))
	{
		db_error("query sensor type ctrl failed");
		return -1;
	}
	ret = ioctl(fd, VIDIOC_G_CTRL, &ctrl);
	return ctrl.value;
}

static int read_frame(int fd)
{
    struct v4l2_buffer buf;
	unsigned int phyaddr;
    memset(&buf, 0, sizeof(struct v4l2_buffer));
    buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ioctl(fd, VIDIOC_DQBUF, &buf);
	if(sensor_type == V4L2_SENSOR_TYPE_YUV){
		phyaddr = buf.m.offset;
	}
	else if(sensor_type == V4L2_SENSOR_TYPE_RAW){
		phyaddr = buf.m.offset + ALIGN_4K(ALIGN_16B(input_size.width) * input_size.height * 3 >> 1);
	}
    disp_set_addr(display_size.width, display_size.height, &phyaddr);
#ifdef DEBUG_CAMERA
	static int a =0;
	int ret;
	if(a ==50){
		FILE *frame_file = NULL;
		frame_file = fopen("/data/frame.yuv","wb");
		if (frame_file == NULL) db_error("open err\n");
		db_error("buf.length: %d\n",buf.length);

		void *addr = mmap (NULL, buf.length,
				    PROT_READ | PROT_WRITE,
				    MAP_SHARED ,
				    fd,
				    buf.m.offset);

		ret=fwrite(addr,1,buf.length,frame_file);
		db_debug("fwrite ret is %d\n",ret);
		fclose(frame_file);
		munmap(addr,buf.length);
		a++;
	}
	else a++;
#endif
    ioctl(fd, VIDIOC_QBUF, &buf);
    return 1;
}

static int disp_init(int x,int y,int width,int height)

{
	unsigned int arg[6];
	memset(&test_info, 0, sizeof(struct test_layer_info));
	if((test_info.dispfh = open("/dev/disp",O_RDWR)) == -1) {
		db_error("open display device fail!\n");
		return -1;
	}
	//get current output type
	disp_output_type output_type;
	for (screen_id = 0;screen_id < 3;screen_id++){
		arg[0] = screen_id;
		output_type = (disp_output_type)ioctl(test_info.dispfh, DISP_CMD_GET_OUTPUT_TYPE, (void*)arg);
		if(output_type != DISP_OUTPUT_TYPE_NONE){
			db_debug("the output type: %d\n",screen_id);
			break;
		}
	}
	test_info.screen_id = screen_id; //0 for lcd ,1 for hdmi, 2 for edp
	test_info.layer_id = camera_layer;
	test_info.layer_info.ck_enable        = 0;
	test_info.layer_info.alpha_mode       = 1; //global alpha
	test_info.layer_info.alpha_value      = 0xff;
	test_info.layer_info.pipe             = 0;
	//test_info.width = ioctl(test_info.dispfh,DISP_CMD_GET_SCN_WIDTH,(void*)arg);	//get screen width and height
	//test_info.height = ioctl(test_info.dispfh,DISP_CMD_GET_SCN_HEIGHT,(void*)arg);

	//display window of the screen
	test_info.layer_info.screen_win.x = x;
	test_info.layer_info.screen_win.y = y;
	test_info.layer_info.screen_win.width    = width;
	test_info.layer_info.screen_win.height   = height;

	//mode
	test_info.layer_info.mode = DISP_LAYER_WORK_MODE_SCALER; //scaler mode

	//data format
	test_info.layer_info.fb.format = DISP_FORMAT_YUV420_SP_UVUV;
	return 0;
}
static int disp_quit(void)
{
	int ret;
	unsigned int arg[6];
	arg[0] = test_info.screen_id;
	arg[1] = test_info.layer_id;
	arg[2] = 0;
	arg[3] = 0;
	ret = ioctl(test_info.dispfh,DISP_CMD_LAYER_DISABLE,(void*)arg);
	if(0 != ret)
		db_error("fail to enable layer\n");

	memset(&test_info, 0, sizeof(struct test_layer_info));
	test_info.layer_id = camera_layer;
	arg[0] = test_info.screen_id;
	arg[1] = test_info.layer_id;
	arg[2] = 0;
	arg[3] = 0;
	ret = ioctl(test_info.dispfh, DISP_CMD_LAYER_SET_INFO, (void*)arg);

	if(0 != ret)
		db_error("fail to set layer info\n");
	close(test_info.dispfh);
	return 0;
}
int disp_set_addr(int width, int height, unsigned int *addr)

{
	unsigned int arg[6];
	int ret;
	disp_layer_info layer_info;

	//source frame size
	test_info.layer_info.fb.size.width = width;
	test_info.layer_info.fb.size.height = height;

	test_info.layer_info.fb.src_win.width = width;
	test_info.layer_info.fb.src_win.height = height;

	test_info.layer_info.fb.addr[0] = *addr;
	test_info.layer_info.fb.addr[1] = (int)(test_info.layer_info.fb.addr[0] + width*height);
	test_info.layer_info.fb.addr[2] = (int)(test_info.layer_info.fb.addr[0] + width*height*5/4);

	arg[0] = test_info.screen_id;
	arg[1] = test_info.layer_id;
	arg[2] = (int)&test_info.layer_info;
	arg[3] = 0;
	ret = ioctl(test_info.dispfh, DISP_CMD_LAYER_SET_INFO, (void*)arg);
	if(0 != ret)
		db_error("fail to set layer info\n");

	arg[0] = test_info.screen_id;
	arg[1] = test_info.layer_id;
	arg[2] = (int)&layer_info;
	ret = ioctl(test_info.dispfh, DISP_CMD_LAYER_GET_INFO, (void*)arg);
	if(0 != ret)
		db_error("fail to get layer info\n");

	arg[0] = test_info.screen_id;
	arg[1] = test_info.layer_id;
	arg[2] = 0;
	arg[3] = 0;
	ret = ioctl(test_info.dispfh,DISP_CMD_LAYER_ENABLE,(void*)arg);
	if(0 != ret)
		db_error("fail to enable layer\n");

	return 0;
}

static void *video_mainloop(void *args)
{
    int fd;
    fd_set fds;
    struct timeval tv;
    int r;
    char dev_name[32];
    struct v4l2_input inp;
    struct v4l2_format fmt;
	struct v4l2_format sub_fmt;
    struct v4l2_streamparm parms;
    struct v4l2_requestbuffers req;
    int i;
    enum v4l2_buf_type type;

    if (csi_cnt == 1) {
        snprintf(dev_name, sizeof(dev_name), "/dev/video1");
    }
    else {
        snprintf(dev_name, sizeof(dev_name), "/dev/video%d", (int)args);
    }
    db_debug("open %s\n", dev_name);
    if ((fd = open(dev_name, O_RDWR,S_IRWXU)) < 0) {
        db_error("can't open %s(%s)\n", dev_name, strerror(errno));
        goto open_err;
    }

    fcntl(fd, F_SETFD, FD_CLOEXEC);
    if (csi_cnt == 1) {
        inp.index = (int)args;
    }
    else {
        inp.index = 0;
    }
    inp.type = V4L2_INPUT_TYPE_CAMERA;

    /* set input input index */
    if (ioctl(fd, VIDIOC_S_INPUT, &inp) == -1) {
        db_error("VIDIOC_S_INPUT error\n");
        goto err;
    }

    parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
   // parms.parm.capture.capturemode = V4L2_MODE_VIDEO;
    parms.parm.capture.timeperframe.numerator = 1;
    parms.parm.capture.timeperframe.denominator = fps;
    if (ioctl(fd, VIDIOC_S_PARM, &parms) == -1) {
        db_error("set frequence failed\n");
        //goto err;
    }
    /* set image format */
    memset(&fmt, 0, sizeof(struct v4l2_format));
	memset(&sub_fmt, 0, sizeof(struct v4l2_format));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = input_size.width;
    fmt.fmt.pix.height      = input_size.height;
    fmt.fmt.pix.pixelformat = csi_format;
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;

	//get sensor type
	sensor_type = getSensorType(fd);
	if(sensor_type == V4L2_SENSOR_TYPE_RAW){
		fmt.fmt.pix.subchannel = &sub_fmt;
		fmt.fmt.pix.subchannel->width = 640;
		fmt.fmt.pix.subchannel->height = 480;
		fmt.fmt.pix.subchannel->pixelformat = csi_format;
		fmt.fmt.pix.subchannel->field = V4L2_FIELD_NONE;
	}
    if (ioctl(fd, VIDIOC_S_FMT, &fmt)<0) {
        db_error("set image format failed\n");
        goto err;
    }
	input_size.width = fmt.fmt.pix.width;
	input_size.height = fmt.fmt.pix.height;
	if(sensor_type == V4L2_SENSOR_TYPE_YUV){
		display_size.width = fmt.fmt.pix.width;
		display_size.height = fmt.fmt.pix.height;
	}else if(sensor_type == V4L2_SENSOR_TYPE_RAW){
		display_size.width = fmt.fmt.pix.subchannel->width;
		display_size.height = fmt.fmt.pix.subchannel->height;
	}
	db_debug("image input width #%d height #%d, diplay width #%d height %d\n",
			input_size.width, input_size.height, display_size.width, display_size.height);

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

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
        db_error("VIDIOC_STREAMON error\n");
        goto disp_exit;
    }

    while (1) {
        if (stop_flag)
            break;

        while (1) {
            if (stop_flag)
                break;

            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            // timeout
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

            if (read_frame(fd)) {
               // break;
            }
        }
     FD_ZERO(&fds);
    }

    if(-1==ioctl(fd, VIDIOC_STREAMOFF, &type))
    {
        db_error("vidioc_streamoff error!\n");
        pthread_exit(-1);
    }
    //disp_stop();
    for (i = 0; i < nbuffers; i++) {
      if(-1==munmap(buffers[i].start, buffers[i].length))
        {
            db_error("munmap error!\n");
            pthread_exit(-1);
        }
    }
    free(buffers);
   if(0!= close(fd))
   {
        db_error("close video fd error!\n");
        pthread_exit(-1);
   }
   pthread_exit(0);
   db_error("ho my gad!\n");
stream_off:
    ioctl(fd, VIDIOC_STREAMOFF, &type);
disp_exit:
    //disp_stop();
unmap:
    for (i = 0; i < nbuffers; i++) {
        munmap(buffers[i].start, buffers[i].length);
    }
buffer_rel:
    free(buffers);
err:
    close(fd);
open_err:
    pthread_exit((void *)-1);
}

int camera_test_init(int x,int y,int width,int height)
{
	db_debug("the window: x: %d,y: %d,width: %d,height: %d\n",x,y,width,height);
    if (script_fetch("camera", "dev_no", &dev_no, 1) ||
            (dev_no != 0 && dev_no != 1)) {
        dev_no = 0;
    }

    if (script_fetch("camera", "dev_cnt", &dev_cnt, 1) ||
            (dev_cnt != 1 && dev_cnt != 2)) {
        db_warn("camera: invalid dev_cnt #%d, use default #1\n", dev_cnt);
        dev_cnt = 1;
    }

    if (script_fetch("camera", "csi_cnt", &csi_cnt, 1) ||
            (csi_cnt != 1 && csi_cnt != 2)) {
        db_warn("camera: invalid csi_cnt #%d, use default #1\n", csi_cnt);
        csi_cnt = 1;
    }

    if (script_fetch("camera", "fps", &fps, 1) ||
            fps < 15) {
        db_warn("camera: invalid fps(must >= 15) #%d, use default #15\n", fps);
        fps = 15;
    }

    db_debug("camera: device count #%d, csi count #%d, fps #%d\n", dev_cnt, csi_cnt, fps);

    csi_format = V4L2_PIX_FMT_NV12;
    req_frame_num = 10;

    /* 受限于带宽，默认使用480p */
    input_size.width = 640;
    input_size.height = 480;



    if (disp_init(x,y,width,height) < 0) {
        db_error("camera: disp init failed\n");
        return -1;
    }

    if (dev_no >= dev_cnt) {
        dev_no = 0;
    }

    video_tid = 0;
    db_debug("camera: create video mainloop thread\n");
    if (pthread_create(&video_tid, NULL, video_mainloop, (void *)dev_no)) {
        db_error("camera: can't create video mainloop thread(%s)\n", strerror(errno));
        return -1;
    }

    return 0;
}

int camera_test_quit(void)
{
    void *retval;

    if (video_tid) {
        stop_flag = 1;
        db_msg("camera: waiting for camera thread finish...\n");
        if (pthread_join(video_tid, &retval)) {
            db_error("cameratester: can't join with camera thread\n");
        }
        db_msg("camera: camera thread exit code #%d\n", (int)retval);
        video_tid = 0;
    }

    disp_quit();

    return 0;
}

int get_camera_cnt(void)
{
    return dev_cnt;
}

int switch_camera(void)
{
    void *retval;

    if (video_tid) {
        stop_flag = 1;
        db_msg("camera: waiting for camera thread finish...\n");
        if (pthread_join(video_tid, &retval)) {
            db_error("cameratester: can't join with camera thread\n");
        }
        db_msg("camera: camera thread exit code #%d\n", (int)retval);
        video_tid = 0;
    }

    dev_no++;
    if (dev_no >= dev_cnt) {
        dev_no = 0;
    }

    stop_flag = 0;
    db_debug("cameratester: create video mainloop thread\n");
    if (pthread_create(&video_tid, NULL, video_mainloop, (void *)dev_no)) {
        db_error("camera: can't create video mainloop thread(%s)\n", strerror(errno));
        return -1;
    }

    return 0;
}
