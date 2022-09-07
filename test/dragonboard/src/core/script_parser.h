/*
 * \file        script_parser.h
 * \brief
 *
 * \version     1.0.0
 * \date        2012年05月31日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#ifndef __SCRIPT_PARSER_H__
#define __SCRIPT_PARSER_H__

#define SCRIPT_VERSION0                 1
#define SCRIPT_VERSION1                 0
#define SCRIPT_VERSION2                 0

/*
 * init script. called before other function.
 * \param name the name of script.
 * \return the id of script share memory segment, on error -1 is returned.
 */
int parse_script(const char *name);

/*
 * deparse script.
 */
void deparse_script(int shmid);

#endif /* __SCRIPT_PARSER_H__ */
