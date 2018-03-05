/**
 * 
 * Release under GPLv2.
 * 
 * @file    vad-cutting.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/03 2018 16:29
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/03 2018      create the file
 * 
 *     last modified: 05/03 2018 16:29
 */
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "wav_helper.h"
#include "signal_helper.h"
#include "cnt_vad.h"
#include "log_helper.h"

#define DATA_POINT_LEN	(320)
#define FRAME_LEN_MS	(20)

#define FRAME_COUNT		(10) // 连续语音时间，(FRAME_COUNT * FRAME_LEN_MS)
#define SAVE_FRONT_CNT	(5)	 // 多保存前节点的长度，(SAVE_FRONT_CNT * FRAME_LEN_MS)
#define SAVE_TAIL_CNT	(5)	 // 多保存前节点的长度，(SAVE_TAIL_CNT * FRAME_LEN_MS)
#define VOICE_MID_CNT	(22) // 语音中，词与词之间的间隔时间，(VOICE_MID_CNT * FRAME_LEN_MS)

#define WAV_DIR					"wav/"
#define VAD_CUTTING_WAV_PATH	WAV_DIR"test.wav"
#define VAD_SAVE_WAV_PATH		WAV_DIR"dst"

const char *version="vad-cutting version: 0.0.3";

int vad_test(int (*voice_time)[2])
{
	short buffer[DATA_POINT_LEN] = {0};
	int frame_cnt = 0;
	int vad_flag = 0, front_frame_cnt = 0;
    int voice_flag = 0;
    int voice_flag_cnt = 0;
    int wav_num = 0;
	int quit_cnt = 0;
	short ret;
	cntVadState vad_stat;

	cntVAD16kInit(&vad_stat);

	wav_file_t *wav_file = wav_file_open(VAD_CUTTING_WAV_PATH);

	while (1) {
		if (wav_file_read(wav_file, buffer, DATA_POINT_LEN * 2) <= 0)
			break;

		ret = cntVAD16k(buffer,&vad_stat);
        if (1 == ret) {
			/*log_i("time: %d.%d", frame_cnt * FRAME_LEN_MS / 1000, frame_cnt * FRAME_LEN_MS % 1000);*/
			voice_flag_cnt = 0;
            if (front_frame_cnt++ >= FRAME_COUNT) {
				vad_flag = 1;
			}
        } else {
			if (quit_cnt++ > 3) {
				quit_cnt = 0;
				front_frame_cnt = 0;
				vad_flag = 0;
			}
        }

		if(vad_flag == 1 && voice_flag == 0) {
			voice_flag = 1;
			voice_time[wav_num][0] = frame_cnt - FRAME_COUNT - SAVE_FRONT_CNT;
			/*log_i("start: %d.%d", voice_time[wav_num][0] * FRAME_LEN_MS / 1000, voice_time[wav_num][0] * FRAME_LEN_MS % 1000);*/
        } else if (voice_flag == 1 && voice_flag_cnt++ == VOICE_MID_CNT) {
			voice_flag = 0;

			voice_time[wav_num][1] = frame_cnt - VOICE_MID_CNT + SAVE_TAIL_CNT;
			wav_num++;
        }

		frame_cnt++;
	}

	wav_file_clean(wav_file);

	return wav_num;
}

#define CHANNELS		(1)
#define SAMPLE_RATE		(16000)
#define BIT_PER_SAMPLE	(16)

#define BYTES_OF_1S		(1 * SAMPLE_RATE * BIT_PER_SAMPLE / 8)
#define BYTES_OF_1MS	(BYTES_OF_1S / 1000)

#define WAV_S_LEN		(15)

void save_wav(int (*voice_time)[2], int wav_num)
{
    char path[256] = {0};
    static char voice_buf[WAV_S_LEN * BYTES_OF_1S] = {0};

	wav_file_t *wav_test_file = wav_file_open(VAD_CUTTING_WAV_PATH);

    if (access(VAD_SAVE_WAV_PATH, F_OK) != 0) {
        mkdir(VAD_SAVE_WAV_PATH, S_IRWXU);
    }

    for (int i = 0; i < wav_num; i++) {
		if (i >= 785)
			sprintf(path, "%s/%04d.wav", VAD_SAVE_WAV_PATH, i+1);
		else
			sprintf(path, "%s/%04d.wav", VAD_SAVE_WAV_PATH, i);

		wav_file_t *wav_file = wav_file_create(path, CHANNELS, SAMPLE_RATE, BIT_PER_SAMPLE);

        int voice_bytes = voice_time[i][0] * FRAME_LEN_MS * BYTES_OF_1MS;
        int voice_last_bytes = (voice_time[i][1] - voice_time[i][0]) * FRAME_LEN_MS * BYTES_OF_1MS;

		wav_file_seek(wav_test_file, voice_bytes, SEEK_SET);
		wav_file_read(wav_test_file, voice_buf, voice_last_bytes);

		wav_file_write(wav_file, voice_buf, voice_last_bytes);
		wav_file_clean(wav_file);

		log_i("start: %d.%d", voice_time[i][0] * FRAME_LEN_MS / 1000, voice_time[i][0] * FRAME_LEN_MS % 1000);
		log_i("-----------end: %d.%d", voice_time[i][1] * FRAME_LEN_MS / 1000, voice_time[i][1] * FRAME_LEN_MS % 1000);
		printf("\n");
    }

	log_i("wav_num: %d", wav_num);

	wav_file_clean(wav_test_file);
}

int main(int argc, char** argv)
{
    int voice_time[2000][2] = {0};

	register_linux_signal_hanler(argv[0]);

	int wav_num = vad_test(voice_time);

	save_wav(voice_time, wav_num);

	log_i("vad cutting OK \n");

	return 1;
}

