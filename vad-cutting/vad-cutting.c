#include "cnt_vad.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "wav_helper.h"
#include "signal_helper.h"
#include "cnt_vad.h"

#define TEST_VAD_OLED

#ifdef TEST_VAD_OLED
#define DATA_POINT_LEN (320)
#define FRAME_LEN_MS (20)
#define FRAME_COUNT (7)
#else
#define DATA_POINT_LEN (160)
#define FRAME_LEN_MS (10)
#define FRAME_COUNT (5)
#endif

const char *version="vad-cutting version: 0.0.3 ";

int is_running = 0;
short vad_flag = 0, sTempVad = 0, neVadCnt = 0;

short vad_test()
{
	short buffer[DATA_POINT_LEN];
	static cntVadState vad_stat;

	unsigned int iTimes = 0;
    char * pcm0 = "vad_test.wav";

	FILE *fPcm0 = fopen(pcm0,"rb");
    fseek(fPcm0,44,SEEK_SET);

	memset(buffer,0,sizeof(short) * DATA_POINT_LEN);

	printf("------------2\n");
	cntVAD16kInit(&vad_stat);


    static int voice_flag = 0;
    static int voice_flag_count = 0;
    int voice_time[2000][2] = {0};
    int voice_count = 0;
#define VOICE_COUNT_ADD (0)

	while (1){
		if (fread(buffer, 2, DATA_POINT_LEN, fPcm0) <= 0)
			break;

		sTempVad = cntVAD16k(buffer,&vad_stat);

        if (1 == sTempVad)
        {
            neVadCnt++;
            if (neVadCnt < FRAME_COUNT)	//Á¬Ðø300ms½ü¶ËÓïÒô
            {
                vad_flag = 0;
            }
            else
            {
                vad_flag = 1;
            }
        }
        else if(-3 == sTempVad)
        {
            printf("please do auth!\n");
        }
        else
        {
            neVadCnt=0;
            vad_flag = 0;
        }

		if(vad_flag == 1)
		{
            voice_flag_count = 0;
            if (voice_flag == 0) {
                voice_flag = 1;
                voice_time[voice_count][0] = iTimes - 30;
            }
        } else {
            if (voice_flag == 1 && voice_flag_count++ == 30) {
                voice_flag_count = 0;
                voice_flag = 0;

                voice_time[voice_count++][1] = iTimes + VOICE_COUNT_ADD;
            }
        }

		iTimes++;

	}//end of while

    char path[256] = {0};
    static char voice_buf[20 * 16000 * 2];

#define VAD_CUTTING_PATH "vad_cutting"

    if (access(VAD_CUTTING_PATH, F_OK) != 0) {
        mkdir(VAD_CUTTING_PATH, S_IRWXU);
    }

    for (int i = 0; i < voice_count; i++) {
        printf("----timestamp:%d.%d is voice!\r\n", \
            voice_time[i][0] * FRAME_LEN_MS / 1000, voice_time[i][0] * FRAME_LEN_MS % 1000);
        printf("timestamp:%d.%d is voice!\r\n", \
            voice_time[i][1] * FRAME_LEN_MS / 1000, voice_time[i][1] * FRAME_LEN_MS % 1000);

        /* printf("%d - %d = %d \n", \ */
                /* voice_time[i][1] * FRAME_LEN_MS, \ */
                /* voice_time[i][0] * FRAME_LEN_MS, \ */
                /* voice_time[i][1] * FRAME_LEN_MS - voice_time[i][0] * FRAME_LEN_MS); */

        if (i >= 785) {
            sprintf(path, "%s/%04d.wav", VAD_CUTTING_PATH, i+1);
        } else {
            sprintf(path, "%s/%04d.wav", VAD_CUTTING_PATH, i);
        }

		wav_file_t *wav_file = wav_file_create(path, 1, 16000, 16);

        int voice_bytes = voice_time[i][0] * FRAME_LEN_MS * 16 * 2;
        /* printf("start: %d \n", voice_bytes); */

        fseek(fPcm0, voice_bytes, SEEK_SET);

        int voice_last_bytes = (voice_time[i][1] - voice_time[i][0]) * FRAME_LEN_MS * 16 * 2;
        /* printf("long: %d \n", voice_last_bytes); */

        /* char *voice_buf = (char *)malloc(voice_last_bytes); */

        fread(voice_buf, voice_last_bytes, 2, fPcm0);

		wav_file_write(wav_file, voice_buf, voice_last_bytes);

		wav_file_clean(wav_file);

        /* free(voice_buf); */
        printf("\n");
    }

    is_running = 0;
    sleep(1);

	fclose(fPcm0);
	return 0;
}

extern void register_linux_signal_hanler(const char *app_name);

int main(int argc, char** argv)
{
	register_linux_signal_hanler(argv[0]);

	vad_test();
	printf("end...\r\n");

	return 1;
}
