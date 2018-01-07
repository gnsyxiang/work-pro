/*
 *  Copyright (C) 2017 Ingenic Semiconductor
 *
 *  MaWeiBin <weibin.ma@ingenic.com>
 *
 *  Elf/IDWS Project
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <errno.h>

#include "log.h"
#include "socket.h"

static struct socket *create_socket_struct(
        const char *name, int fd) {
    struct socket *sok =
            (struct socket *)malloc(sizeof(struct socket));

    sok->fd = fd;
    sok->name = name;
    pthread_mutex_init(&sok->lock, NULL);

    return sok;
}

static void *create_socket_client(const char *name) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        log_e("%s: failed to create socket: %s(%d), name: %s\n",
                        __func__, strerror(errno), -errno, name);

        return NULL;
    }

    int ret = fcntl(fd, FD_CLOEXEC, 1);
    assert(ret == 0);

    return (void *)create_socket_struct(name, fd);
}

static void *create_socket_server(const char *name) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        log_e("%s: failed to create socket: %s(%d), name: %s\n",
                        __func__, strerror(errno), -errno, name);

        return NULL;
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, name);
    addr.sun_path[0] = 0;

    int addrLen = strlen(name) + offsetof(struct sockaddr_un, sun_path);
    int status = bind(fd, (const struct sockaddr *)&addr, addrLen);
    if (status < 0) {
        log_e("%s: failed to bind socket: %s(%d), name: %s\n",
                        __func__, strerror(errno), -errno, name);

        close(fd);
        return NULL;
    }

    return (void *)create_socket_struct(name, fd);
}

static void delete_socket(void *socket) {
    struct socket *sok = socket;

    close(sok->fd);
    pthread_mutex_destroy(&sok->lock);

    free(sok);
}

static int write_buffer(int fd, const char *buf, int size) {
    int offset = 0;
    int sz = size;

    while (sz > 0) {
        int bytes = write(fd, buf + offset, sz);
        if (bytes <= 0) {
//            log_d("%s: write error, size: %d, %s(%d)\n",
//                    __func__, size, strerror(errno), -errno);
            return bytes;
        }

        sz -= bytes;
        offset += bytes;
    }

    return size;
}

static int read_buffer(int fd, char *buf, int size) {
    int offset = 0;
    int sz = size;

    while (sz > 0) {
        int bytes = read(fd, buf + offset, sz);
        if (bytes <= 0) {
//            log_d("%s: read error, size: %d, %s(%d)\n",
//                    __func__, size, strerror(errno), -errno);
            return bytes;
        }

        sz -= bytes;
        offset += bytes;
    }

    return size;
}

static int socket_write(void *socket, const void *buf, int size) {
    struct socket *sok = socket;

    pthread_mutex_lock(&sok->lock);

    int ret = write_buffer(sok->fd, buf, size);

    pthread_mutex_unlock(&sok->lock);

    return ret;
}

static int socket_read(void *socket, void *buf, int size) {
    struct socket *sok = socket;

    pthread_mutex_lock(&sok->lock);

    int ret = read_buffer(sok->fd, buf, size);

    pthread_mutex_unlock(&sok->lock);

    return ret;
}

static int socket_connect(void *socket, int timeout) {
    struct socket *sok = socket;

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sok->name);
    addr.sun_path[0] = 0;

    int addrLen = strlen(sok->name) + offsetof(struct sockaddr_un, sun_path);
    int status;
    int time = 0;

    log_e("%s: wait for connect to server\n", sok->name);

    do {
        status = connect(sok->fd, (const struct sockaddr *)&addr, addrLen);
        if (status < 0) {
            sleep(1);
        }
    } while ((status < 0) && (time++ < timeout));

    if (status < 0)
        log_e("%s: wait for connect to server error !!!\n", sok->name);
    else
        log_e("%s: connect to server\n", sok->name);

    return 0;
}

static int socket_wait_for_connect(
        void *socket,
        void *user,
        link_connect_callback_t callback) {
    struct socket *sok = socket;
    int fd = sok->fd;

    int status = listen(fd, SOMAXCONN);
    if (status < 0) {
        log_e("%s: failed to listen socket: %s(%d), name: %s\n",
                __func__, strerror(errno), -errno, sok->name);

        return -1;
    }

    while (1) {
        fd_set read_fs;
        FD_ZERO(&read_fs);
        FD_SET(sok->fd, &read_fs);

        int ret = select(FD_SETSIZE, &read_fs, NULL, NULL, NULL);
        if (ret < 0) {
            log_e("%s: select error: %s(%d)\n",
                    __func__, strerror(errno), -errno);
            break;
        }

        if (FD_ISSET(sok->fd, &read_fs)) {
            int fd = accept(sok->fd, NULL, NULL);
            if (fd < 0) {
                log_e("%s: failed to accept socket: %s(%d), name: %s\n",
                            __func__, strerror(errno), -errno, sok->name);
                return -1;
            }

            struct socket *new_sok =
                    create_socket_struct(sok->name, fd);

            /**
             * callback to link manager
             */
            callback(user, new_sok);
        }
    }

    return 0;
}

int socket_get_fd(void *socket) {
    struct socket *sok = (struct socket *)socket;

    if (sok == NULL)
        return -1;

    return sok->fd;
}

static struct link_ops link_ops = {
        .create_client = create_socket_client,
        .create_server = create_socket_server,
        .delete = delete_socket,
        .connect = socket_connect,
        .wait_for_connect = socket_wait_for_connect,
        .read = socket_read,
        .write = socket_write,
        .get_fd = socket_get_fd,
};

struct link_ops *get_link_ops(void) {
    return &link_ops;
}


