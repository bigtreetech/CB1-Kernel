/*
 * \file        df_view.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 * \Descriptions:
 *      create the inital version
 *
 * \version     1.1.0
 * \date        2012年09月26日
 * \author      Martin <zhengjiewen@allwinnertech.com>
 * \Descriptions:
 *      add some new features:
 *      1.wifi hotpoint display with ssid and single strongth value.
 *      2.add mic power bar to the ui
 *      3.make the display automatically switch between lcd and hdmi
 *      4.the ui will automatically adjust under different test config
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <directfb.h>

#if defined (_SUN8IW6P) || (_SUN50IW1P) || (_SUN50IW3P)
#include "sun8iw6p_display.h"
#elif defined (_SUN8IW5P) || (_SUN9IW1P)
#include "sun8iw5p_display.h"
#else
#include "sunxi_display2.h"
#endif

#include "dragonboard.h"
#include "script.h"
#include "view.h"
#include "test_case.h"
#include "tp_track.h"
#include "cameratest.h"
#include "df_view.h"

extern int get_auto_testcases_number(void);

extern int get_manual_testcases_number(void);

#ifndef _SUN9IW1P
static int reboot_button_init(void);
static int poweroff_button_init(void);
#endif

#define DFBCHECK(x...) \
    do {                                                             \
        DFBResult err = x;                                           \
        if (err != DFB_OK) {                                         \
             fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );  \
             DirectFBErrorFatal( #x, err );                          \
        }                                                            \
    } while (0)

#define CLAMP(x, low, high) \
    (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#define DATADIR                         "/dragonboard/fonts"

#define FONT16_HEIGHT                   16
#define FONT20_HEIGHT                   20
#define FONT24_HEIGHT                   24
#define FONT28_HEIGHT                   28
#define FONT48_HEIGHT                   48
#define DEFAULT_FONT_SIZE               FONT20_HEIGHT

#define MENU_HEIGHT                     48
#define ITEM_HEIGHT                     48

#define BUTTON_WIDTH                    96
#define BUTTON_HEIGHT                   32

#define HEIGHT_ADJUST                   (2 * ITEM_HEIGHT)

#define AUTO_MENU_NAME                  "自动测试项"
#define MANUAL_MENU_NAME                "手动测试项"
#define WIFI_MENU_NAME                  "WIFI热点列表"

#define CLEAR_BUTTON_NAME               "清屏"
#define SWITCH_BUTTON_NAME              "切换"
#define REBOOT_BUTTON_NAME              "重启"
#define POWEROFF_BUTTON_NAME            "关机"
#define TP_DISPLAY_NAME                 "触摸"

#define BUILDIN_TC_ID_TP                -1

#define MIC_TITLE_HEIGHT               MENU_HEIGHT
#define MIC_POWER_BAR_HEIGHT_PERCENT     10
#define MIC_POWER_BAR_WITH_SHIFT         5
#define MIC_POWER_BAR_WITH_SHIFT_TOTAL   (MIC_POWER_BAR_WITH_SHIFT-1)
#define MIC_WINDOW_SHOW 0



IDirectFB                      *dfb;
IDirectFBDisplayLayer          *layer;
DFBDisplayLayerConfig           layer_config;
DFBGraphicsDeviceDescription    gdesc;

/* font declaration */
IDirectFBFont                  *font16;
IDirectFBFont                  *font20;
IDirectFBFont                  *font24;
IDirectFBFont                  *font28;
IDirectFBFont                  *font48;
IDirectFBFont                  *font;

static int font_size;

/* input device: tp */
static IDirectFBInputDevice    *tp_dev = NULL;

/* input interfaces: device and its buffer */
static IDirectFBEventBuffer    *events;

static pthread_t evt_tid;

static int df_view_id;

static struct list_head auto_tc_list;
static struct list_head manual_tc_list;

struct color_rgb color_table[9] =
{
    {0xff, 0xff, 0xff},
    {0xff, 0xff, 0x00},
    {0x00, 0xff, 0x00},
    {0x00, 0xff, 0xff},
    {0xff, 0x00, 0xff},
    {0xff, 0x00, 0x00},
    {0x00, 0x00, 0xff},
    {0x00, 0x00, 0x00},
    {0x08, 0x46, 0x84}
};

static int item_init_bgcolor;
static int item_init_fgcolor;
static int item_ok_bgcolor;
static int item_ok_fgcolor;
static int item_fail_bgcolor;
static int item_fail_fgcolor;

static char pass_str[12];
static char fail_str[12];

static struct item_data tp_data;

struct df_window
{
    IDirectFBWindow        *window;
    IDirectFBSurface       *surface;
    DFBWindowDescription    desc;
    int                     bgcolor;
};

struct df_window_menu
{
    char name[36];
    int width;
    int height;
    int bgcolor;
    int fgcolor;
};

struct df_button
{
    char name[20];
    int x;
    int y;
    int width;
    int height;
    int bgcolor;
    int fgcolor;
    int bdcolor;
};

static struct df_window         auto_window;
static struct df_window_menu    auto_window_menu;

static struct df_window         manual_window;
static struct df_window_menu    manual_window_menu;

static struct df_window         video_window;

static struct df_window         misc_window;

static struct df_window         wifi_window;
static struct df_window_menu    wifi_window_menu;

static struct df_window         mic1_window;


static struct df_window         mic2_window;


struct one_wifi_hot_point{
    char ssid[64];
    char single_level_db[16];
    int  single_level;
    int x;
    int y;
    int width;
    int height;
    int bgcolor;
    int fgcolor;
    struct list_head list;
    };
struct wifi_hot_point_display_t{
    int current_display_position_x;
    int current_display_position_y;
    int one_point_display_width;
    int one_point_display_height;
    int total_point_can_be_display;
    int total_point_have_displayed;
    int total_point_searched;
    };

static struct wifi_hot_point_display_t wifi_hot_point_display;
static struct list_head wifi_list;

struct mic_power_bar_t{
  int BarColor[2];
  int XOff, YOff;
  int width,height;
  int v;       //desire volume
  int Min, Max;//the volume space
  char bar_name[16];
  struct df_window * mic_window;
};

static struct mic_power_bar_t mic1_power_bar;
static struct mic_power_bar_t mic2_power_bar;

static int mic_activated=0;
static int mic1_used    =0;
static int mic2_used    =0;

static int camera_activated=0;

#ifndef _SUN9IW1P
static struct df_button         clear_button;
static struct df_button         switch_button;
static struct df_button         switch_button;
static struct df_button         reboot_button;
static struct df_button         poweroff_button;
#endif


disp_output_type disp_output_type_t;

static int auto_window_init(void)
{
    db_debug("auto test case window init...\n");
    auto_window.desc.flags  = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT |
                               DWDESC_CAPS);

    auto_window.desc.posx   = 0;
   // auto_window.desc.posy   = (layer_config.height >> 1) + height_adjust;
    auto_window.desc.posy   = manual_window.desc.height;
    if(mic_activated && MIC_WINDOW_SHOW){
         auto_window.desc.width  = layer_config.width - (layer_config.width>>MIC_POWER_BAR_WITH_SHIFT);
    }
    else
    {
         auto_window.desc.width  = layer_config.width;
    }
    auto_window.desc.height = MENU_HEIGHT+ITEM_HEIGHT*get_auto_testcases_number();
    auto_window.desc.caps   = DWCAPS_ALPHACHANNEL;

    DFBCHECK(layer->CreateWindow(layer, &auto_window.desc, &auto_window.window));
    auto_window.window->GetSurface(auto_window.window, &auto_window.surface);
    auto_window.window->SetOpacity(auto_window.window, 0xff);
    auto_window.window->SetOptions(auto_window.window, DWOP_KEEP_POSITION);

    auto_window.bgcolor = item_init_bgcolor;
    DFBCHECK(auto_window.surface->SetColor(auto_window.surface,
                                           color_table[auto_window.bgcolor].r,
                                           color_table[auto_window.bgcolor].g,
                                           color_table[auto_window.bgcolor].b, 0xff));
    DFBCHECK(auto_window.surface->FillRectangle(auto_window.surface, 0, 0,
                auto_window.desc.width, auto_window.desc.height));

    /* draw menu */
    if (script_fetch("df_view", "auto_menu_name", (int *)auto_window_menu.name, 8)) {
        strcpy(auto_window_menu.name, AUTO_MENU_NAME);
    }

    auto_window_menu.width  = auto_window.desc.width;
    auto_window_menu.height = MENU_HEIGHT;

    if (script_fetch("df_view", "menu_bgcolor", &auto_window_menu.bgcolor, 1) ||
            auto_window_menu.bgcolor < COLOR_WHITE_IDX || auto_window_menu.bgcolor > COLOR_BLACK_IDX) {
        auto_window_menu.bgcolor = COLOR_YELLOW_IDX;
    }

    if (script_fetch("df_view", "menu_fgcolor", &auto_window_menu.fgcolor, 1) ||
            auto_window_menu.fgcolor < COLOR_WHITE_IDX || auto_window_menu.fgcolor > COLOR_BLACK_IDX) {
        auto_window_menu.fgcolor = COLOR_BLACK_IDX;
    }

    DFBCHECK(auto_window.surface->SetColor(auto_window.surface,
                                           color_table[auto_window_menu.bgcolor].r,
                                           color_table[auto_window_menu.bgcolor].g,
                                           color_table[auto_window_menu.bgcolor].b, 0xff));
    DFBCHECK(auto_window.surface->FillRectangle(auto_window.surface, 0, 0, auto_window_menu.width, auto_window_menu.height));
    DFBCHECK(auto_window.surface->SetFont(auto_window.surface, font48));
    DFBCHECK(auto_window.surface->SetColor(auto_window.surface,
                                           color_table[auto_window_menu.fgcolor].r,
                                           color_table[auto_window_menu.fgcolor].g,
                                           color_table[auto_window_menu.fgcolor].b, 0xff));
    DFBCHECK(auto_window.surface->DrawString(auto_window.surface, auto_window_menu.name, -1, 4,
                MENU_HEIGHT / 2 + FONT48_HEIGHT / 2 - FONT48_HEIGHT / 8, DSTF_LEFT));

    auto_window.surface->Flip(auto_window.surface, NULL, 0);

    return 0;
}

