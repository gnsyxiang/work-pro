/**
 *
 * Release under GPLv2.
 * 
 * @file    log.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    07/01 2018 13:19
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        07/01 2018      create the file
 * 
 *     last modified: 07/01 2018 13:19
 */
#ifndef __LOG_H__
#define __LOG_H__
#include <libgen.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_VERBOSE,
};

#define LOG_DEBUG(level, ...)                                          \
            log_debug(level, basename(__FILE__), __LINE__, __VA_ARGS__)       \


void log_debug(int level, const char *file, int line, const char *fmt, ...);

#define log_d(...)      LOG_DEBUG(LOG_DEBUG, __VA_ARGS__)
#define log_i(...)      LOG_DEBUG(LOG_INFO,  __VA_ARGS__)
#define log_w(...)      LOG_DEBUG(LOG_WARN, __VA_ARGS__)
#define log_e(...)      LOG_DEBUG(LOG_ERROR, __VA_ARGS__)

void log_set_debug_level(int level);

#ifdef __cplusplus
}
#endif

#endif

