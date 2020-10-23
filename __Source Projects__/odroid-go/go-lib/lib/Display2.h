
#pragma once

#include <stdint.h>

#define DISPLAY_WIDTH (320)
#define DISPLAY_HEIGHT (240) 



// void lock_display();
// void unlock_display();
// void send_continue_wait();
// void send_continue_line();
// void send_reset_drawing(int left, int top, int width, int height);
// void send_display_boot_program();

class Display2 {
public:


  static void Init();

  static void WriteFrame(uint8_t* frameBuffer, uint16_t* palette);
  static void backlight_percentage_set(int value);
  static void clear(uint16_t color);
  static int isBacklightInitialized();

};