#ifndef _SUN9IW1P
static int clear_button_init(void)
{
    if (script_fetch("df_view", "clear_button_name", (int *)clear_button.name, 4)) {
        strcpy(clear_button.name, CLEAR_BUTTON_NAME);
    }

    clear_button.width  = BUTTON_WIDTH;
    clear_button.height = BUTTON_HEIGHT;
    clear_button.x      = manual_window.desc.width - clear_button.width;
    clear_button.y      = 0;

    clear_button.bdcolor = COLOR_RED_IDX;
    clear_button.bgcolor = COLOR_BLUE_IDX;
    clear_button.fgcolor = COLOR_WHITE_IDX;

    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[clear_button.bdcolor].r,
                                             color_table[clear_button.bdcolor].g,
                                             color_table[clear_button.bdcolor].b, 0xff));
    DFBCHECK(manual_window.surface->DrawLine(manual_window.surface, clear_button.x,
                                                                    clear_button.y,
                                                                    clear_button.x,
                                                                    clear_button.y + clear_button.height - 1));
    DFBCHECK(manual_window.surface->DrawLine(manual_window.surface, clear_button.x,
                                                                    clear_button.y,
                                                                    clear_button.x + clear_button.width - 1,
                                                                    clear_button.y));
    DFBCHECK(manual_window.surface->DrawLine(manual_window.surface, clear_button.x + clear_button.width - 1,
                                                                    clear_button.y,
                                                                    clear_button.x + clear_button.width - 1,
                                                                    clear_button.y + clear_button.height - 1));
    DFBCHECK(manual_window.surface->DrawLine(manual_window.surface, clear_button.x,
                                                                    clear_button.y + clear_button.height - 1,
                                                                    clear_button.x + clear_button.width - 1,
                                                                    clear_button.y + clear_button.height - 1));

    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[clear_button.bgcolor].r,
                                             color_table[clear_button.bgcolor].g,
                                             color_table[clear_button.bgcolor].b, 0xff));
    DFBCHECK(manual_window.surface->FillRectangle(manual_window.surface, clear_button.x + 1,
                                                                         clear_button.y + 1,
                                                                         clear_button.width - 2,
                                                                         clear_button.height - 2));

    DFBCHECK(manual_window.surface->SetFont(manual_window.surface, font24));
    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[clear_button.fgcolor].r,
                                             color_table[clear_button.fgcolor].g,
                                             color_table[clear_button.fgcolor].b, 0xff));
    DFBCHECK(manual_window.surface->DrawString(manual_window.surface, clear_button.name, -1, clear_button.x + (clear_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    manual_window.surface->Flip(manual_window.surface, NULL, 0);

    return 0;
}

static int clear_button_redraw(color)
{
    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[color].r,
                                             color_table[color].g,
                                             color_table[color].b, 0xff));
    DFBCHECK(manual_window.surface->FillRectangle(manual_window.surface, clear_button.x + 1,
                                                                         clear_button.y + 1,
                                                                         clear_button.width - 2,
                                                                         clear_button.height - 2));

    DFBCHECK(manual_window.surface->SetFont(manual_window.surface, font24));
    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[clear_button.fgcolor].r,
                                             color_table[clear_button.fgcolor].g,
                                             color_table[clear_button.fgcolor].b, 0xff));
    DFBCHECK(manual_window.surface->DrawString(manual_window.surface, clear_button.name, -1, clear_button.x + (clear_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    manual_window.surface->Flip(manual_window.surface, NULL, 0);

    return 0;
}
#endif

static int manual_window_init(void)
{
    db_debug("manual test case window init...\n");
    printf("layer_config.width =  %d, layer_config.height = %d\n",layer_config.width,layer_config.height);
    manual_window.desc.flags  = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT |
                                 DWDESC_CAPS);

    manual_window.desc.posx   = 0;
    manual_window.desc.posy   = 10;
    if(mic_activated && MIC_WINDOW_SHOW){
        manual_window.desc.width  = layer_config.width -(layer_config.width>>MIC_POWER_BAR_WITH_SHIFT);
    }
    else
    {
         manual_window.desc.width  = layer_config.width;
    }
   // manual_window.desc.height = (layer_config.height >> 1) + height_adjust;    /* get draw type */
    if (disp_output_type_t==DISP_OUTPUT_TYPE_LCD){

       manual_window.desc.height = MENU_HEIGHT+ITEM_HEIGHT*(1+get_manual_testcases_number());

    }
    else
    {
       manual_window.desc.height = MENU_HEIGHT+ITEM_HEIGHT*get_manual_testcases_number();
    }


    db_debug("manual_window.desc.height=%d\n",manual_window.desc.height);
    manual_window.desc.caps   = DWCAPS_ALPHACHANNEL;

    DFBCHECK(layer->CreateWindow(layer, &manual_window.desc, &manual_window.window));
    manual_window.window->GetSurface(manual_window.window, &manual_window.surface);
    manual_window.window->SetOpacity(manual_window.window, 0xff);
    manual_window.window->SetOptions(manual_window.window, DWOP_KEEP_POSITION);

    manual_window.bgcolor = item_init_bgcolor;
    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[manual_window.bgcolor].r,
                                             color_table[manual_window.bgcolor].g,
                                             color_table[manual_window.bgcolor].b, 0xff));
    DFBCHECK(manual_window.surface->FillRectangle(manual_window.surface, 0, 0,
                manual_window.desc.width, manual_window.desc.height));

    /* draw menu */
    if (script_fetch("df_view", "manual_menu_name", (int *)manual_window_menu.name, 8)) {
        strcpy(manual_window_menu.name, MANUAL_MENU_NAME);
    }

    manual_window_menu.width  = manual_window.desc.width;
    manual_window_menu.height = MENU_HEIGHT;

    if (script_fetch("df_view", "menu_bgcolor", &manual_window_menu.bgcolor, 1) ||
            manual_window_menu.bgcolor < COLOR_WHITE_IDX || manual_window_menu.bgcolor > COLOR_BLACK_IDX) {
        manual_window_menu.bgcolor = COLOR_YELLOW_IDX;
    }

    if (script_fetch("df_view", "menu_fgcolor", &manual_window_menu.fgcolor, 1) ||
            manual_window_menu.fgcolor < COLOR_WHITE_IDX || manual_window_menu.fgcolor > COLOR_BLACK_IDX) {
        manual_window_menu.fgcolor = COLOR_BLACK_IDX;
    }

    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[manual_window_menu.bgcolor].r,
                                             color_table[manual_window_menu.bgcolor].g,
                                             color_table[manual_window_menu.bgcolor].b, 0xff));
    DFBCHECK(manual_window.surface->FillRectangle(manual_window.surface, 0, 0, manual_window_menu.width, manual_window_menu.height));
    DFBCHECK(manual_window.surface->SetFont(manual_window.surface, font48));
    DFBCHECK(manual_window.surface->SetColor(manual_window.surface,
                                             color_table[manual_window_menu.fgcolor].r,
                                             color_table[manual_window_menu.fgcolor].g,
                                             color_table[manual_window_menu.fgcolor].b, 0xff));
    DFBCHECK(manual_window.surface->DrawString(manual_window.surface, manual_window_menu.name, -1, 4,
                MENU_HEIGHT / 2 + FONT48_HEIGHT / 2 - FONT48_HEIGHT / 8, DSTF_LEFT));

#ifndef _SUN9IW1P
    /* draw clear button */
    if(disp_output_type_t==DISP_OUTPUT_TYPE_LCD){
    clear_button_init();
    }
#endif
    manual_window.surface->Flip(manual_window.surface, NULL, 0);

    return 0;
}


