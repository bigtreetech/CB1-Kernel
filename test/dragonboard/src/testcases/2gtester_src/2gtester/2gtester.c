/*
 * \file        2gtester.c
 * \brief       just an example.
 *
 * \version     1.0.0
 * \date        2012年11月20日
 * \author      martin <zhengjiewen@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

/* include system header */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include<termios.h>

/* include "dragonboard_inc.h" */
#include "dragonboard_inc.h"

#define MAX_AT_RESPONSE 0x1000
int device_fd;

static char s_ATBuffer[MAX_AT_RESPONSE];

static int writeline (const char *s)
{
    int cur = 0;
    int len = strlen(s);
    int written;

    if (device_fd < 0) {
        return -1;
    }


    /* the main string */
    while (cur < len) {
        do {
            written = write (device_fd, s + cur, len - cur);
        } while (written < 0 && errno == EINTR);

        if (written < 0) {

            return -1;
        }

        cur += written;
    }

    /* the \r  */

    do {
        written = write (device_fd, "\r" , 1);
    } while ((written < 0 && errno == EINTR) || (written == 0));

    if (written < 0) {
        db_error("2g tester:write \\r error\n");
        return -1;
    }

    return 0;
}

static const char *readline()
{
    int count;
    db_msg("prepare to readline\n");
      do{
            count = read(device_fd, s_ATBuffer,MAX_AT_RESPONSE);
            db_msg("readline count=%d\n",count);

        } while (count < 0 && errno == EINTR);

        if (count > 0){

            s_ATBuffer[count] = '\0';//mark the string end
        }
        else if (count<=0) {
            /* read error encountered or EOF reached */
            if(count == 0) {
                db_error("ATchannel: EOF reached\n");
            } else {
                db_error("ATchannel: read error %s\n", strerror(errno));
            }
            return NULL;
        }

    return s_ATBuffer;
}

//in setup_2g_device,we do three things:
//first: delay sometime to ensure the 2g device setup properly
//second: open the device node
//third: sent "AT+CSQ" command to query the single level and return it
int setup_2g_device(char *device_node_path,int setup_delay)
{
    char *p_cur;
    int sl;
    struct termios  ios;

    device_fd= open (device_node_path, O_RDWR);
    if(device_fd<0)
    {
        db_error("2gtester:open device error\n");
    }

   /* disable echo on serial ports */
   tcgetattr( device_fd, &ios );
   ios.c_lflag = 0;
   if(tcsetattr( device_fd, TCSANOW, &ios ))
   {
      db_error("set tty attr fail\n");
      return 0;
   }

    db_msg("sleep %d s to wait for hardware ready\n",setup_delay);

    sleep(setup_delay);//sleep befor hardware get ready
    readline(); //dump the  msg
    db_msg("2gtester dump readline: %s\n",s_ATBuffer);


    if(writeline("AT+CSQ")){ //WRITE single query command
        return 0;
    }

    sleep(1);
    if(!readline()){
        db_error("2gtester:no correct response for the AT+CSQ\n");
        return 0;
    }
    db_msg("2gtester AT+CSQ readline: %s\n",s_ATBuffer);
    p_cur=strstr(s_ATBuffer,"+CSQ:");
    if (*p_cur == NULL) {
        return 0;
    }

    p_cur+=5; //point to the rssi start position
    db_msg("p_cur=%s\n",p_cur);
    sl=atoi(p_cur);
    sl=(sl*2)-113;
    db_msg("sl=%d\n",sl);
    return sl;

}

int call_number_2g_device(char *call_number)
{
    char *cmd;
    char *p_cur;
    int try=0;
    asprintf(&cmd, "ATD%s;", call_number);
    if(writeline(cmd)){
        free(cmd);
        return -1;
    }
    free(cmd);

    sleep(1);

    while(try<3){
         if(!readline()){
             sleep(2);
              try++;
         }else
         {
            break;
         }
    }
    db_msg("2gtester ATD readline: %s\n",s_ATBuffer);
    if(try>=3) {
        db_error("2gtester:no correct response for the ATD\n");
        return -1;
    }
     p_cur=strstr(s_ATBuffer,"OK");
    if(p_cur)
    {
        return 0;
    }
    else
    {
        return -1;
    }

}

int  hangup_2g_device()
{
    char *cmd;
    asprintf(&cmd, "AT+CHUP");
    if(writeline(cmd)){
        free(cmd);
        return -1;
    }

    free(cmd);
    return 0;

}

/* C entry.
 *
 * \param argc the number of arguments.
 * \param argv the arguments.
 *
 * DO NOT CHANGES THE NAME OF PARAMETERS, otherwise your program will get
 * a compile error if you are using INIT_CMD_PIPE macro.
 */
int main(int argc, char *argv[])
{
    char buf[128];
    char device_node_path[256];
    char call_number[128];
    int single_level;
    int setup_delay;
    int call_time;

    /* init cmd pipe after local variable declaration */
    INIT_CMD_PIPE();

    init_script(atoi(argv[2]));

    db_msg("2g tester\n");

    if(script_fetch("2g","device_node",(int*)device_node_path,sizeof(device_node_path)/4)){

        db_warn("2gtester:can't fetch device_node,set to /dev/ttyUSB0\n");
        strcpy(device_node_path,"/dev/ttyUSB0");

    }
    db_msg("device_node=%s\n",device_node_path);
    if(script_fetch("2g","call_number",(int*)call_number,sizeof(call_number)/4)){

        db_warn("2gtester:can't fetch call_number,set to 10086\n");
        strcpy(call_number,"10086");

    }
    if(script_fetch("2g","setup_delay",&setup_delay,1)){

        db_warn("2gtester:can't fetch setup_delay,set to 30s\n");
        setup_delay=30;

    }

    if(script_fetch("2g","call_time",&call_time,1)){

        db_warn("2gtester:can't fetch call_time,set to 30s\n");
        call_time=30;

    }

    single_level=setup_2g_device(device_node_path,setup_delay);

    if(single_level==0)
    {
        SEND_CMD_PIPE_FAIL_EX("can't get single level");
        return -1;
    }
    db_msg("single level=%d\n",single_level);
    sprintf(buf,"single level=%d dB\n",single_level);
    SEND_CMD_PIPE_OK_EX(buf);

    //make a telephone call with the specified number
    //return: 0 means ok; -1 means fail;
    if(call_number_2g_device(call_number))
    {
        SEND_CMD_PIPE_FAIL_EX("can't make a call!");
        return -1;
    }
    //call time delay,you can speak th each other in this time
    sleep(call_time);
    //
    hangup_2g_device();
    /* send OK to core if test OK, otherwise send FAIL
     * by using SEND_CMD_PIPE_FAIL().
     */
    SEND_CMD_PIPE_OK_EX("test done");

    return 0;
}
