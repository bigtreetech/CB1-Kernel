/*
**********************************************************************************************************************
*											        eGon
*						                     the Embedded System
*									       script parser sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : script.c
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include <malloc.h>
#include "types.h"
#include <string.h>
#include "script.h"
#include "crc.h"
#include "sunxi_mbr.h"
#include <ctype.h>
#include <unistd.h>

static  char  *script_mod_buf = NULL;           //指向第一个主键
static  int    script_main_key_count = 0;       //保存主键的个数

static  int   partition_start;
static  int   partition_next;

static  int   _test_str_length(char *str)
{
	int length = 0;

	while(str[length++])
	{
		if(length > 32)
		{
			length = 32;
			break;
		}
	}

	return length;
}
/*
************************************************************************************************************
*
*                                             script_parser_init
*
*    函数名称：
*
*    参数列表：script_buf: 脚本数据池
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int script_parser_init(char *script_buf)
{
	script_head_t   *script_head;

	if(script_buf)
	{
		script_mod_buf = script_buf;
		script_head = (script_head_t *)script_mod_buf;

		script_main_key_count = script_head->main_key_count;

		return SCRIPT_PARSER_OK;
	}
	else
	{
		printf("thi input script data buffer is null\n");

		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
}
/*
************************************************************************************************************
*
*                                             script_parser_exit
*
*    函数名称：
*
*    参数列表：NULL
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int script_parser_exit(void)
{
	script_mod_buf = NULL;
	script_main_key_count = 0;

	return SCRIPT_PARSER_OK;
}

/*
************************************************************************************************************
*
*                                             script_parser_fetch
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：根据传进的主键，子键，取得对应的数值
*
*
************************************************************************************************************
*/
int script_parser_fetch(char *main_name, char *sub_name, int value[])
{
	char   main_char[32], sub_char[32];
	script_main_key_t  *main_key;
	script_sub_key_t   *sub_key;
	int    i, j, k;
	int    pattern, word_count;

	//检查脚本buffer是否存在
	if(!script_mod_buf)
	{
		printf("the script data buffer is null, unable to parser\n");

		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
	//检查主键名称和子键名称是否为空
	if((main_name == NULL) || (sub_name == NULL))
	{
		printf("the input main key name or sub key name is null\n");

		return SCRIPT_PARSER_KEYNAME_NULL;
	}
	//检查数据buffer是否为空
	if(value == NULL)
	{
		printf("the input data value is null\n");

		return SCRIPT_PARSER_DATA_VALUE_NULL;
	}
	//保存主键名称和子键名称，如果超过16字节则截取16字节
	memset(main_char, 0, sizeof(main_char));
	memset(sub_char, 0, sizeof(sub_char));
	if(_test_str_length(main_name) <= 32)
	{
		strcpy(main_char, main_name);
	}
	else
	{
		strncpy(main_char, main_name, 31);
	}

	if(_test_str_length(sub_name) <= 32)
	{
		strcpy(sub_char, sub_name);
	}
	else
	{
		strncpy(sub_char, sub_name, 31);
	}

	for(i=0;i<script_main_key_count;i++)
	{
		main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + i * sizeof(script_main_key_t));
		if(strcmp(main_key->main_name, main_char))    //如果主键不匹配，寻找下一个主键
		{
			continue;
		}
		//主键匹配，寻找子键名称匹配
		for(j=0;j<main_key->lenth;j++)
		{
			sub_key = (script_sub_key_t *)(script_mod_buf + (main_key->offset<<2) + (j * sizeof(script_sub_key_t)));
			if(strcmp(sub_key->sub_name, sub_char))    //如果主键不匹配，寻找下一个主键
			{
				continue;
			}
			pattern    = (sub_key->pattern>>16) & 0xffff;             //获取数据的类型
			word_count = (sub_key->pattern>> 0) & 0xffff;             //获取所占用的word个数
			//取出数据
			switch(pattern)
			{
				case DATA_TYPE_SINGLE_WORD:                           //单word数据类型
					value[0] = *(int *)(script_mod_buf + (sub_key->offset<<2));
					break;

				case DATA_TYPE_STRING:     							  //字符串数据类型
					memcpy((char *)value, script_mod_buf + (sub_key->offset<<2), word_count << 2);
					break;

				case DATA_TYPE_GPIO_WORD:							 //多word数据类型
					k = 0;
					while(k < word_count)
					{
						value[k] = *(int *)(script_mod_buf + (sub_key->offset<<2) + (k<<2));
						k ++;
					}
					break;
			}

			return SCRIPT_PARSER_OK;
		}
	}

	return SCRIPT_PARSER_KEY_NOT_FIND;
}