#ifndef _SUN9IW1P
static int switch_button_init(void)
{
    db_debug("switch button init...\n");

    if (script_fetch("df_view", "swicth_button_name", (int *)switch_button.name, 4)) {
        strcpy(switch_button.name, SWITCH_BUTTON_NAME);
    }

    switch_button.width  = BUTTON_WIDTH;
    switch_button.height = BUTTON_HEIGHT;
    switch_button.x      = (misc_window.desc.width - switch_button.width) / 2;
    switch_button.y      = 0;

    switch_button.bdcolor = COLOR_RED_IDX;
    switch_button.bgcolor = COLOR_BLUE_IDX;
    switch_button.fgcolor = COLOR_WHITE_IDX;

    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[switch_button.bdcolor].r,
                                           color_table[switch_button.bdcolor].g,
                                           color_table[switch_button.bdcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, switch_button.x,
                                                                switch_button.y,
                                                                switch_button.x,
                                                                switch_button.y + switch_button.height - 1));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, switch_button.x,
                                                                switch_button.y,
                                                                switch_button.x + switch_button.width - 1,
                                                                switch_button.y));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, switch_button.x + switch_button.width - 1,
                                                                switch_button.y,
                                                                switch_button.x + switch_button.width - 1,
                                                                switch_button.y + switch_button.height - 1));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, switch_button.x,
                                                                switch_button.y + switch_button.height - 1,
                                                                switch_button.x + switch_button.width - 1,
                                                                switch_button.y + switch_button.height - 1));

    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[switch_button.bgcolor].r,
                                           color_table[switch_button.bgcolor].g,
                                           color_table[switch_button.bgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, switch_button.x + 1,
                                                                     switch_button.y + 1,
                                                                     switch_button.width - 2,
                                                                     switch_button.height - 2));

    DFBCHECK(misc_window.surface->SetFont(misc_window.surface, font24));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[switch_button.fgcolor].r,
                                           color_table[switch_button.fgcolor].g,
                                           color_table[switch_button.fgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawString(misc_window.surface, switch_button.name, -1, switch_button.x + (switch_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    misc_window.surface->Flip(misc_window.surface, NULL, 0);

    return 0;
}


static int switch_button_redraw(int color)
{
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[color].r,
                                           color_table[color].g,
                                           color_table[color].b, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, switch_button.x + 1,
                                                                     switch_button.y + 1,
                                                                     switch_button.width - 2,
                                                                     switch_button.height - 2));

    DFBCHECK(misc_window.surface->SetFont(misc_window.surface, font24));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[switch_button.fgcolor].r,
                                           color_table[switch_button.fgcolor].g,
                                           color_table[switch_button.fgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawString(misc_window.surface, switch_button.name, -1, switch_button.x + (switch_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    misc_window.surface->Flip(misc_window.surface, NULL, 0);

    return 0;
}


static int reboot_button_init(void)
{
    db_debug("reboot button init...\n");

    if (script_fetch("df_view", "reboot_button_name", (int *)reboot_button.name, 4)) {
        strcpy(reboot_button.name, REBOOT_BUTTON_NAME);
    }

    reboot_button.width  = BUTTON_WIDTH;
    reboot_button.height = BUTTON_HEIGHT;
    reboot_button.x      = misc_window.desc.width - reboot_button.width;
    reboot_button.y      = 0;

    reboot_button.bdcolor = COLOR_RED_IDX;
    reboot_button.bgcolor = COLOR_BLUE_IDX;
    reboot_button.fgcolor = COLOR_WHITE_IDX;

    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[reboot_button.bdcolor].r,
                                           color_table[reboot_button.bdcolor].g,
                                           color_table[reboot_button.bdcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, reboot_button.x,
                                                                reboot_button.y,
                                                                reboot_button.x,
                                                                reboot_button.y + reboot_button.height - 1));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, reboot_button.x,
                                                                reboot_button.y,
                                                                reboot_button.x + reboot_button.width - 1,
                                                                reboot_button.y));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, reboot_button.x + reboot_button.width - 1,
                                                                reboot_button.y,
                                                                reboot_button.x + reboot_button.width - 1,
                                                                reboot_button.y + reboot_button.height - 1));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, reboot_button.x,
                                                                reboot_button.y + reboot_button.height - 1,
                                                                reboot_button.x + reboot_button.width - 1,
                                                                reboot_button.y + reboot_button.height - 1));

    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[reboot_button.bgcolor].r,
                                           color_table[reboot_button.bgcolor].g,
                                           color_table[reboot_button.bgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, reboot_button.x + 1,
                                                                     reboot_button.y + 1,
                                                                     reboot_button.width - 2,
                                                                     reboot_button.height - 2));

    DFBCHECK(misc_window.surface->SetFont(misc_window.surface, font24));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[reboot_button.fgcolor].r,
                                           color_table[reboot_button.fgcolor].g,
                                           color_table[reboot_button.fgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawString(misc_window.surface, reboot_button.name, -1, reboot_button.x + (reboot_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    misc_window.surface->Flip(misc_window.surface, NULL, 0);

    return 0;
}


static int reboot_button_redraw(int color)
{
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[color].r,
                                           color_table[color].g,
                                           color_table[color].b, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, reboot_button.x + 1,
                                                                     reboot_button.y + 1,
                                                                     reboot_button.width - 2,
                                                                     reboot_button.height - 2));

    DFBCHECK(misc_window.surface->SetFont(misc_window.surface, font24));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[reboot_button.fgcolor].r,
                                           color_table[reboot_button.fgcolor].g,
                                           color_table[reboot_button.fgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawString(misc_window.surface, reboot_button.name, -1, reboot_button.x + (reboot_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    misc_window.surface->Flip(misc_window.surface, NULL, 0);

    return 0;
}


static int poweroff_button_init(void)
{
    db_debug("poweroff button init...\n");

    if (script_fetch("df_view", "poweroff_button_name", (int *)poweroff_button.name, 4)) {
        strcpy(poweroff_button.name, POWEROFF_BUTTON_NAME);
    }

    poweroff_button.width  = BUTTON_WIDTH;
    poweroff_button.height = BUTTON_HEIGHT;
    poweroff_button.x      = 0;
    poweroff_button.y      = 0;

    poweroff_button.bdcolor = COLOR_RED_IDX;
    poweroff_button.bgcolor = COLOR_BLUE_IDX;
    poweroff_button.fgcolor = COLOR_WHITE_IDX;

    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[poweroff_button.bdcolor].r,
                                           color_table[poweroff_button.bdcolor].g,
                                           color_table[poweroff_button.bdcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, poweroff_button.x,
                                                                poweroff_button.y,
                                                                poweroff_button.x,
                                                                poweroff_button.y + poweroff_button.height - 1));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, poweroff_button.x,
                                                                poweroff_button.y,
                                                                poweroff_button.x + poweroff_button.width - 1,
                                                                poweroff_button.y));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, poweroff_button.x + poweroff_button.width - 1,
                                                                poweroff_button.y,
                                                                poweroff_button.x + poweroff_button.width - 1,
                                                                poweroff_button.y + poweroff_button.height - 1));
    DFBCHECK(misc_window.surface->DrawLine(misc_window.surface, poweroff_button.x,
                                                                poweroff_button.y + poweroff_button.height - 1,
                                                                poweroff_button.x + poweroff_button.width - 1,
                                                                poweroff_button.y + poweroff_button.height - 1));

    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[poweroff_button.bgcolor].r,
                                           color_table[poweroff_button.bgcolor].g,
                                           color_table[poweroff_button.bgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, poweroff_button.x + 1,
                                                                     poweroff_button.y + 1,
                                                                     poweroff_button.width - 2,
                                                                     poweroff_button.height - 2));

    DFBCHECK(misc_window.surface->SetFont(misc_window.surface, font24));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[poweroff_button.fgcolor].r,
                                           color_table[poweroff_button.fgcolor].g,
                                           color_table[poweroff_button.fgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawString(misc_window.surface, poweroff_button.name, -1, poweroff_button.x + (poweroff_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    misc_window.surface->Flip(misc_window.surface, NULL, 0);

    return 0;
}


static int poweroff_button_redraw(int color)
{
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[color].r,
                                           color_table[color].g,
                                           color_table[color].b, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, poweroff_button.x + 1,
                                                                     poweroff_button.y + 1,
                                                                     poweroff_button.width - 2,
                                                                     poweroff_button.height - 2));

    DFBCHECK(misc_window.surface->SetFont(misc_window.surface, font24));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                           color_table[poweroff_button.fgcolor].r,
                                           color_table[poweroff_button.fgcolor].g,
                                           color_table[poweroff_button.fgcolor].b, 0xff));
    DFBCHECK(misc_window.surface->DrawString(misc_window.surface, poweroff_button.name, -1, poweroff_button.x + (poweroff_button.width >> 1),
                MENU_HEIGHT / 2 + FONT24_HEIGHT / 2 - FONT24_HEIGHT / 8, DSTF_CENTER));

    misc_window.surface->Flip(misc_window.surface, NULL, 0);

    return 0;
}
#endif


static int video_window_init(void)
{
    int x,y,height,width;

    if(!camera_activated) return 0;

    if(mic_activated){
        x= (layer_config.width >> 1)+(layer_config.width>>MIC_POWER_BAR_WITH_SHIFT);
        width  = (layer_config.width >> 1)-(layer_config.width>>MIC_POWER_BAR_WITH_SHIFT);

    }else
    {
        x = (layer_config.width >> 1);
        width = (layer_config.width >> 1);
    }

    y=0;
    height=layer_config.height>>1;


    video_window.desc.flags  = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT |
                               DWDESC_CAPS);

    video_window.desc.posx   = x;
    video_window.desc.posy   = y;
    video_window.desc.width  = width;
    video_window.desc.height = height;
    video_window.desc.caps   = DWCAPS_ALPHACHANNEL;

    DFBCHECK(layer->CreateWindow(layer, &video_window.desc, &video_window.window));
    video_window.window->GetSurface(video_window.window, &video_window.surface);
    video_window.window->SetOpacity(video_window.window, 0xff);
    video_window.window->SetOptions(video_window.window, DWOP_KEEP_POSITION);

    int i,idx=0;

      for (i = 0; i < video_window.desc.width; i+=video_window.desc.width>>3)
    {


         DFBCHECK(video_window.surface->SetColor(video_window.surface,
                                           color_table[idx].r,
                                           color_table[idx].g,
                                           color_table[idx].b, 0xff));
         DFBCHECK(video_window.surface->FillRectangle(video_window.surface, i, 0,
                video_window.desc.width>>3, video_window.desc.height));
         idx++;

    }

    DFBCHECK(video_window.surface->SetColor(video_window.surface,
                                               color_table[COLOR_WHITE_IDX].r,
                                               color_table[COLOR_WHITE_IDX].g,
                                               color_table[COLOR_WHITE_IDX].b, 0xff));
    video_window.surface->DrawLine(video_window.surface,0,video_window.desc.height-1,\
                                    video_window.desc.width,video_window.desc.height-1);
    video_window.surface->Flip(video_window.surface, NULL, 0);

    if (access("/tmp/camera_insmod_done", F_OK)) {
        db_error("camera module insmod incompletion\n");
        return 0;
    }
#ifndef _SUN9IW1P
    if (camera_test_init(x,y,width,height)) {
        return -1;
    }
    if (get_camera_cnt() > 1) {
        switch_button_init();
    }
    reboot_button_init();
    poweroff_button_init();
#endif
    return 0;
}

