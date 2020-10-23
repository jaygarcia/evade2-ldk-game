#ifndef GFX_H
#define GFX_H

#include <stdint.h>

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_BRIGHT_BLUE 9
#define COLOR_BRIGHT_GREEN 10
#define COLOR_BRIGHT_CYAN 11
#define COLOR_BRIGHT_RED 12
#define COLOR_BRIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

class Gfx {
public:
  static void Init();
  static void CreateFrameBuffers();
  
  static void FillScreen(uint8_t color);
  static void SetColor(uint8_t index, uint8_t r, uint8_t b, uint8_t g);
  // static void FillScreen(uint16_t color);

  static void Paint(bool clear);
  static void Clear();
  
  static bool checkBoundary(int16_t x, int16_t y);

  static void swap(int16_t &a, int16_t &b);
  static void DrawPixel(int16_t x, int16_t y, uint8_t color);
  static void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
  
  static void DrawFastVLine(int16_t x, int16_t y, uint8_t h, uint8_t color);
  static void DrawFastHLine(int16_t x, int16_t y, uint16_t w, uint8_t color);

  static void DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color);
  static void FillRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color);
  
  static void DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
  static void FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);

  static void DrawRoundRect(int16_t x, int16_t y, uint16_t w, uint8_t h, uint16_t r, uint8_t color);
  static void FillRoundRect(int16_t x, int16_t y, uint16_t w, uint8_t h, uint16_t r, uint8_t color);

  static void DrawCircle(int16_t x0, int16_t y0, uint16_t r, uint8_t color);
  static void DrawCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint16_t corners, uint8_t color);

  static void FillCircle(int16_t x0, int16_t y0, uint16_t r, uint8_t color);
  static void FillCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint16_t sides, int16_t delta, uint8_t color);

};

#endif