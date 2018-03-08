/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa_record_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/03 2018 20:55
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/03 2018      create the file
 * 
 *     last modified: 06/03 2018 20:55
 */
#include <stdio.h>

#include "alsa_record.h"
#include "wav_helper.h"

void save_pcm_to_wav(alsa_handle_t *alsa_handle)
{
	int frame_len = FRAMES_SIZE * FRAMES_CNT;
	char *buffer = malloc(frame_len);

	printf("frame_len: %d \n", frame_len);

	wav_file_t *new_wav_file = wav_file_create("test.wav", CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

	for (int i = 0; i < 200; ++i) {
		alsa_record_read_pcm(alsa_handle, buffer, frame_len);

		printf("i: %d \n", i);

		wav_file_write(new_wav_file, buffer, frame_len);
	}

	wav_file_clean(new_wav_file);
	free(buffer);
}

int main(void)
{
	alsa_params_t alsa_params;

	alsa_params.pcm_name	= DEV_NAME;
	alsa_params.format		= SND_PCM_FORMAT_S16_LE;
	alsa_params.channels	= CHANNELS;
	alsa_params.sample_rate	= SAMPLE_RATE;

	alsa_handle_t *alsa_handle  = alsa_record_get_handle(alsa_params);
	if (!alsa_handle)
		return -1;

	save_pcm_to_wav(alsa_handle);

	alsa_record_put_handle(alsa_handle);

	printf("alsa record OK \n");

	return 0;
}