void* wifi_hot_point_proccess(void* argv)
{


    FILE *wifi_pipe;
    char buffer[64];
    char* ssid;
    char* single_level_db;
    int single;
    int one_level_width;
    int current_pozition_x;
    struct list_head *pos;
    IDirectFBSurface *surface;
    struct one_wifi_hot_point *p_hot_point;

    printf("this is the wifi_hot_point_proccess thread...\n");
    wifi_pipe = fopen(WIFI_PIPE_NAME, "r");
    setlinebuf(wifi_pipe);
while(1){
get_wifi_point_data:
    if (fgets(buffer, sizeof(buffer), wifi_pipe) == NULL) {
               continue;
       }

        if(0==strcmp("######\n",buffer)){

            // db_msg("one turn complete...\n");
            // db_msg("have_searched=%d\n",wifi_hot_point_display.total_point_searched);
            // db_msg("can_be_display=%d\n",wifi_hot_point_display.total_point_can_be_display);
            // db_msg("have_displayed=%d\n",wifi_hot_point_display.total_point_have_displayed);

             list_for_each(pos, &wifi_list) {
             struct one_wifi_hot_point *temp = list_entry(pos, struct one_wifi_hot_point, list);
              free(temp );
             }
            list_del_init(&wifi_list);

            if(wifi_hot_point_display.total_point_have_displayed<wifi_hot_point_display.total_point_can_be_display)
            {
                 int wifi_hot_point_not_diplay_number;
                 wifi_hot_point_not_diplay_number=wifi_hot_point_display.total_point_can_be_display-\
                 wifi_hot_point_display.total_point_have_displayed;
                surface = wifi_window.surface;
                //get the start of the none display area
                wifi_hot_point_display.current_display_position_y=MENU_HEIGHT+\
                                      wifi_hot_point_display.total_point_have_displayed*wifi_hot_point_display.one_point_display_height;

                DFBCHECK(surface->SetColor(surface,
                                  color_table[item_init_bgcolor].r,
                                  color_table[item_init_bgcolor].g,
                                  color_table[item_init_bgcolor].b, 0xff));
                //because we have a line at coordinate(0,0)-(0,height),so ,let's clear from 1
                DFBCHECK(surface->FillRectangle(surface,1,\
                    wifi_hot_point_display.current_display_position_y,wifi_hot_point_display.one_point_display_width,\
                    wifi_hot_point_not_diplay_number*wifi_hot_point_display.one_point_display_height));
                surface->Flip(surface, NULL, 0);

            }
            memset(&wifi_hot_point_display,0,sizeof(struct wifi_hot_point_display_t));

            wifi_hot_point_display.current_display_position_x=4;
            wifi_hot_point_display.current_display_position_y=MENU_HEIGHT;
            wifi_hot_point_display.one_point_display_width   =wifi_window_menu.width;
            wifi_hot_point_display.one_point_display_height  =ITEM_HEIGHT;
            wifi_hot_point_display.total_point_have_displayed=0;
            wifi_hot_point_display.total_point_searched      =0;
            wifi_hot_point_display.total_point_can_be_display=(wifi_window.desc.height-\
            wifi_window_menu.height)/wifi_hot_point_display.one_point_display_height;
            goto get_wifi_point_data;

        }


       ssid=strtok(buffer, ":");
       single_level_db=strtok(NULL, "\n");
       if(ssid==NULL||single_level_db==NULL){
        goto get_wifi_point_data;
       }
         //add for wifi hot point.
        surface = wifi_window.surface;

        list_for_each(pos, &wifi_list) {
        p_hot_point = list_entry(pos, struct one_wifi_hot_point, list);

        if (0==strcmp(p_hot_point->ssid,ssid)) {//cmp the ssid
               // wifi hot point exist in the list ,so return.
              // db_msg("wifi:%s already exist\n",p_hot_point->ssid);
               goto get_wifi_point_data;
            }
        }
       //wifi hot point not exist in the list ,so add it
        wifi_hot_point_display.total_point_searched++;
       if(wifi_hot_point_display.total_point_have_displayed>=wifi_hot_point_display.total_point_can_be_display)
       {
           goto get_wifi_point_data; //  display space now isn't enough
       }

       wifi_hot_point_display.total_point_have_displayed++;
       p_hot_point= malloc(sizeof(struct one_wifi_hot_point));
       strcpy(p_hot_point->ssid,ssid);
       strcpy(p_hot_point->single_level_db,single_level_db);

       single=atoi(single_level_db);
       if(single<0  &&single>=-55) p_hot_point->single_level=5;
       if(single<-55&&single>=-60) p_hot_point->single_level=4;
       if(single<-60&&single>=-65) p_hot_point->single_level=3;
       if(single<-65&&single>=-70) p_hot_point->single_level=2;
       if(single<-70&&single>=-75) p_hot_point->single_level=1;
       if(p_hot_point->single_level>5||p_hot_point->single_level<1){
            p_hot_point->single_level=1;
       }
     //  db_msg("wifi:ssid=%s,single_level_db=%s\n",wifi_hot_point->ssid,wifi_hot_point->single_level_db);
       p_hot_point->bgcolor=item_init_bgcolor;
       p_hot_point->fgcolor=item_init_fgcolor;
       p_hot_point->x      =wifi_hot_point_display.current_display_position_x;
       p_hot_point->y      =wifi_hot_point_display.current_display_position_y;
       p_hot_point->width  =wifi_hot_point_display.one_point_display_width;
       p_hot_point->height=wifi_hot_point_display.one_point_display_height;
       list_add_tail(&(p_hot_point->list), &wifi_list);

       DFBCHECK(surface->SetColor(surface,
                                  color_table[p_hot_point->bgcolor].r,
                                  color_table[p_hot_point->bgcolor].g,
                                  color_table[p_hot_point->bgcolor].b, 0xff));
      DFBCHECK(surface->FillRectangle(surface, p_hot_point->x, p_hot_point->y,\
        p_hot_point->width, p_hot_point->height));

       DFBCHECK(surface->SetFont(surface, font24));

       DFBCHECK(surface->SetColor(surface,
                                  color_table[p_hot_point->fgcolor].r,
                                  color_table[p_hot_point->fgcolor].g,
                                  color_table[p_hot_point->fgcolor].b, 0xff));


       //draw ssid
       DFBCHECK(surface->DrawString(surface,p_hot_point->ssid, -1,p_hot_point->x,
                   p_hot_point->y + p_hot_point->height-FONT24_HEIGHT/4 , DSTF_LEFT));
       //draw single level dB
       sprintf(buffer, "%s dB",p_hot_point->single_level_db);
       DFBCHECK(surface->DrawString(surface,buffer, -1,p_hot_point->x+(wifi_window.desc.width>>1),
                   p_hot_point->y + p_hot_point->height-FONT24_HEIGHT/4 , DSTF_LEFT));
       //draw single level

      // db_msg("wifi_window.desc.width=%d\n",wifi_window.desc.width);

       current_pozition_x=(wifi_window.desc.width*2)/3;
      // db_msg("current_pozition_x=%d\n",current_pozition_x);
       one_level_width=((wifi_window.desc.width-current_pozition_x)/5)-(wifi_window.desc.width>>7);
       if(single>=-60){
             DFBCHECK(surface->SetColor(surface,
                          color_table[COLOR_GREEN_IDX].r,
                          color_table[COLOR_GREEN_IDX].g,
                          color_table[COLOR_GREEN_IDX].b, 0xff));
          }
        else
          {
               DFBCHECK(surface->SetColor(surface,
                          color_table[COLOR_RED_IDX].r,
                          color_table[COLOR_RED_IDX].g,
                          color_table[COLOR_RED_IDX].b, 0xff));

         }
       for(single=0;single<p_hot_point->single_level;single++){


           DFBCHECK(surface->FillRectangle(surface, current_pozition_x, p_hot_point->y+\
            (p_hot_point->height>>2),\
            one_level_width, p_hot_point->height>>1));
            current_pozition_x+=one_level_width;

       }
       wifi_hot_point_display.current_display_position_y+=wifi_hot_point_display.one_point_display_height;

       DFBCHECK(surface->SetColor(surface,
                          color_table[COLOR_WHITE_IDX].r,
                          color_table[COLOR_WHITE_IDX].g,
                          color_table[COLOR_WHITE_IDX].b, 0xff));
       wifi_window.surface->DrawLine(surface,0,wifi_hot_point_display.current_display_position_y-1,\
                   wifi_window.desc.width,wifi_hot_point_display.current_display_position_y-1);

       surface->Flip(surface, NULL, 0);

      // db_msg("wifi:%s add\n",wifi_hot_point->ssid);

    }

}
static int wifi_window_init(void)
{
    pthread_t tid;
    int ret;
    db_debug("wifi list window init...\n");

    wifi_window.desc.flags  = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT |
                                 DWDESC_CAPS);
    if(mic_activated && MIC_WINDOW_SHOW){
     wifi_window.desc.posx   = (layer_config.width >> 1)+(layer_config.width>>MIC_POWER_BAR_WITH_SHIFT);
     wifi_window.desc.width  = (layer_config.width >> 1)-(layer_config.width>>MIC_POWER_BAR_WITH_SHIFT);
    }else
    {
     wifi_window.desc.posx   = mic1_window.desc.width + mic2_window.desc.width+misc_window.desc.width;
     wifi_window.desc.width  = layer_config.width- misc_window.desc.width - mic1_window.desc.width;
    }

    if(camera_activated){
        wifi_window.desc.posy   = layer_config.height >> 1;
        wifi_window.desc.height = (layer_config.height >> 1);
    }else
    {
        wifi_window.desc.posy   = manual_window.desc.height+auto_window.desc.height;
        wifi_window.desc.height = layer_config.height;
    }

    wifi_window.desc.caps   = DWCAPS_ALPHACHANNEL;

    DFBCHECK(layer->CreateWindow(layer, &wifi_window.desc, &wifi_window.window));
    wifi_window.window->GetSurface(wifi_window.window, &wifi_window.surface);
    wifi_window.window->SetOpacity(wifi_window.window, 0xff);
    wifi_window.window->SetOptions(wifi_window.window, DWOP_KEEP_POSITION);

    wifi_window.bgcolor = item_init_bgcolor;
    DFBCHECK(wifi_window.surface->SetColor(wifi_window.surface,
                                             color_table[wifi_window.bgcolor].r,
                                             color_table[wifi_window.bgcolor].g,
                                             color_table[wifi_window.bgcolor].b, 0xff));
    DFBCHECK(wifi_window.surface->FillRectangle(wifi_window.surface, 0, 0,
                wifi_window.desc.width, wifi_window.desc.height));

    /* draw menu */
    if (script_fetch("df_view", "wifi_menu_name", (int *)wifi_window_menu.name, 8)) {
        strcpy(wifi_window_menu.name, WIFI_MENU_NAME);
    }

    wifi_window_menu.width  = wifi_window.desc.width;
    wifi_window_menu.height = MENU_HEIGHT;

    if (script_fetch("df_view", "menu_bgcolor", &wifi_window_menu.bgcolor, 1) ||
            wifi_window_menu.bgcolor < COLOR_WHITE_IDX || wifi_window_menu.bgcolor > COLOR_BLACK_IDX) {
        wifi_window_menu.bgcolor = COLOR_YELLOW_IDX;
    }

    if (script_fetch("df_view", "menu_fgcolor", &wifi_window_menu.fgcolor, 1) ||
            wifi_window_menu.fgcolor < COLOR_WHITE_IDX || wifi_window_menu.fgcolor > COLOR_BLACK_IDX) {
        wifi_window_menu.fgcolor = COLOR_BLACK_IDX;
    }

    wifi_window_menu.bgcolor=COLOR_BEAUTY_IDX;
    wifi_window_menu.fgcolor=COLOR_WHITE_IDX;
    DFBCHECK(wifi_window.surface->SetColor(wifi_window.surface,
                                             color_table[wifi_window_menu.bgcolor].r,
                                             color_table[wifi_window_menu.bgcolor].g,
                                             color_table[wifi_window_menu.bgcolor].b, 0xff));
    DFBCHECK(wifi_window.surface->FillRectangle(wifi_window.surface, 0, 0, wifi_window_menu.width, wifi_window_menu.height));
    DFBCHECK(wifi_window.surface->SetFont(wifi_window.surface, font48));
    DFBCHECK(wifi_window.surface->SetColor(wifi_window.surface,
                                             color_table[wifi_window_menu.fgcolor].r,
                                             color_table[wifi_window_menu.fgcolor].g,
                                             color_table[wifi_window_menu.fgcolor].b, 0xff));
    DFBCHECK(wifi_window.surface->DrawString(wifi_window.surface, wifi_window_menu.name, -1, 4,
                MENU_HEIGHT / 2 + FONT48_HEIGHT / 2 - FONT48_HEIGHT / 8, DSTF_LEFT));

    wifi_window.surface->DrawLine(wifi_window.surface,0,0,0,wifi_window.desc.height);
    wifi_window.surface->Flip(wifi_window.surface, NULL, 0);

    memset(&wifi_hot_point_display,0,sizeof(struct wifi_hot_point_display_t));
    wifi_hot_point_display.current_display_position_x=4;
    wifi_hot_point_display.current_display_position_y=MENU_HEIGHT;
    wifi_hot_point_display.one_point_display_width   =wifi_window_menu.width;
    wifi_hot_point_display.one_point_display_height  =ITEM_HEIGHT;
    wifi_hot_point_display.total_point_can_be_display=(wifi_window.desc.height-\
    wifi_window_menu.height)/wifi_hot_point_display.one_point_display_height;


     /* create named pipe */
    unlink(WIFI_PIPE_NAME);
    if (mkfifo(WIFI_PIPE_NAME,S_IFIFO | 0666) == -1) {
        db_error("core: mkfifo error(%s)\n", strerror(errno));
        return -1;
    }
    ret = pthread_create(&tid, NULL, wifi_hot_point_proccess, NULL);
     if (ret != 0) {
         db_error("df_view: create wifi hot point proccess thread failed\n");
     }

    return 0;
}