/*
************************************************************************************************************
*
*                                             script_parser_fetch
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：根据传进的主键，子键，取得对应的数值
*
*
************************************************************************************************************
*/
int script_parser_fetch_partition(void)
{
	script_main_key_t  *main_key;
	int  i;

	//检查脚本buffer是否存在
	if(!partition_start)		//先找partition_start，然后找其后面的partition
	{
		for(i=0;i<script_main_key_count;i++)
		{
			main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + i * sizeof(script_main_key_t));
			if(strcmp(main_key->main_name, "partition_start"))    //如果主键不匹配，寻找下一个主键
			{
				continue;
			}
			else
			{
				partition_start = i;
				partition_next = partition_start;
				break;
			}
		}
		if(!partition_start)
		{
			printf("unable to find key partition_start\n");

			return -1;
		}
	}
	partition_next  ++;
	main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + partition_next * sizeof(script_main_key_t));
	if(strcmp(main_key->main_name, "partition"))
	{
		printf("this is not a partition key\n");

		return 0;
	}

	return partition_next;
}


/*
************************************************************************************************************
*
*                                             script_parser_fetch
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：根据传进的主键，子键，取得对应的数值
*
*
************************************************************************************************************
*/
int script_parser_fetch_mainkey_sub(char *sub_name, int index, int *value)
{
	char   sub_char[32];
	script_main_key_t  *main_key;
	script_sub_key_t  *sub_key;
	int    j, k;
	int    pattern, word_count;

	//检查脚本buffer是否存在
	if(!script_mod_buf)
	{
		printf("the script data buffer is null, unable to parser\n");

		return SCRIPT_PARSER_EMPTY_BUFFER;
	}
	//检查主键名称和子键名称是否为空
	if(sub_name == NULL)
	{
		printf("the input sub key name is null\n");

		return SCRIPT_PARSER_KEYNAME_NULL;
	}
	//保存主键名称和子键名称，如果超过16字节则截取16字节
	memset(sub_char, 0, sizeof(sub_char));
	if(_test_str_length(sub_name) <= 32)
	{
		strcpy(sub_char, sub_name);
	}

	main_key = (script_main_key_t *)(script_mod_buf + (sizeof(script_head_t)) + index * sizeof(script_main_key_t));
	if(strcmp(main_key->main_name, "partition"))    //如果主键不匹配，寻找下一个主键
	{
		printf("this is not a good partition key\n");

		return -1;
	}
	//主键匹配，寻找子键名称匹配
	for(j=0;j<main_key->lenth;j++)
	{
		sub_key = (script_sub_key_t *)(script_mod_buf + (main_key->offset<<2) + (j * sizeof(script_sub_key_t)));
		if(strcmp(sub_key->sub_name, sub_char))    //如果主键不匹配，寻找下一个主键
		{
			continue;
		}
		pattern    = (sub_key->pattern>>16) & 0xffff;             //获取数据的类型
		word_count = (sub_key->pattern>> 0) & 0xffff;             //获取所占用的word个数
		//取出数据
		switch(pattern)
		{
			case DATA_TYPE_SINGLE_WORD:                           //单word数据类型
				value[0] = *(int *)(script_mod_buf + (sub_key->offset<<2));
				break;

			case DATA_TYPE_STRING:     							  //字符串数据类型
				if(!word_count)
				{
					return 1;
				}
				memcpy((char *)value, script_mod_buf + (sub_key->offset<<2), word_count << 2);
				break;

			case DATA_TYPE_GPIO_WORD:							 //多word数据类型
				k = 0;
				while(k < word_count)
				{
					value[k] = *(int *)(script_mod_buf + (sub_key->offset<<2) + (k<<2));
					k ++;
				}
				break;
		}

		return SCRIPT_PARSER_OK;
	}

	return SCRIPT_PARSER_KEY_NOT_FIND;
}

