/*
 * \file        test_case.h
 * \brief
 *
 * \version     1.0.0
 * \date        2012年06月01日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#ifndef __TEST_CASE_H__
#define __TEST_CASE_H__

#define CATEGORY_AUTO                   0
#define CATEGORY_MANUAL                 1
#define CATEGORY_WIFI                   2


#define WAIT_COMPLETION                 0
#define NO_WAIT_COMPLETION              1

struct testcase_base_info
{
    char name[32];
    char display_name[68];
    int activated;
    char binary[20];
    int id;
    int category; /* 0: auto, 1: manual */
    int run_type;
};

#define INIT_CMD_PIPE()                                         \
    FILE *cmd_pipe;                                             \
    int test_case_id;                                           \
    if (argc < 4) {                                             \
        db_error("%s: invalid parameter, #%d\n", argv[0], argc);\
        return -1;                                              \
    }                                                           \
    cmd_pipe = fopen(CMD_PIPE_NAME, "w");                       \
    setlinebuf(cmd_pipe);                                       \
    test_case_id = atoi(argv[3])

#define SEND_CMD_PIPE_OK()                                      \
    fprintf(cmd_pipe, "%d 0\n", test_case_id)

#define SEND_CMD_PIPE_OK_EX(exdata)                             \
    fprintf(cmd_pipe, "%d 0 %s\n", test_case_id, exdata)

#define SEND_CMD_PIPE_FAIL()                                    \
    fprintf(cmd_pipe, "%d 1\n", test_case_id)

#define SEND_CMD_PIPE_FAIL_EX(exdata)                           \
    fprintf(cmd_pipe, "%d 1 %s\n", test_case_id, exdata)

#define EXIT_CMD_PIPE()                                         \
    fclose(cmd_pipe)


#endif /* __TEST_CASE_H__ */
