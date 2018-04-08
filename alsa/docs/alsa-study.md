alsa-study
==========

## 基本概念

### period

关于peroid的概念有这样的描述：The “period” is a term that corresponds to a fragment in the OSS world. The period defines the size at which a PCM interrupt is generated. 从底层驱动看来，应该是PCM DMA单次传送数据帧的大小。其实真正关注底层驱动的话，它并不是关心peroid_time，它关心的是peroid_size，这两者有转换关系。具体见struct snd_pcm_hardware结构体。

注意在ALSA中peroid_size是以frame为单位的。

唯一要留意的是snd_pcm_writei()和snd_pcm_readi()的第三个参数size也是以frame为单位，不要忘记frames和bytes的转换。

## 播放

原理: 从WAV文件读取PCM数据，通过I2S或AC97依次送到Audio Codec。
难点在于对snd_pcm_hw_params_t的设置，尤其要确定每次要送到Audio Codec的数据帧大小(peroid_size)。

## 录音







