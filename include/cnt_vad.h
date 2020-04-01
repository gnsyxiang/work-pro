/**
 * 
 * Release under GPLv2.
 * 
 * @file    cnt_vad.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    05/03 2018 10:10
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        05/03 2018      create the file
 * 
 *     last modified: 05/03 2018 10:10
 */
#ifndef CNT_VAD_H
#define CNT_VAD_H

typedef char Word8;
typedef short Word16;
typedef int Word32;
typedef double Float64;
typedef float Float32;
typedef long long	 Word64;

#define L_FILT16k       15      /* Delay of down-sampling filter              */
#define COMPLEN         12       /* Number of sub-bands used by VAD     */
#define F_5TH_CNT       5        /* Number of 5th order filters in VAD  */
#define F_3TH_CNT       6        /* Number of 3th order filters in VAD  */

typedef struct {
   Float64 mem_pow_sum;             /* power of previous frame               */

   Float32 mem_bckr_est[COMPLEN];   /* background noise estimate             */
   Float32 mem_ave_level[COMPLEN];  /* averaged input components             */
                                    /* for stationary estimation             */
   Float32 mem_level[COMPLEN];      /* input levels of the previous frame    */
   Float32 mem_sub_level[COMPLEN];  /* input levels calculated at the end of */
                                    /* a frame (lookahead)                   */
   Float32 mem_a_data5[F_5TH_CNT][2];/* memory for the filter bank           */
   Float32 mem_a_data3[F_3TH_CNT];  /* memory for the filter bank            */

   Float32 mem_sp_max;              /* maximum level                         */
   Float32 mem_speech_level;        /* estimated speech level                */

   Word16 mem_burst_count;          /* counts length of a speech burst       */
   Word16 mem_hang_count;           /* hangover counter                      */
   Word16 mem_stat_count;           /* stationary counter                    */
   Word16 mem_vadreg;               /* flags for intermediate VAD decisions  */
   Word16 mem_pitch_tone;           /* flags for pitch and tone detection    */
   Word16 mem_sp_est_cnt;           /* counter for speech level estimation   */
   Word16 mem_sp_max_cnt;           /* counts frames that contains speech    */

} cnt_Core_Vad_State;


typedef struct
{
   /* Float32 */
   Float32 mem_decim[2 * L_FILT16k];   /* speech decimated filter memory     */
   Float32 mem_sig_in[4];              /* hp50 filter memory                 */
   Float32 mem_preemph;                /* speech preemph filter memory       */


   cnt_Core_Vad_State vadSt;

} cntVadState;

short cntVAD16kInit(cntVadState *spe_state);
short cntVAD16k(short speech16k[],   cntVadState *spe_state);

#endif

