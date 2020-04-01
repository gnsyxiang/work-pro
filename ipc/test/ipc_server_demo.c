/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    socket_server.c
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
#include <unistd.h>

#include "parse_cmd.h"
#include "socket.h"

#define BUF_LEN (10)

void socket_cb(void *user, void *connection)
{
	link_ops_t *link_ops = get_link_ops();
	socket_t *socket = connection;

	char buf[BUF_LEN] = {0};
	link_ops->read(socket, buf, 3);

	printf("buf: %s \n", buf);
}

static int socket_server_test(void)
{
	socket_t *socket;
	link_ops_t *link_ops = get_link_ops();

	socket = link_ops->create_server("socket");

	link_ops->wait_for_connect(socket, NULL, socket_cb);

	return 0;
}

void socket_server_init(void)
{
	handle_test_cmd_t socket_server_test_cmd[] = {
		{"2", socket_server_test},
	};

	register_test_cmd(socket_server_test_cmd, ARRAY_NUM(socket_server_test_cmd));
}
