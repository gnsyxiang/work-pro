/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 16:39
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 16:39
 */
#include <stdio.h>

#include "parse_cmd.h"

static char usr_input[10];

void socket_client_init(void);
void socket_server_init(void);

void init(void)
{
	socket_client_init();
	socket_server_init();
}

void dis_func(void)
{
	printf("-----------------------------------------------\n");
	printf("enter the sequence number, select the function \n");
	printf("-----------------------------------------------\n");
	printf(" 1. socket client test \n");
	printf(" 2. socket server test \n");

	printf("input your number: ");
}

int main(int argc, const char *argv[])
{
	dis_func();

	init();

	scanf("%s", usr_input);

	match_test_cmd(usr_input);

    return 0;
}

