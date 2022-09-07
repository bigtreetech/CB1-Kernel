/*
 * \file        core.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "dragonboard.h"
#include "test_case.h"
#include "view.h"
#include "script_parser.h"
#include "script.h"
#include "core.h"

#define SCRIPT_NAME                     "/boot/test_config.fex"

static int total_testcases = 0;
static int total_testcases_auto=0;
static int total_testcases_manual=0;
static struct testcase_base_info *base_info = NULL;
static int base_info_shmid;

static int accel_case_id = 0;
static int gyro_case_id = 0;
static int mag_case_id = 0;

int get_auto_testcases_number(void)
{
return total_testcases_auto;
}

int get_manual_testcases_number(void)
{
return total_testcases_manual;
}

static int parse_testcase()
{
    int i, j, mainkey_cnt;
    struct testcase_base_info *info;
    char mainkey_name[32], display_name[64], binary[16];
    int activated, category, run_type;
    int len;

    mainkey_cnt = script_mainkey_cnt();
    info = malloc(sizeof(struct testcase_base_info) * mainkey_cnt);
    if (info == NULL) {
        db_error("core: allocate memory for temporary test case basic "
                "information failed(%s)\n", strerror(errno));
        return -1;
    }
    memset(info, 0, sizeof(struct testcase_base_info) * mainkey_cnt);

    for (i = 0, j = 0; i < mainkey_cnt; i++) {
        memset(mainkey_name, 0, 32);
        script_mainkey_name(i, mainkey_name);

        if (script_fetch(mainkey_name, "display_name", (int *)display_name, 16))
            continue;

        if (script_fetch(mainkey_name, "activated", &activated, 1))
            continue;

        if (display_name[0] && activated == 1) {
            strncpy(info[j].name, mainkey_name, 32);
            strncpy(info[j].display_name, display_name, 64);
            info[j].activated = activated;

			if(!strcmp("gyro",info[j].name))
				gyro_case_id = j;
			if(!strcmp("gsensor",info[j].name))
				accel_case_id = j;
			if(!strcmp("compass",info[j].name))
				mag_case_id = j;

            if (script_fetch(mainkey_name, "program", (int *)binary, 4) == 0) {
                strncpy(info[j].binary, binary, 16);
            }

            info[j].id = j;

            if (script_fetch(mainkey_name, "category", &category, 1) == 0) {
                info[j].category = category;

                if(category==0){
                    total_testcases_auto++;
                 }else
                 {
                    total_testcases_manual++;
                 }
            }

            if (script_fetch(mainkey_name, "run_type", &run_type, 1) == 0) {
                info[j].run_type = run_type;
            }

            j++;
        }
    }
    total_testcases = j;

    db_msg("core: total test cases #%d\n", total_testcases);
    db_msg("core: total test cases_auto #%d\n", total_testcases_auto);
    db_msg("core: total test cases_manual #%d\n", total_testcases_manual);
    if (total_testcases == 0) {
	if (info) {
		free(info);
		info = NULL;
	}
        return 0;
    }

    len = sizeof(struct testcase_base_info) * total_testcases;
    base_info_shmid = shmget(IPC_PRIVATE, len, IPC_CREAT | 0666);
    if (base_info_shmid == -1) {
        db_error("core: allocate share memory segment for test case basic "
                "information failed(%s)\n", strerror(errno));
	if (info) {
		free(info);
		info = NULL;
	}
        return -1;
    }

    base_info = shmat(base_info_shmid, 0, 0);
    if (base_info == (void *)-1) {
        db_error("core: attach the share memory for test case basic "
                "information failed(%s)\n", strerror(errno));
        shmctl(base_info_shmid, IPC_RMID, 0);
	if (info) {
		free(info);
		info = NULL;
	}
        return -1;
    }
    memcpy(base_info, info, sizeof(struct testcase_base_info) *
            total_testcases);
	if (info) {
		free(info);
		info = NULL;
	}

    return total_testcases;
}

static void deparse_testcase(void)
{
    if (base_info) {
        shmdt(base_info);
        base_info = NULL;
    }

    total_testcases = 0;
}

static int draw_testcases(void)
{
    int i;
    struct item_data it_data;

    for (i = 0; i < total_testcases; i++) {
        memset(&it_data, 0, sizeof(struct item_data));
        strcpy(it_data.name, base_info[i].name);
        strcpy(it_data.display_name, base_info[i].display_name);
        it_data.category = base_info[i].category;
        it_data.status = -1;
        db_debug("core: draw test case: %s, display name: %s, category: %s\n",
                it_data.name, it_data.display_name,
                it_data.category ? "manual" : "auto");
        view_insert_item(base_info[i].id, &it_data);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int script_shmid;
    int ret;
    char **args;
    char binary[16] = "launcher";
    pid_t launcher_pid;
    char buffer[128];
    FILE *from_child;
    char *test_case_id_s;
    int test_case_id;
    char *result_s;
    int result;
    struct item_data it_data;
    char *exdata;
    int exdata_len;
    int i = 0;


    /* init script and view */
    db_msg("core: parse script %s...\n", SCRIPT_NAME);
    script_shmid = parse_script(SCRIPT_NAME);
    if (script_shmid == -1) {
        db_error("core: parse script failed\n");
        return -1;
    }

    db_msg("core: init script...\n");
    ret = init_script(script_shmid);
    if (ret) {
        db_error("core: init script failed(%d)\n", ret);
        return -1;
    }

    db_msg("core: init view...\n");

    /* parse and draw all test cases to view */
    db_msg("core: parse test case from script...\n");
    ret = parse_testcase();
    if (ret < 0) {
        db_error("core: parse all test case from script failed(%d)\n", ret);
        return -1;
    }
    else if (ret == 0) {
        db_warn("core: NO TEST CASE to be run\n");
        return -1;
    }

    ret = init_view();
    if (ret) {
        db_error("core: init view failed(%d)\n", ret);
        return -1;
    }

    db_msg("core: draw test case to view...\n");
    ret = draw_testcases();
    if (ret) {
        db_error("core: draw all test cases to view failed(%d)\n", ret);
        return -1;
    }
    view_sync();

    /* create named pipe */
    unlink(CMD_PIPE_NAME);
    if (mkfifo(CMD_PIPE_NAME, S_IFIFO | 0666) == -1) {
        db_error("core: mkfifo error(%s)\n", strerror(errno));
        return -1;
    }

    /* fork launcher process */
    db_msg("core: fork launcher process...\n");
    args = malloc(sizeof(char *) * 6);
    args[0] = binary;
    args[1] = malloc(10);
    sprintf(args[1], "%d", DRAGONBOARD_VERSION);
    args[2] = malloc(10);
    sprintf(args[2], "%d", script_shmid);
    args[3] = malloc(10);
    sprintf(args[3], "%d", total_testcases);
    args[4] = malloc(10);
    sprintf(args[4], "%d", base_info_shmid);
    args[5] = NULL;

    launcher_pid = fork();
    if (launcher_pid < 0) {
        db_error("core: fork launcher process failed(%s)\n", strerror(errno));
    }
    else if (launcher_pid == 0) {
        execvp(binary, args);
        db_error("core: can't run %s(%s)\n", binary, strerror(errno));
	for (i = 0; i < 6; i++) {
		if (args[i]) {
			free(args[i]);
			args[i] = NULL;
		}
	}
	if (args) {
		free(args);
		args = NULL;
	}
        _exit(-1);
    }

    /* listening to child process */
    db_msg("core: listening to child process, starting...\n");
    from_child = fopen(CMD_PIPE_NAME, "r");
    while (1) {
        if (fgets(buffer, sizeof(buffer), from_child) == NULL) {
            continue;
        }

        db_dump("core: command from child: %s", buffer);

        /* test completion */
        test_case_id_s = strtok(buffer, " \n");
        db_dump("test case id #%s\n", test_case_id_s);
        if (strcmp(buffer, TEST_COMPLETION) == 0)
            break;

        if (test_case_id_s == NULL)
            continue;
        test_case_id = atoi(test_case_id_s);

        result_s = strtok(NULL, " \n");
        db_dump("result: %s\n", result_s);
        if (result_s == NULL)
            continue;
        result = atoi(result_s);

        exdata = strtok(NULL, "\n");

        db_dump("%s TEST %s\n", base_info[test_case_id].name,
                (result == 0) ? "OK" : "Fail");

        /* update view item */
        memset(&it_data, 0, sizeof(struct item_data));

		if((test_case_id == 101  || test_case_id == 132) && accel_case_id != 0)
			test_case_id = accel_case_id;
		if(test_case_id == 106 && gyro_case_id != 0)
			test_case_id = gyro_case_id;
		if(test_case_id == 108 && mag_case_id != 0)
			test_case_id = mag_case_id;

		if(test_case_id > 100)
			continue;

        strncpy(it_data.name, base_info[test_case_id].name, 32);
        strncpy(it_data.display_name, base_info[test_case_id].display_name, 64);
        it_data.category = base_info[test_case_id].category;
        it_data.status = result;
        if (exdata) {
            /* trim space */
            while (*exdata == ' ' || *exdata == '\t')
                exdata++;
            exdata_len = strlen(exdata);
            exdata_len--;
            while (exdata >= 0 && (exdata[exdata_len] == ' ' ||
                   exdata[exdata_len] == '\t'))
                exdata_len--;

            if (exdata_len > 0) {
                exdata[++exdata_len] = '\0';
                db_dump("extra data len #%d: %s\n", exdata_len, exdata);
                strncpy(it_data.exdata, exdata, 64);
            }
        }
        view_update_item(test_case_id, &it_data);
    }
    fclose(from_child);

    db_msg("core: listening to child process, stoping...\n");

    deparse_testcase();
    exit_view();
    deinit_script();
    deparse_script(script_shmid);

    return 0;
}
