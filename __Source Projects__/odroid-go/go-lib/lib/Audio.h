#ifndef AUDIO_H
#define AUDIO_H

#define VOLUME_LEVEL_COUNT (5)
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "driver/i2s.h"
#include "driver/rtc_io.h"


class Audio {

public:
  static void SetVolume(float value);
  // static void ChangeVolume();
  static float GetVolume();
  static void Init(uint16_t new_sample_rate);
  static void Terminate();
  static void Submit(short* stereoAudioBuffer, int frameCount);
  static int  GetSampleRate();
  static void Mixdown(short *stereoAudioBuffer, int frameCount);
  static void Mute();
  static void Unmute();

};

#endif