static int misc_window_init(void)
{
    int rwidth, gwidth, bxpos, bwidth;

    db_debug("misc window init...\n");

    misc_window.desc.flags  = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT |
                               DWDESC_CAPS);

    misc_window.desc.posx   = mic1_window.desc.width + mic2_window.desc.width;
    misc_window.desc.posy   = manual_window.desc.height+auto_window.desc.height;
    if(misc_window.desc.posy>=layer_config.height){
        db_msg("no enough space to misc window\n");
        return 0;
    }
    if(mic_activated && MIC_WINDOW_SHOW){
         misc_window.desc.width  = (layer_config.width >> 1)-(layer_config.width>>MIC_POWER_BAR_WITH_SHIFT);
    }
    else
    {
         misc_window.desc.width  = layer_config.width >> 1;
    }
    misc_window.desc.height = layer_config.height-misc_window.desc.posy;
    misc_window.desc.caps   = DWCAPS_ALPHACHANNEL;

    DFBCHECK(layer->CreateWindow(layer, &misc_window.desc, &misc_window.window));
    misc_window.window->GetSurface(misc_window.window, &misc_window.surface);
    misc_window.window->SetOpacity(misc_window.window, 0xff);
    misc_window.window->SetOptions(misc_window.window, DWOP_KEEP_POSITION);
    #if 1
    // draw RGB
    rwidth = gwidth = misc_window.desc.width / 3;
    bxpos = rwidth + gwidth;
    bwidth = misc_window.desc.width - bxpos;
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface, 0xff, 0, 0, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, 0, 0, rwidth, misc_window.desc.height));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface, 0, 0xff, 0, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, rwidth, 0, gwidth, misc_window.desc.height));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface, 0, 0, 0xff, 0xff));
    DFBCHECK(misc_window.surface->FillRectangle(misc_window.surface, bxpos, 0, bwidth, misc_window.desc.height));
    #else
    int i,h_d4;
    h_d4=misc_window.desc.height/3;
    /*
    for(i=0;i<misc_window.desc.width;i++){
      DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                               i,
                                               i,
                                               i, 0xff));
      DFBCHECK(misc_window.surface->DrawLine(misc_window.surface,i,
                                                                      0,
                                                                      h_d4,
                                                                      1));

      //  line(i,0,h_d4,1,color(0xff,i,i,i),bf);
    }*/
	for(i=0;i<misc_window.desc.width;i++){

      DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                                 i,
                                                 0,
                                                 0, 0xff));
      DFBCHECK(misc_window.surface->DrawLine(misc_window.surface,i,
                                                                        h_d4,
                                                                        h_d4,
                                                                        1));

     //   line(i,h_d4,h_d4,1,color(0xff,i,0,0),bf);
	}
	for(i=0;i<misc_window.desc.width;i++){

      DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                                 0,
                                                 i,
                                                 0, 0xff));
      DFBCHECK(misc_window.surface->DrawLine(misc_window.surface,i,
                                                                        h_d4*2,
                                                                        h_d4,
                                                                        1));
     //   line(i,h_d4*2,h_d4,1,color(0xff,0,i,0),bf);
	}
	for(i=0;i<misc_window.desc.width;i++){

      DFBCHECK(misc_window.surface->SetColor(misc_window.surface,
                                                 0,
                                                 0,
                                                 i, 0xff));
      DFBCHECK(misc_window.surface->DrawLine(misc_window.surface,i,
                                                                        h_d4*3,
                                                                        h_d4,
                                                                        1));
      //  line(i,h_d4*3,h_d4,1,color(0xff,0,0,i),bf);
    }
    #endif
    /* draw copryright */
    DFBCHECK(misc_window.surface->SetFont(misc_window.surface, font24));
    DFBCHECK(misc_window.surface->SetColor(misc_window.surface, 0, 0, 0, 0xff));
    DFBCHECK(misc_window.surface->DrawString(misc_window.surface, COPYRIGHT, -1, misc_window.desc.width - 4,
                misc_window.desc.height - FONT24_HEIGHT / 4, DSTF_RIGHT));

    misc_window.surface->Flip(misc_window.surface, NULL, 0);

    return 0;
}

void update_mic_power_bar(struct mic_power_bar_t *mic_power_bar)
{
    float percent_of_power;
    int power_height;
    percent_of_power=(float)((float)(abs(mic_power_bar->v))/((mic_power_bar->Max)-(mic_power_bar->Min)));
    power_height=percent_of_power*mic_power_bar->height;
    //set the front color

    DFBCHECK(mic_power_bar->mic_window->surface->SetColor(mic_power_bar->mic_window->surface,
                                         color_table[mic_power_bar->BarColor[1]].r,
                                         color_table[mic_power_bar->BarColor[1]].g,
                                         color_table[mic_power_bar->BarColor[1]].b, 0xff));

    //draw the front part
    DFBCHECK(mic_power_bar->mic_window->surface->FillRectangle(mic_power_bar->mic_window->surface,\
                                            mic_power_bar->XOff,\
                                            mic_power_bar->height-power_height,
                                            mic_power_bar->width,
                                            mic_power_bar->height));

   DFBCHECK(mic_power_bar->mic_window->surface->SetColor(mic_power_bar->mic_window->surface,
                                              color_table[mic_power_bar->BarColor[0]].r,
                                              color_table[mic_power_bar->BarColor[0]].g,
                                              color_table[mic_power_bar->BarColor[0]].b, 0xff));
    //draw the back part
    DFBCHECK(mic_power_bar->mic_window->surface->FillRectangle(mic_power_bar->mic_window->surface,\
                                            mic_power_bar->XOff,\
                                            mic_power_bar->YOff,
                                            mic_power_bar->width,
                                            mic_power_bar->height-power_height));

   //draw the bar name
   DFBCHECK(mic_power_bar->mic_window->surface->SetColor(mic_power_bar->mic_window->surface,
                                          color_table[COLOR_WHITE_IDX].r,
                                          color_table[COLOR_WHITE_IDX].g,
                                          color_table[COLOR_WHITE_IDX].b, 0x10));
   DFBCHECK(mic_power_bar->mic_window->surface->DrawString(mic_power_bar->mic_window->surface,\
                                                mic_power_bar->bar_name, -1,\
                                                mic_power_bar->mic_window->desc.width>>1,
                                                mic_power_bar->mic_window->desc.height>>1, DSTF_CENTER));

    mic_power_bar->mic_window->surface->Flip(mic_power_bar->mic_window->surface, NULL, 0);


}


void* mic_audio_receive(void* argv)
{
    FILE *mic_pipe;
    char buffer[128];
    char *channel;
    printf("this is the mic_audio_receive thread...\n");
    mic_pipe = fopen(MIC_PIPE_NAME, "r");
    setlinebuf(mic_pipe);
while(1){
     if (fgets(buffer, sizeof(buffer), mic_pipe) == NULL) {
               continue;
       }
      channel= strtok(buffer, "#");
      if(channel==NULL) continue;
      mic1_power_bar.v=atoi(channel);
      channel = strtok(NULL, " \n");
      if(channel==NULL) continue;
      mic2_power_bar.v=atoi(channel);
#ifdef MIC1_WINDOW_SHOW
      update_mic_power_bar(&mic1_power_bar);
#endif
      update_mic_power_bar(&mic2_power_bar);
    }
    fclose(mic_pipe);

}
static int mic_window_init(void)
{
    pthread_t tid;
    int ret;

 //   if(!mic_activated || !MIC_WINDOW_SHOW)
//		return 0 ;
    db_debug("mic window init...\n");
#ifdef MIC1_WINDOW_SHOW
   mic1_window.desc.flags  = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT |
                              DWDESC_CAPS);
   mic1_window.desc.posx   = 0;
   mic1_window.desc.posy   = manual_window.desc.height+auto_window.desc.height;
   mic1_window.desc.width  = (layer_config.width >>MIC_POWER_BAR_WITH_SHIFT_TOTAL);
   mic1_window.desc.height = layer_config.height - (manual_window.desc.height+auto_window.desc.height);
   mic1_window.desc.caps   = DWCAPS_ALPHACHANNEL;
#endif
   mic2_window.desc.flags  = (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT |
                              DWDESC_CAPS);
   mic2_window.desc.posx   = misc_window.desc.width;
   mic2_window.desc.posy   = manual_window.desc.height+auto_window.desc.height;
   mic2_window.desc.width  = (layer_config.width >>MIC_POWER_BAR_WITH_SHIFT_TOTAL);
   mic2_window.desc.height = layer_config.height - (manual_window.desc.height+auto_window.desc.height);
   mic2_window.desc.caps   = DWCAPS_ALPHACHANNEL;

