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
#include "socket.h"

#define BUF_LEN (10)

static int socket_client_test(void)
{
	socket_t *socket;
	link_ops_t *link_ops = get_link_ops();

	socket = link_ops->create_client("socket");

	link_ops->connect(socket, 3);

	char buf[BUF_LEN] = "abc";
	link_ops->write(socket, buf, sizeof(buf));

	return 0;
}

void socket_client_init(void)
{
	handle_test_cmd_t socket_client_test_cmd[] = {
		{"1", socket_client_test},
	};

	register_test_cmd(socket_client_test_cmd, ARRAY_NUM(socket_client_test_cmd));
}

