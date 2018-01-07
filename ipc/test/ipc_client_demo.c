/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    socket_client.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    28/12 2017 17:40
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        28/12 2017      create the file
 * 
 *     last modified: 28/12 2017 17:40
 */
#include <stdio.h>

#include "parse_cmd.h"

static int socket_client_test(void)
{
	return 0;
}

void socket_client_init(void)
{
	handle_test_cmd_t socket_client_test_cmd[] = {
		{"1", socket_client_test},
	};

	register_test_cmd(socket_client_test_cmd, ARRAY_NUM(socket_client_test_cmd));
}