#ifdef MIC1_WINDOW_SHOW
   DFBCHECK(layer->CreateWindow(layer, &mic1_window.desc, &mic1_window.window));
   mic1_window.window->GetSurface(mic1_window.window, &mic1_window.surface);
   mic1_window.window->SetOpacity(mic1_window.window, 0xff);
   mic1_window.window->SetOptions(mic1_window.window, DWOP_KEEP_POSITION);
#endif
   DFBCHECK(layer->CreateWindow(layer, &mic2_window.desc, &mic2_window.window));
   mic2_window.window->GetSurface(mic2_window.window, &mic2_window.surface);
   mic2_window.window->SetOpacity(mic2_window.window, 0xff);
   mic2_window.window->SetOptions(mic2_window.window, DWOP_KEEP_POSITION);

#ifdef MIC1_WINDOW_SHOW
   //draw a rect around the window
   DFBCHECK(mic1_window.surface->SetColor(mic1_window.surface,
                                          color_table[COLOR_WHITE_IDX].r,
                                          color_table[COLOR_WHITE_IDX].g,
                                          color_table[COLOR_WHITE_IDX].b, 0xff));

   DFBCHECK(mic1_window.surface->DrawRectangle(mic1_window.surface,0,0,\
                                         mic1_window.desc.width,
                                         mic1_window.desc.height));
#endif
   DFBCHECK(mic2_window.surface->SetColor(mic2_window.surface,
                                          color_table[COLOR_WHITE_IDX].r,
                                          color_table[COLOR_WHITE_IDX].g,
                                          color_table[COLOR_WHITE_IDX].b, 0xff));

   DFBCHECK(mic2_window.surface->DrawRectangle(mic2_window.surface,0,0,\
                                         mic2_window.desc.width,
                                         mic2_window.desc.height));
#ifdef MIC1_WINDOW_SHOW
   mic1_power_bar.BarColor[0]=COLOR_BLACK_IDX;
   mic1_power_bar.BarColor[1]=COLOR_MAGENTA_IDX;
   mic1_power_bar.XOff       =1;
   mic1_power_bar.YOff       =1;
   mic1_power_bar.width      =mic1_window.desc.width-2;//except the rounded rectangle
   mic1_power_bar.height     =mic1_window.desc.height-2;//except the rounded rectangle
   mic1_power_bar.Min        =0; //will be change
   mic1_power_bar.Max        =32768;//will be change
   mic1_power_bar.v          =0;
   if(mic1_used){
        memcpy(mic1_power_bar.bar_name,"Mic1",sizeof("Mic1"));
   }
   else
   if(mic2_used)
   {
     memcpy(mic1_power_bar.bar_name,"Mic2",sizeof("Mic2"));
   }
   mic1_power_bar.mic_window =&mic1_window;
#endif

   mic2_power_bar.BarColor[0]=COLOR_BLACK_IDX;
   mic2_power_bar.BarColor[1]=COLOR_BEAUTY_IDX;
   mic2_power_bar.XOff       =1;
   mic2_power_bar.YOff       =1;
   mic2_power_bar.width      =mic2_window.desc.width-2;//except the rounded rectangle
   mic2_power_bar.height     =mic2_window.desc.height-2;//except the rounded rectangle
   mic2_power_bar.Min        =0; //will be change
   mic2_power_bar.Max        =32768;//will be change
   mic2_power_bar.v          =0;
   if(!mic2_used){
       memcpy(mic2_power_bar.bar_name,"Mic1",sizeof("Mic1"));
   }else
   {
        memcpy(mic2_power_bar.bar_name,"Mic2",sizeof("Mic2"));
   }

   mic2_power_bar.mic_window =&mic2_window;

#ifdef MIC1_WINDOW_SHOW
   //draw the mic1 words
   DFBCHECK(mic1_window.surface->SetFont(mic1_window.surface, font24));
   DFBCHECK(mic1_window.surface->SetColor(mic1_window.surface,
                                          color_table[COLOR_WHITE_IDX].r,
                                          color_table[COLOR_WHITE_IDX].g,
                                          color_table[COLOR_WHITE_IDX].b, 0x10));
   DFBCHECK(mic1_window.surface->DrawString(mic1_window.surface, mic1_power_bar.bar_name, -1,mic1_window.desc.width>>1,
                mic1_window.desc.height>>1, DSTF_CENTER));
#endif

   //draw the mic2 words
   DFBCHECK(mic2_window.surface->SetFont(mic2_window.surface, font24));
   DFBCHECK(mic2_window.surface->SetColor(mic2_window.surface,
                                          color_table[COLOR_WHITE_IDX].r,
                                          color_table[COLOR_WHITE_IDX].g,
                                          color_table[COLOR_WHITE_IDX].b, 0x10));
   DFBCHECK(mic2_window.surface->DrawString(mic2_window.surface,mic2_power_bar.bar_name, -1,mic2_window.desc.width>>1,
                mic2_window.desc.height>>1, DSTF_CENTER));

#ifdef MIC1_WINDOW_SHOW
   //flip the buffer to the screen
   mic1_window.surface->Flip(mic1_window.surface, NULL, 0);
#endif
   mic2_window.surface->Flip(mic2_window.surface, NULL, 0);


       /* create named pipe */
    unlink(MIC_PIPE_NAME);
    if (mkfifo(MIC_PIPE_NAME,S_IFIFO | 0666) == -1) {
        db_error("core: mkfifo error(%s)\n", strerror(errno));
        return -1;
    }
    ret = pthread_create(&tid, NULL, mic_audio_receive, NULL);
     if (ret != 0) {
         db_error("df_view: create sound play thread failed\n");
     }


   return 0;


}




static int df_font_init(void)
{
    DFBFontDescription font_desc;

    if (script_fetch("df_view", "font_size", &font_size, 1)) {
        printf("get font_size fail\n");
        font_size = DEFAULT_FONT_SIZE;
    }

    /* create font 16 pixel */
    font_desc.flags  = DFDESC_HEIGHT;
    font_desc.height = FONT16_HEIGHT;
    DFBCHECK(dfb->CreateFont(dfb, DATADIR"/wqy-zenhei.ttc", &font_desc, &font16));

    /* create font 20 pixel */
    font_desc.flags  = DFDESC_HEIGHT;
    font_desc.height = FONT20_HEIGHT;
    DFBCHECK(dfb->CreateFont(dfb, DATADIR"/wqy-zenhei.ttc", &font_desc, &font20));

    /* create font 24 pixel */
    font_desc.flags  = DFDESC_HEIGHT;
    font_desc.height = FONT24_HEIGHT;
    DFBCHECK(dfb->CreateFont(dfb, DATADIR"/wqy-zenhei.ttc", &font_desc, &font24));

    /* create font 28 pixel */
    font_desc.flags  = DFDESC_HEIGHT;
    font_desc.height = FONT28_HEIGHT;
    DFBCHECK(dfb->CreateFont(dfb, DATADIR"/wqy-zenhei.ttc", &font_desc, &font28));

    /* create font 48 pixel */
    font_desc.flags  = DFDESC_HEIGHT;
    font_desc.height = FONT48_HEIGHT;
    DFBCHECK(dfb->CreateFont(dfb, DATADIR"/wqy-zenhei.ttc", &font_desc, &font48));
    if (font_size == FONT24_HEIGHT) {
        font = font24;
    }else if(font_size == FONT28_HEIGHT){
        font = font28;
    }else if(font_size == FONT48_HEIGHT){
        font = font48;
    }else {
        font = font20;
    }

    return 0;
}

static int df_config_init(void)
{
    if(script_fetch("mic", "activated", &mic_activated,1)){
        mic_activated=0;
     }

   if (script_fetch("mic", "mic1_used", &mic1_used, 1)) {
        mic1_used=0;
        db_msg("core: can't fetch mic1_used, set to default: %d\n", mic1_used);

    }

    if (script_fetch("mic", "mic2_used", &mic2_used, 1)) {
        mic2_used=0;
        db_msg("core: can't fetch mic2_used, set to default: %d\n", mic2_used);

    }
    if(!mic1_used&&!mic2_used){

        mic1_used=1;
        mic2_used=1;
    }
    if (script_fetch("camera", "activated", &camera_activated, 1)){

       camera_activated=0;
    }

    if (script_fetch("df_view", "item_init_bgcolor", &item_init_bgcolor, 1) ||
            item_init_bgcolor < COLOR_WHITE_IDX ||
            item_init_bgcolor > COLOR_BLACK_IDX) {
        item_init_bgcolor = COLOR_WHITE_IDX;
    }

    if (script_fetch("df_view", "item_init_fgcolor", &item_init_fgcolor, 1) ||
            item_init_fgcolor < COLOR_WHITE_IDX ||
            item_init_fgcolor > COLOR_BLACK_IDX) {
        item_init_fgcolor = COLOR_BLACK_IDX;
    }

    if (script_fetch("df_view", "item_ok_bgcolor", &item_ok_bgcolor, 1) ||
            item_ok_bgcolor < COLOR_WHITE_IDX ||
            item_ok_bgcolor > COLOR_BLACK_IDX) {
        item_ok_bgcolor = COLOR_WHITE_IDX;
    }

    if (script_fetch("df_view", "item_ok_fgcolor", &item_ok_fgcolor, 1) ||
            item_ok_fgcolor < COLOR_WHITE_IDX ||
            item_ok_fgcolor > COLOR_BLACK_IDX) {
        item_ok_fgcolor = COLOR_BLUE_IDX;
    }

    if (script_fetch("df_view", "item_fail_bgcolor", &item_fail_bgcolor, 1) ||
            item_fail_bgcolor < COLOR_WHITE_IDX ||
            item_fail_bgcolor > COLOR_BLACK_IDX) {
        item_fail_bgcolor = COLOR_WHITE_IDX;
    }

    if (script_fetch("df_view", "item_fail_fgcolor", &item_fail_fgcolor, 1) ||
            item_fail_fgcolor < COLOR_WHITE_IDX ||
            item_fail_fgcolor > COLOR_BLACK_IDX) {
        item_fail_fgcolor = COLOR_RED_IDX;
    }

    if (script_fetch("df_view", "pass_str", (int *)pass_str,
                sizeof(pass_str) / 4 - 1)) {
        strcpy(pass_str, "Pass");
    }

    if (script_fetch("df_view", "fail_str", (int *)fail_str,
                sizeof(fail_str) / 4 - 1)) {
        strcpy(fail_str, "Fail");
    }

    return 0;
}

