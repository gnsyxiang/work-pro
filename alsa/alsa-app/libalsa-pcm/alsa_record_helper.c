/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa_record_helper.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/03 2018 20:44
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/03 2018      create the file
 * 
 *     last modified: 06/03 2018 20:44
 */
#include <stdio.h>
#include <stdlib.h>

#include "mem_helper.h"

#define ALSA_RECORD_GB
#include "alsa_record.h"
#undef ALSA_RECORD_GB

#define DEBUG(x,y...)	\
	do {				\
		printf("[%s : %s : %d]", __FILE__, __func__, __LINE__); \
		printf(x, ##y); \
		printf("\n");	\
	} while(0)

#define ERROR(x,y...)	\
	do {				\
		printf("[%s : %s : %d]", __FILE__, __func__, __LINE__); \
		printf(x, ##y); \
		printf("\n");	\
	} while(0)

int alsa_set_param(alsa_handle_t *alsa_handle, alsa_params_t alsa_params)
{
	snd_pcm_hw_params_t *hw_params;

	snd_pcm_hw_params_alloca(&hw_params);
	/*snd_pcm_hw_params_malloc(&hw_params);*/
	/*snd_pcm_hw_params_free(hw_params);*/

	if (snd_pcm_hw_params_any(alsa_handle->handle, hw_params) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params_set_access(alsa_handle->handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
		ERROR("snd_pcm_hw_params_set_access faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params_set_format(alsa_handle->handle, hw_params, alsa_params.format) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	unsigned int sample_rate = alsa_params.sample_rate;
	if (snd_pcm_hw_params_set_rate_near(alsa_handle->handle, hw_params, &sample_rate, 0) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (alsa_params.sample_rate != sample_rate) {
		ERROR("The rate %d Hz is not supported by your hardware. ==> Using %d Hz instead.",
				alsa_params.sample_rate, sample_rate);
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params_set_channels(alsa_handle->handle, hw_params, alsa_params.channels) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_hw_params(alsa_handle->handle, hw_params) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	if (snd_pcm_prepare(alsa_handle->handle) < 0) {
		ERROR("snd_pcm_hw_params_any faild");
		goto ERR_HANDLE;
	}

	snd_pcm_uframes_t alsa_buffer_size;
	snd_pcm_hw_params_get_period_size(hw_params, &alsa_handle->idle_fram_cnt, 0);
	snd_pcm_hw_params_get_buffer_size(hw_params, &alsa_buffer_size);
	if (alsa_handle->idle_fram_cnt == alsa_buffer_size) {
		ERROR(("Can't use period equal to buffer size (%lu == %lu)"), alsa_handle->idle_fram_cnt, alsa_buffer_size);
		goto ERR_HANDLE;
	}

	return 0;

ERR_HANDLE:
	snd_pcm_close(alsa_handle->handle);
	return -1;
}

alsa_handle_t *alsa_record_get_handle(alsa_params_t alsa_params)
{
	alsa_handle_t *alsa_handle = (alsa_handle_t *)alloc_mem(ALSA_HANDLE_T_LEN);

	if (snd_pcm_open(&alsa_handle->handle, alsa_params.pcm_name, SND_PCM_STREAM_CAPTURE, 0) < 0) {
		ERROR("err snd_pcm_open, device-name: %s", DEV_NAME);
		return NULL;
	}

	if (0 != alsa_set_param(alsa_handle, alsa_params)) {
		snd_pcm_close(alsa_handle->handle);
		free_mem(alsa_handle);

		ERROR("err snd_pcm_open, device-name: %s", DEV_NAME);
		return NULL;
	}

	return alsa_handle;
}

void alsa_record_put_handle(alsa_handle_t *alsa_handle)
{
	snd_pcm_close(alsa_handle->handle);
	free_mem(alsa_handle);
}

int alsa_record_read_pcm(alsa_handle_t *alsa_handle, void *buf, int len)
{
	assert(alsa_handle->handle);
	assert(buf);
	int buf_len = len / FRAMES_SIZE;

	int ret = snd_pcm_readi(alsa_handle->handle, buf, buf_len);
	if (ret != buf_len) {
		printf ("read from audio interface failed (%s)\n", snd_strerror(ret));
		exit (1);
	}

	return ret;
}

