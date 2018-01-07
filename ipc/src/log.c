/**
 * 
 * Release under GPLv2.
 * 
 * @file    log.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/01 2018 13:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/01 2018      create the file
 * 
 *     last modified: 07/01 2018 13:41
 */
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>

#include "log.h"

#define LOG_BUF_SIZE 2048

static int log_debug_level = LOG_VERBOSE;

static double get_clock(void) {
    struct timespec ts;

    clock_gettime(CLOCK_BOOTTIME, &ts);
    return (double) ts.tv_sec + (double) (ts.tv_nsec) / 1000000000;
}

static void log_output(char *buffer) {
    printf("%s\n", buffer);
}

void log_debug(int level, const char *file, int line, const char *fmt, ...) {
    char buffer[LOG_BUF_SIZE];
    int size = 0, sz;

    if (!((level <= LOG_VERBOSE) && (level <= log_debug_level)))
        return ;

    size = sprintf(buffer, "[%.03f] ", get_clock());

    sz = sprintf(buffer + size, "%s:%d: ", file, line);
    size += sz;

    va_list var_args;

    va_start(var_args, fmt);
    vsnprintf(buffer + size, LOG_BUF_SIZE - size, fmt, var_args);
    va_end(var_args);

    log_output(buffer);
}

void log_set_debug_level(int level) {
    log_debug_level = level;
}