static int df_windows_init(void)
{
    unsigned int args[4];
    int disp;
    int fb;
    unsigned int layer_id = 0;
    int argc;
    char **argv;

    /* open /dev/disp */
    if ((disp = open("/dev/disp", O_RDWR)) == -1) {
        db_error("can't open /dev/disp(%s)\n", strerror(errno));
        return -1;
    }

    /* open /dev/fb0 */
    fb = open("/dev/fb0", O_RDWR);
#if 0
    if (ioctl(fb, FBIOGET_LAYER_HDL_0, &layer_id) < 0) {
        db_error("can't open /dev/fb0(%s)\n", strerror(errno));
        close(disp);
        return -1;
    }
#endif

    args[0] = 0;

	//DISP_CMD_GET_OUTPUT_TYPE = 0x09, according "linux-3.4\include\video\drv_display.h":698
    //disp_output_type_t = (disp_output_type)ioctl(disp, DISP_CMD_GET_OUTPUT_TYPE,(void*)args);
	disp_output_type_t=DISP_OUTPUT_TYPE_LCD;
	if (disp_output_type_t < 0) {
		db_error("Can't get line disp_output_type_t!!!\n");
	}
    db_msg("disp_output_type=%d\n",disp_output_type_t);

	#if 0
    if(disp_output_type_t==DISP_OUTPUT_TYPE_HDMI){
        disp_window	scn_window;
        disp_layer_info layer_para;
        unsigned int tv_scale_factor;
        if(0==script_fetch("df_view", "tv_scale_factor", &tv_scale_factor,1)){
            if(tv_scale_factor<50&&tv_scale_factor>100){
                db_msg("the tv_scale_factor isn't correct,set it to defult:no scale.\n");
                tv_scale_factor=100;
                }else
                {
                    db_msg("fetch tv_scale_factor successfully,tv_scale_factor=%d%\n",tv_scale_factor);
                }
             }else
             {
                db_msg("fetch the tv_scale_factor error,set it to defult: 100%\n");
                tv_scale_factor=100;};
                args[0]= 0;//screen 0
                args[1] = (__u32)layer_id;//layer number
                args[2] = (__u32)& layer_para;// paramerter
                //ioctl(disp, DISP_CMD_LAYER_GET_PARA,  (void*)args); //remove by ljq
                //set the screen window size use tv scale factor
                layer_para.scn_win.width=(unsigned int)(layer_para.src_win.width*(tv_scale_factor*0.01));
                layer_para.scn_win.height=(unsigned int)(layer_para.src_win.height*(tv_scale_factor*0.01));
                layer_para.scn_win.x=(unsigned int)(layer_para.src_win.width-layer_para.scn_win.width)>>1;
                layer_para.scn_win.y=(unsigned int)(layer_para.src_win.height-layer_para.scn_win.height)>>1;
                db_msg("src_win.x=%d,src_win.y=%d\n",layer_para.src_win.x,layer_para.src_win.y);
                db_msg("src_win.width=%d,src_win.height=%d\n",layer_para.src_win.width,layer_para.src_win.height);
                // set this layer to scaler mode,and scale.
                layer_para.mode=DISP_LAYER_WORK_MODE_SCALER;
                ioctl(disp, DISP_CMD_LAYER_SET_PARA,  (void*)args);
                //check if we set successfully
                args[0] = 0;
                args[1] = layer_id;
                args[2] = (__u32)& scn_window;
                ioctl(disp,DISP_CMD_LAYER_GET_SCN_WINDOW,(void*)args);
                db_msg("scn_win.x=%d,scn_win.y=%d\n",scn_window.x,scn_window.y);
                db_msg("scn_win.width=%d,scn_win.height=%d\n",scn_window.width,scn_window.height);
     }
	#endif
    /* set layer bottom */
    args[0] = 0;
    args[1] = layer_id;
#if defined (_SUN8IW6P) || (_SUN50IW1P) || (_SUN50IW3P)
    ioctl(disp, DISP_LAYER_BOTTOM, args);
#elif defined (_SUN8IW5P) || (_SUN9IW1P)
    ioctl(disp, DISP_CMD_LAYER_BOTTOM, args);
#endif

    close(disp);
    close(fb);

    /* init directfb */
    argc = 1;
    argv = malloc(sizeof(char *) * argc);
    argv[0] = "df_view";
    DFBCHECK(DirectFBInit(&argc, &argv));
    DFBCHECK(DirectFBCreate(&dfb));

    dfb->GetDeviceDescription(dfb, &gdesc);
    DFBCHECK(dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer));
    layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);

    if (!((gdesc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL) &&
                (gdesc.blitting_flags & DSBLIT_BLEND_COLORALPHA))) {
        layer_config.flags = DLCONF_BUFFERMODE;
        layer_config.buffermode = DLBM_BACKSYSTEM;

        layer->SetConfiguration(layer, &layer_config);
    }

    layer->GetConfiguration(layer, &layer_config);

    if(disp_output_type_t==DISP_OUTPUT_TYPE_LCD){

        layer->EnableCursor(layer, 1); // a83-f1 因为tp驱动的缘故，不显示鼠标，20140806

    }else
    {
        layer->EnableCursor(layer, 0);
    }

    db_msg("screen_width: %d, sceen_height: %d\n", layer_config.width, layer_config.height);

    /* init font */
    df_font_init();

    /* init config */
    df_config_init();

    /* init misc window befor video window */
    manual_window_init();
    auto_window_init();
   // video_window_init();
    misc_window_init();
    mic_window_init();
    wifi_window_init();

    if(disp_output_type_t==DISP_OUTPUT_TYPE_LCD){
        tp_track_init();
    }


    INIT_LIST_HEAD(&auto_tc_list);
    INIT_LIST_HEAD(&manual_tc_list);
    INIT_LIST_HEAD(&wifi_list);
#if 0
    printf("------------- to open video node\n");
    while((access("/dev/video0",F_OK)) == -1)
	sleep(1);
    video_window_init();
#endif
    return 0;
}

static int df_insert_item(int id, struct item_data *data)
{
    struct list_head *pos, *tc_list;
    IDirectFBSurface *surface;
    int window_height;
    int x, y;
    struct df_item_data *df_data;
    char item_str[132];

    if (data == NULL)
        return -1;

    if (data->category == CATEGORY_AUTO) {
        tc_list = &auto_tc_list;
        surface = auto_window.surface;
        window_height = auto_window.desc.height;
    }
    else if (data->category == CATEGORY_MANUAL) {
        tc_list = &manual_tc_list;
        surface = manual_window.surface;
        window_height = manual_window.desc.height;
    }
    else {
        db_error("unknown category of item: %s\n", data->name);
        return -1;
    }

    x = 0;
    y = MENU_HEIGHT;
    list_for_each(pos, tc_list) {
        struct df_item_data *temp = list_entry(pos, struct df_item_data, list);
        y += temp->height;
    }

    if (y + ITEM_HEIGHT > window_height) {
        db_error("no more space for item: %s\n", data->name);
        return -1;
    }

    df_data = malloc(sizeof(struct df_item_data));
    if (df_data == NULL)
        return -1;

    df_data->id = id;
    strncpy(df_data->name, data->name, 32);
    strncpy(df_data->display_name, data->display_name, 64);
    df_data->category = data->category;
    df_data->status = data->status;
    if (data->exdata[0]) {
        strncpy(df_data->exdata, data->exdata, 64);
        snprintf(item_str, 128, "%s %s", df_data->display_name, df_data->exdata);
    }
    else {
        snprintf(item_str, 128, "%s", df_data->display_name);
    }

    df_data->x      = x;
    df_data->y      = y;
    df_data->width  = auto_window.desc.width;
    df_data->height = ITEM_HEIGHT;

    df_data->bgcolor = item_init_bgcolor;
    df_data->fgcolor = item_init_fgcolor;

#if 0
    db_debug("test case: %s\n", df_data->name);
    db_debug("        x: %d\n", df_data->x);
    db_debug("        y: %d\n", df_data->y);
    db_debug("    width: %d\n", df_data->width);
    db_debug("   height: %d\n", df_data->height);
#endif

    list_add(&df_data->list, tc_list);

    DFBCHECK(surface->SetColor(surface,
                               color_table[df_data->bgcolor].r,
                               color_table[df_data->bgcolor].g,
                               color_table[df_data->bgcolor].b, 0xff));
    DFBCHECK(surface->FillRectangle(surface, df_data->x, df_data->y, df_data->width, df_data->height));
    DFBCHECK(surface->SetFont(surface, font));
    DFBCHECK(surface->SetColor(surface,
                               color_table[df_data->fgcolor].r,
                               color_table[df_data->fgcolor].g,
                               color_table[df_data->fgcolor].b, 0xff));
    DFBCHECK(surface->DrawString(surface, item_str, -1, df_data->x + 4,
                df_data->y + df_data->height - font_size / 4, DSTF_LEFT));

    surface->Flip(surface, NULL, 0);

    return 0;
}

