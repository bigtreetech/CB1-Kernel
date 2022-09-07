/*
 * \file        launcher.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年06月01日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "dragonboard.h"
#include "test_case.h"

struct no_wait_task
{
    pid_t pid;
    struct testcase_base_info *base_info;
    struct list_head list;
};

static struct list_head no_wait_task_list;
static FILE *cmd_pipe;

static void wait_completion(pid_t pid)
{
    int status;

    if (waitpid(pid, &status, 0) < 0) {
        db_error("launcher: waitpid(%d) failed(%s)\n", pid, strerror(errno));
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status)) {
        db_error("launcher: error in process %d(%d)\n", pid,
                WEXITSTATUS(status));
    }
}

static int run_one_testcase(int version, int script_shmid,
        struct testcase_base_info *info)
{
    char **args;
    pid_t pid;
    int i = 0;

    args = malloc(sizeof(char *) * 5);
    args[0] = info->binary;
    args[1] = malloc(10);
    sprintf(args[1], "%d", version);
    args[2] = malloc(10);
    sprintf(args[2], "%d", script_shmid);
    args[3] = malloc(10);
    sprintf(args[3], "%d", info->id);
    args[4] = NULL;

    pid = fork();
    if (pid < 0) {
        db_error("launcher: fork %s process failed(%s)\n", info->binary,
                strerror(errno));
	for (i = 0; i < 5;i++) {
		if(args[i]) {
			free(args[i]);
			args[i] = NULL;
		}
	}
	if (args) {
		free(args);
		args = NULL;
	}
        return -1;
    }
    else if (pid == 0) {
        db_msg("launcher: starting %s\n", info->binary);
        execvp(info->binary, args);
        fprintf(cmd_pipe, "%d 1\n", info->id);
        db_error("launcher: can't run %s(%s)\n", info->binary, strerror(errno));
        _exit(-1);
    }

    usleep(1000);

    /* now: we only wait auto test case due to system performance */
    if (info->run_type == WAIT_COMPLETION && info->category == CATEGORY_AUTO) {
        db_msg("launcher: wait %s completion\n", info->binary);
        wait_completion(pid);
    }
    else {
        struct no_wait_task *task;

        task = malloc(sizeof(struct no_wait_task));
        if (task == NULL)
            return -1;
        memset(task, 0, sizeof(struct no_wait_task));

        task->pid = pid;
        task->base_info = info;
        list_add(&task->list, &no_wait_task_list);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int version;
    int script_shmid;
    int i, total_testcases;
    int base_info_shmid;
    struct testcase_base_info *info;
    struct list_head *pos;

    if (argc < 5) {
        db_error("launcher: invalid parameters count #%d\n", argc);
        return -1;
    }

    db_msg("launcher: runing...\n");
    cmd_pipe = fopen(CMD_PIPE_NAME, "w");
    setlinebuf(cmd_pipe);

    INIT_LIST_HEAD(&no_wait_task_list);

    /* run all test cases */
    version = atoi(argv[1]);
    script_shmid = atoi(argv[2]);
    total_testcases = atoi(argv[3]);
    base_info_shmid = atoi(argv[4]);
    info = shmat(base_info_shmid, 0, 0);
    if (info == (void *)-1) {
        db_error("launcher: attach the share memory segment for test case "
                "basic information failed(%s)\n", strerror(errno));
        return -1;
    }

    if (total_testcases == 0) {
        db_error("launcher: no test cases will be launched\n");
        return -1;
    }

    db_debug("launcher: total test cases #%d\n", total_testcases);
    for (i = 0; i < total_testcases; i++) {
        if (info[i].binary[0] == '\0') {
            fprintf(cmd_pipe, "%d 1\n", info[i].id);
            continue;
        }

        run_one_testcase(version, script_shmid, &info[i]);
    }

    /* wait no wait task completion */
    list_for_each(pos, &no_wait_task_list) {
        struct no_wait_task *task = list_entry(pos, struct no_wait_task, list);
        db_msg("launcher: wait %s completion\n", task->base_info->binary);
        wait_completion(task->pid);
    }

    /* clean up */
    fprintf(cmd_pipe, "%s\n", TEST_COMPLETION);
    fclose(cmd_pipe);
    shmdt(info);

    return 0;
}
