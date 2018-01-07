/**
 * Copyright (C) 2017 xxx Co., Ltd.
 * Release under GPLv2.
 * 
 * @file    socket.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/01 2018 13:20
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/01 2018      create the file
 * 
 *     last modified: 07/01 2018 13:20
 */
#ifndef IPC_SOCKET_H
#define IPC_SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

typedef void (*link_connect_callback_t)(void *user, void *connection);


struct socket {
    int fd;
    const char *name;
    pthread_mutex_t lock;
};

/**
 * for socket implement
 */
struct link_ops {
	void *(*create_client)(const char *name);
	void *(*create_server)(const char *name);
	void (*delete)(void *connection);

	int (*connect)(void *connection, int timeout);
	int (*wait_for_connect)(void *connection, void *user, link_connect_callback_t cb);

	int (*read)(void *connection, void *buf, int size);
	int (*write)(void *connection, const void *buf, int size);

	int (*get_fd)(void *connection);
};

struct link_ops *get_link_ops(void);

#ifdef __cplusplus
}
#endif

#endif
