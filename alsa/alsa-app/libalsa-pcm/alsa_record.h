/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa_record.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/03 2018 20:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/03 2018      create the file
 * 
 *     last modified: 06/03 2018 20:43
 */
#ifndef __BASE_LIB_ALSA_RECORD_H_
#define __BASE_LIB_ALSA_RECORD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <alsa/asoundlib.h>

typedef struct {
	const char *pcm_name;
	snd_pcm_format_t format;
	int channels;
	unsigned int sample_rate;
} alsa_params_t;

typedef struct {
	snd_pcm_t *handle;
	alsa_params_t alsa_params;
	snd_pcm_uframes_t idle_fram_cnt;
} alsa_handle_t;
#define ALSA_HANDLE_T_LEN (sizeof(alsa_handle_t))


#ifndef ALSA_RECORD_GB
#define ALSA_RECORD_EX extern
#else
#define ALSA_RECORD_EX
#endif

//#define DEV_NAME		"default"//"plughw:2,0"
//#define DEV_NAME		"plughw:2,0"
#define DEV_NAME		"hw:1,0"

#define CHANNELS		(2)
#define SAMPLE_RATE		(44100)
#define BIT_PER_SAMPLE	(16)

#define FRAMES_CNT		(1024)
#define FRAMES_SIZE		(BIT_PER_SAMPLE / 8 * CHANNELS)

ALSA_RECORD_EX alsa_handle_t *alsa_record_get_handle(alsa_params_t alsa_params);
ALSA_RECORD_EX void alsa_record_put_handle(alsa_handle_t *alsa_handle);
ALSA_RECORD_EX int alsa_record_read_pcm(alsa_handle_t *alsa_handle, void *buf, int len);

#ifdef __cplusplus
}
#endif

#endif /* end __BASE_LIB_ALSA_RECORD_H_ */