static int df_update_item(int id, struct item_data *data)
{
    struct list_head *pos, *tc_list;
    IDirectFBSurface *surface;
    struct df_item_data *df_data;
    char item_str[132];

    if (data == NULL)
        return -1;

    if (data->category == CATEGORY_AUTO) {
        tc_list = &auto_tc_list;
        surface = auto_window.surface;
    }
    else if (data->category == CATEGORY_MANUAL) {
        tc_list = &manual_tc_list;
        surface = manual_window.surface;
    }
    else {
        db_error("unknown category of item: %s\n", data->name);
        return -1;
    }

    df_data = NULL;
    list_for_each(pos, tc_list) {
        struct df_item_data *temp = list_entry(pos, struct df_item_data, list);
        if (temp->id == id) {
            df_data = temp;
            break;
        }
    }

    if (df_data == NULL) {
        db_error("no such test case id #%d, name: %s\n", id, data->name);
        return -1;
    }

    df_data->status = data->status;
    if (df_data->status < 0) {
        df_data->bgcolor = item_init_bgcolor;
        df_data->fgcolor = item_init_fgcolor;
    }
    else if (df_data->status == 0) {
        df_data->bgcolor = item_ok_bgcolor;
        df_data->fgcolor = item_ok_fgcolor;
    }
    else {
        df_data->bgcolor = item_fail_bgcolor;
        df_data->fgcolor = item_fail_fgcolor;
    }

    if (data->exdata[0]) {
        strncpy(df_data->exdata, data->exdata, 64);
        if (df_data->status < 0) {
            snprintf(item_str, 128, "%s: %s", df_data->display_name,
                                              df_data->exdata);
        }
        else if (df_data->status == 0) {
            snprintf(item_str, 128, "%s: [%s] %s", df_data->display_name,
                                                   pass_str,
                                                   df_data->exdata);
        }
        else {
            snprintf(item_str, 128, "%s: [%s] %s", df_data->display_name,
                                                   fail_str,
                                                   df_data->exdata);
        }
    }
    else {
        if (df_data->status < 0) {
            snprintf(item_str, 128, "%s", df_data->display_name);
        }
        else if (df_data->status == 0) {
            snprintf(item_str, 128, "%s: [%s]", df_data->display_name,
                                                pass_str);
        }
        else {
            snprintf(item_str, 128, "%s: [%s]", df_data->display_name,
                                                fail_str);
        }
    }

#if 0
    db_debug("test case: %s\n", df_data->name);
    db_debug("        x: %d\n", df_data->x);
    db_debug("        y: %d\n", df_data->y);
    db_debug("    width: %d\n", df_data->width);
    db_debug("   height: %d\n", df_data->height);
#endif

    DFBCHECK(surface->SetColor(surface,
                               color_table[df_data->bgcolor].r,
                               color_table[df_data->bgcolor].g,
                               color_table[df_data->bgcolor].b, 0xff));
    DFBCHECK(surface->FillRectangle(surface, df_data->x, df_data->y, df_data->width, df_data->height));
    DFBCHECK(surface->SetFont(surface, font));
    DFBCHECK(surface->SetColor(surface,
                               color_table[df_data->fgcolor].r,
                               color_table[df_data->fgcolor].g,
                               color_table[df_data->fgcolor].b, 0xff));
    DFBCHECK(surface->DrawString(surface, item_str, -1, df_data->x + 4,
                df_data->y + df_data->height - font_size / 4, DSTF_LEFT));

    surface->Flip(surface, NULL, 0);

    return 0;

}

static int df_delete_item(int id)
{
    return -1;
}

static void df_sync(void)
{
    auto_window.surface->Flip(auto_window.surface, NULL, 0);
    manual_window.surface->Flip(manual_window.surface, NULL, 0);
}

static struct view_operations df_view_ops =
{
    .insert_item = df_insert_item,
    .update_item = df_update_item,
    .delete_item = df_delete_item,
    .sync        = df_sync
};

#ifndef _SUN9IW1P
static void button_handle(int x, int y)
{
    static int clear_press = 0;
    static int switch_press = 0;
    static int reboot_press = 0;
    static int poweroff_press = 0;
    static int count = 0;

    /* check clear button is press */
    if (x > clear_button.x && x < clear_button.x + clear_button.width &&
        y > clear_button.y && y < clear_button.y + clear_button.height) {
        if (clear_press) {
            clear_press = 0;
            tp_track_clear();
            clear_button_redraw(clear_button.bgcolor);
        }
        else {
            clear_button_redraw(COLOR_CYAN_IDX);
            clear_press = 1;
        }
    }
    else if (
             x > (misc_window.desc.posx + switch_button.x) &&
             x < (misc_window.desc.posx + switch_button.x + switch_button.width) &&
             y > (misc_window.desc.posy + switch_button.y) &&
             y < (misc_window.desc.posy + switch_button.y + switch_button.height)) {
        if (switch_press) {
	        count = 0;
            switch_press = 0;
#if 0
            switch_camera();
#endif
            switch_button_redraw(switch_button.bgcolor);
        }
        else {
           count++;
           if(count % 5 == 0) {
                switch_press = 1;
                switch_button_redraw(COLOR_CYAN_IDX);
           }
        }
    }
    else if (
             x > (misc_window.desc.posx + reboot_button.x) &&
             x < (misc_window.desc.posx + reboot_button.x + reboot_button.width) &&
             y > (misc_window.desc.posy + reboot_button.y) &&
             y < (misc_window.desc.posy + reboot_button.y + reboot_button.height)) {
        if (reboot_press) {
            reboot_press = 0;
            reboot_button_redraw(reboot_button.bgcolor);
            system("reboot");
        }
        else {
            reboot_press = 1;
            reboot_button_redraw(COLOR_CYAN_IDX);
        }
    }
    else if (
             x > (misc_window.desc.posx + poweroff_button.x) &&
             x < (misc_window.desc.posx + poweroff_button.x + poweroff_button.width) &&
             y > (misc_window.desc.posy + poweroff_button.y) &&
             y < (misc_window.desc.posy + poweroff_button.y + poweroff_button.height)) {
        if (poweroff_press) {
            poweroff_press = 0;
            poweroff_button_redraw(poweroff_button.bgcolor);
            system("poweroff");
        }
        else {
            poweroff_press = 1;
            poweroff_button_redraw(COLOR_CYAN_IDX);
        }
    }
    else {
	    count = 0;
        switch_press = 0;
        clear_press = 0;
    }
}
#endif

static void show_mouse_event(DFBInputEvent *evt)
{
    static int press = 0;
    static int tp_x = -1, tp_y = -1;
    static int flag = 0x00;

    int mouse_x = 0, mouse_y = 0;

    char buf[64];

    *buf = 0;
    if (evt->type == DIET_AXISMOTION) {
        if (evt->flags & DIEF_AXISABS) {
            switch (evt->axis) {
                case DIAI_X:
                    mouse_x = evt->axisabs;
                    break;
                case DIAI_Y:
                    mouse_y = evt->axisabs;
                    break;
                case DIAI_Z:
                    snprintf(buf, sizeof(buf), "Z axis (abs): %d", evt->axisabs);
                    break;
                default:
                    snprintf(buf, sizeof(buf), "Axis %d (abs): %d", evt->axis, evt->axisabs);
                    break;
            }
        }
        else if (evt->flags & DIEF_AXISREL) {
            switch (evt->axis) {
                case DIAI_X:
                    mouse_x += evt->axisrel;
                    break;
                case DIAI_Y:
                    mouse_y += evt->axisrel;
                    break;
                case DIAI_Z:
                    snprintf(buf, sizeof(buf), "Z axis (rel): %d", evt->axisrel);
                    break;
                default:
                    snprintf (buf, sizeof(buf), "Axis %d (rel): %d", evt->axis, evt->axisrel);
                    break;
            }
        }
        else {
            db_debug("axis: %x\n", evt->axis);
        }

        mouse_x = CLAMP(mouse_x, 0, layer_config.width  - 1);
        mouse_y = CLAMP(mouse_y, 0, layer_config.height - 1);
    }
    else {
         snprintf(buf, sizeof(buf), "Button %d", evt->button);
    }

    db_dump("x #%d, y #%d\n", mouse_x, mouse_y);
    if (buf[0]) {
        db_dump("mouse event: %s\n", buf);
    }

    if (mouse_x != 0) {
        flag |= 0x01;
        tp_x = mouse_x;
    }
    if (mouse_y != 0) {
        flag |= 0x02;
        tp_y = mouse_y;
    }

    /* handle button event */
    if (mouse_x == 0 && mouse_y == 0) {
        tp_data.status = 0;
        snprintf(tp_data.exdata, 64, "(%d, %d)", tp_x, tp_y);
        df_update_item(BUILDIN_TC_ID_TP, &tp_data);
#ifndef  _SUN9IW1P
        button_handle(tp_x, tp_y);
#endif
        if (press) {
            /* draw last point anyway */
            tp_track_draw(tp_x, tp_y, -1);
            flag &= ~0x03;
            press = 0;
        }
        else {
            /* draw first point if we have get x and y */
            if ((flag & 0x03) == 0x03) {
                tp_track_draw(tp_x, tp_y, press);
            }
            flag &= ~0x03;
            press = 1;
        }
    }
    else {
        /* draw this point if we have get x and y */
        if ((flag & 0x03) == 0x03) {
            tp_track_draw(tp_x, tp_y, press);
            flag &= ~0x03;
        }
    }
}

/*
 * event main loop
 */
static void *event_mainloop(void *args)
{
    DFBInputDeviceKeySymbol last_symbol = DIKS_NULL;

    while (1) {
        DFBInputEvent evt;

        DFBCHECK(events->WaitForEvent(events));
        while (events->GetEvent(events, DFB_EVENT(&evt)) == DFB_OK) {
            show_mouse_event(&evt);
        }

        if (evt.type == DIET_KEYRELEASE) {
            if ((last_symbol == DIKS_ESCAPE || last_symbol == DIKS_EXIT) &&
                    (evt.key_symbol == DIKS_ESCAPE || evt.key_symbol == DIKS_EXIT)) {
                db_debug("Exit event main loop...\n");
                break;
            }

            last_symbol = evt.key_symbol;
        }
    }

    return (void *)0;
}

static int buildin_tc_init(void)
{
    char name[32];
    char display_name[64];

    memset(&tp_data, 0, sizeof(struct item_data));
    strncpy(name, "tp", 32);
    if (script_fetch(name, "display_name", (int *)display_name, sizeof(display_name) / 4)) {
        strncpy(tp_data.display_name, TP_DISPLAY_NAME, 64);
    }
    else {
        strncpy(tp_data.display_name, display_name, 64);
    }

    tp_data.category = CATEGORY_MANUAL;
    tp_data.status   = -1;
    df_insert_item(BUILDIN_TC_ID_TP, &tp_data);

    return 0;
}

int df_view_init(void)
{
    int ret;

    db_msg("directfb view init...\n");
    df_windows_init();
    df_view_id = register_view("directfb", &df_view_ops);
    if (df_view_id == 0)
        return -1;
	disp_output_type_t=DISP_OUTPUT_TYPE_LCD;

	/* VR9  does not have touchpad,there is no need to run event mainloop */
#ifndef _SUN50IW3P
    if(disp_output_type_t==DISP_OUTPUT_TYPE_LCD){
         db_msg("buildin_tc_init...\n");
         buildin_tc_init();

          /* get touchscreen */
          DFBCHECK(dfb->GetInputDevice(dfb, 1, &tp_dev));
         /* create an event buffer for touchscreen */
          DFBCHECK(tp_dev->CreateEventBuffer(tp_dev, &events));

         /* create event mainloop */
          ret = pthread_create(&evt_tid, NULL, event_mainloop, NULL);
          if (ret != 0) {
                db_error("create event mainloop failed\n");
                unregister_view(df_view_id);
               return -1;
          }
    }
#endif
    return 0;
}

int df_view_exit(void)
{
    unregister_view(df_view_id);
    return 0;
}
