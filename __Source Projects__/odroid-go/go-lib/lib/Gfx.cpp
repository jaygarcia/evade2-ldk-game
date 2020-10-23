#include "Gfx.h"

#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "esp_spiffs.h"
#include "esp_system.h"
#include "esp_wifi.h" // need?

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "driver/rtc_io.h"
#include "driver/spi_master.h"

#include "Display2.h"



//Todo: Discuss with @Mike about architecture
uint16_t palette[265];
// #define COLOR_BLACK (0)
// #define COLOR_BLUE (1)
// #define COLOR_GREEN (2)
// #define COLOR_CYAN (3)
// #define COLOR_RED (4)
// #define COLOR_MAGENTA (5)
// #define COLOR_BROWN (6)
// #define COLOR_GRAY (7)
// #define COLOR_DARK_GRAY (8)
// #define COLOR_BRIGHT_BLUE (9)
// #define COLOR_BRIGHT_GREEN (10)
// #define COLOR_BRIGHT_CYAN (11)
// #define COLOR_BRIGHT_RED (12)
// #define COLOR_BRIGHT_MAGENTA (13)
// #define COLOR_YELLOW (14)
// #define COLOR_WHITE (15)

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

uint8_t *frameBuffer[2];
int currentFrameBuffer = 0;

QueueHandle_t gfxQueue;
TaskHandle_t gfxTaskHandle;
bool gfxTaskIsRunning = false;

void gfxTask(void *arg) {
  printf("**********video task!**********\n");
  uint8_t* displayBuffer;

  gfxTaskIsRunning = true;

  while(1) {
    xQueuePeek(gfxQueue, &displayBuffer, portMAX_DELAY);

    //Todo: error: ISO C++ forbids comparison between pointer and integer [-fpermissive]
    // if (displayBuffer == 1) {
    //   break;
    // }

    // Display2::writeFrame(displayBuffer);

    // Todo: Should we copy the palette? 
    Display2::WriteFrame(displayBuffer, palette); 

    xQueueReceive(gfxQueue, &displayBuffer, portMAX_DELAY);
  }

  printf("gfxTask ended! Entering useless loop!\n");

  while (1) {}
}

//TODO: expose? Mike, let's dicuss! --Jay
void initPalette() {
  for (int i = 128; i < 255; i++) {
    Gfx::SetColor(i, i, i, i);
  }
  Gfx::SetColor(COLOR_BLACK, 0, 0, 0);
  Gfx::SetColor(COLOR_BLUE, 0, 0, 255);
  Gfx::SetColor(COLOR_GREEN, 0, 255, 0);
  Gfx::SetColor(COLOR_CYAN, 0, 255, 255);
  Gfx::SetColor(COLOR_RED, 255, 0, 0);
  Gfx::SetColor(COLOR_MAGENTA, 255, 0, 255);
  Gfx::SetColor(COLOR_BROWN, 150, 150, 0);
  Gfx::SetColor(COLOR_WHITE, 255, 255, 255);
  Gfx::SetColor(255, 255, 255, 255);
}

void Gfx::CreateFrameBuffers() {
  printf("Gfx:%s entered.\n", __func__);

  /** Frame Buffers **/
  frameBuffer[0] = (uint8_t *)heap_caps_malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
  // frameBuffer[0] = (uint8_t *)heap_caps_malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT, MALLOC_CAP_8BIT); // Only get 25FPS!!
  // frameBuffer[0] = (uint8_t *)malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT);
  if (! frameBuffer[0]) { 
    printf("Cannot allocate memory for frame buffer 0!\n");
    fflush(stdout);
    abort(); 
  }
  memset(frameBuffer[0], 0, DISPLAY_WIDTH * DISPLAY_HEIGHT);
  printf("%s: frameBuffer[0]=%p\n", __func__, (void *)frameBuffer[0]);

  frameBuffer[1] = (uint8_t *) heap_caps_malloc(DISPLAY_WIDTH * DISPLAY_WIDTH, MALLOC_CAP_8BIT | MALLOC_CAP_DMA);
  // frameBuffer[1] = (uint8_t *) heap_caps_malloc(DISPLAY_WIDTH * DISPLAY_WIDTH, MALLOC_CAP_8BIT); // Only get 25FPS!!
  // frameBuffer[1] = (uint8_t *)malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT);
  if (! frameBuffer[1]) { 
    printf("Cannot allocate memory for frame buffer 1!\n");
    fflush(stdout);
    abort(); 
  }
  memset(frameBuffer[1], 0, DISPLAY_WIDTH * DISPLAY_HEIGHT);
  printf("Gfx:%s: frameBuffer[1] = %p\n", __func__, (void *)frameBuffer[1]);


}

void Gfx::Init() {
  // Todo: Figure out where to put this in the sequence.
  // initPalette();

  // printf("Gfx:%s entered.\n", __func__);

  // Gfx::CreateFrameBuffers();

  Display2::Init();

  gfxQueue = xQueueCreate(1, sizeof(uint16_t*));
  xTaskCreatePinnedToCore(&gfxTask, "gfxTask", 1024 * 4, NULL, 5, &gfxTaskHandle, 1);
}

void Gfx::FillScreen(uint8_t color) {
  // printf("Gfx:%s: 0%4X\n", __func__, color);
  memset(frameBuffer[currentFrameBuffer], color, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint8_t));
}

void Gfx::Clear() {
  FillScreen(0);
}

void Gfx::Paint(bool clear) {
  // printf("Gfx:%s %i\n", __func__, clear);

  xQueueSend(gfxQueue, &frameBuffer[currentFrameBuffer], portMAX_DELAY);
  currentFrameBuffer = currentFrameBuffer ? 0 : 1;

  if (clear) {
    memset(frameBuffer[currentFrameBuffer], 0, DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint8_t));
  }
}



void printBits(size_t const size, void const * const ptr, int format565 = 0) {
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;
  int ctr = 0;
  for (i = size - 1; i >= 0;i--)     {
    for (j=7;j>=0;j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
      ctr++;
      if (format565) {
        if (ctr == 3 || ctr == 8 || ctr == 13) {
          printf(" ");
        }
      }
    }
  }
  // puts("");
}

uint16_t color565_(uint8_t r, uint8_t g, uint8_t b) {



  // uint16_t red = r << 8;// & ~(1 << 10);
  // uint16_t blue = (b & 192) << 2;
  // uint16_t green = (g << 8) >> 11;

  uint16_t red = ((r >> 3) & 0x1F) << 11;
  uint16_t blue = ((b & 0b11111100 ) <<3);;// ((b >> 2) & 0X3F) << 5;
  uint16_t green = (g >> 3) & 0x1F;

  uint8_t t1 = b;
  uint16_t t2 = blue;
  // uint16_t final = blue;

  uint16_t final = (uint16_t)(red | green | blue);

  {
    if (r < 10) {
      printf("  %i ", t1);
    }
    else if (r < 100) {
      printf(" %i ", t1);
    }
    else {
      printf("%i ", t1);
    }
  }

  printBits(sizeof(uint8_t), &t1);
  
  // printf("  (shifted) = ");
  // printBits(sizeof(uint16_t), &t2, 1);
  
  printf("  |  ");
  printBits(sizeof(uint16_t), &final, 1);
  printf("\n"); fflush(stdout);

  return final;
  
  // return (r << 8) | ((uint8_t)(g & 192) << 2) | (b & 224);


  // uint8_t red = ((r >> 3) & 0x1F) << 11;
  // uint8_t blue = (b >> 3) & 0x1F;
  // uint8_t green = ((g >> 2) & 0X3F) << 5;
  // return (uint16_t) (red | green | blue);

}

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KBLU  "\x1B[34m"
#define KWHT  "\x1B[37m"

void Gfx::SetColor(uint8_t index, uint8_t b, uint8_t r, uint8_t g) {

  uint16_t blue = (b & 0b11111000) << 5;
  uint16_t red = (r & 0b11111000);//((b) << 3);;// ((b >> 2) & 0X3F) << 5;

  uint16_t g2 = (g & 0b00011100) << 11;

  uint16_t g1 = (g & 0b11100000) >> 5;

  uint16_t green = g1 + g2;

  uint16_t final = (uint16_t)(red + green + blue);

  // printf("%s", KRED);
  // printBits(sizeof(uint16_t), &red, 1);
  // printf("%s", KNRM);

  // printf(" | %s", KBLU);
  // printBits(sizeof(uint16_t), &blue, 1);
  // printf(KNRM);

  // printf(" | %s", KGRN);
  // printBits(sizeof(uint16_t), &green, 1);
  // printf("%s | ", KNRM); 

  // printBits(sizeof(uint16_t), &final, 1);
  // printf("\n");
  // fflush(stdout);

  palette[index] = final;
}




void Gfx::DrawPixel(int16_t x, int16_t y, uint8_t color) {
  // printf("DrawPixel c=%i",)
  // TODO: this if statement is expensive if DrawPixel is called in doubly
  // nested loops!
  if ((x < 0) || (x > DISPLAY_WIDTH) || (y < 0) || (y > DISPLAY_HEIGHT)) {
    return;
  }

  frameBuffer[currentFrameBuffer][(y * DISPLAY_WIDTH) + x] = color;
}

void Gfx::swap(int16_t& a, int16_t& b) {
  int16_t temp = a;
  a = b;
  b = temp;
}

void Gfx::DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color) {
  DrawFastHLine(x, y, w, color);
  DrawFastHLine(x, y + h - 1, w, color);
  DrawFastVLine(x, y, h, color);
  DrawFastVLine(x + w - 1, y, h, color);
}

void Gfx::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
  // Bresenham's algorithm
  // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  bool steep = abs(y1 - y0) > abs(x1 - x0);

  if (steep) {
    // int16_t temp = a;
    // a = b;
    // b = temp;

    // Gfx::swap(x0, y0);
    int16_t temp = x0;
    x0 = y0;
    y0 = temp;

    // Gfx::swap(x1, y1);
    temp = x1;
    x1 = y1;
    y1 = temp;
  }

  if (x0 > x1) {
    // Gfx::swap(x0, x1);
    int16_t temp = x0;
    x0 = x1;
    x1 = temp;

    // Gfx::swap(y0, y1);
    temp = y0;
    y0 = y1;
    y1 = temp;
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      // Gfx::DrawPixel(y0, x0, color);
      // y0 is X coord and x0 is Y coord in this case
      if ((y0 >= 0) && (y0 <= DISPLAY_WIDTH) && (x0 >= 0) && (x0 < DISPLAY_HEIGHT)) {
        frameBuffer[currentFrameBuffer][(x0 * DISPLAY_WIDTH) + y0] = color;
      }
    } else {
      // Gfx::DrawPixel(x0, y0, color);
      // x0 is X doord and y0 is Y coord in this case
      if ((x0 >= 0) && (x0 <= DISPLAY_WIDTH) && (y0 >= 0) && (y0 < DISPLAY_HEIGHT)) {
        frameBuffer[currentFrameBuffer][(y0 * DISPLAY_WIDTH) + x0] = color;
      }
    }

    err -= dy;

    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void Gfx::DrawFastVLine(int16_t x, int16_t y, uint8_t h, uint8_t color) {
  int16_t zero = 0, end = (y + h - 1);

  for (int16_t a = max(zero, y); a < min(end, DISPLAY_HEIGHT); a++) {
    Gfx::DrawPixel(x, a, color);
  }
}

void Gfx::DrawFastHLine(int16_t x, int16_t y, uint16_t w, uint8_t color) {
  int16_t xEnd; // last x point + 1

  // Do y bounds checks
  if (y < 0 || y >= DISPLAY_HEIGHT)
    return;

  xEnd = x + w;

  // Check if the entire line is not on the display
  if (xEnd <= 0 || x >= DISPLAY_WIDTH)
    return;

  // Don't start before the left edge
  if (x < 0)
    x = 0;

  // Don't end past the right edge
  if (xEnd > DISPLAY_WIDTH)
    xEnd = DISPLAY_WIDTH;

  // calculate actual width (even if unchanged)
  w = xEnd - x;

  // buffer pointer plus row offset + x offset
  uint32_t ptr = (y * DISPLAY_WIDTH) + x;
  register uint8_t* dispBuf = frameBuffer[currentFrameBuffer] + ptr;

  while (w--) {
    *dispBuf++ = color;
  }
}

void Gfx::FillRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color) {
  for (int16_t i = x; i < x + w; i++) {
    DrawFastVLine(i, y, h, color);
  }
}

void Gfx::DrawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color) {
  DrawLine(x0, y0, x1, y1, color);
  DrawLine(x1, y1, x2, y2, color);
  DrawLine(x2, y2, x0, y0, color);
}

void Gfx::FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color) {
  int16_t a, b, y, last;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1);
    swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1);
    swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1);
    swap(x0, x1);
  }

  if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if (x1 < a) {
      a = x1;
    } else if (x1 > b) {
      b = x1;
    }
    if (x2 < a) {
      a = x2;
    } else if (x2 > b) {
      b = x2;
    }
    DrawFastHLine(a, y0, b - a + 1, color);
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1, sa = 0, sb = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if (y1 == y2) {
    last = y1; // Include y1 scanline
  } else {
    last = y1 - 1; // Skip it
  }

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b) {
      swap(a, b);
    }

    DrawFastHLine(a, y, b - a + 1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);

  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b) {
      swap(a, b);
    }

    DrawFastHLine(a, y, b - a + 1, color);
  }
}

void Gfx::DrawCircle(int16_t x0, int16_t y0, uint16_t r, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  DrawPixel(x0, y0 + r, color);
  DrawPixel(x0, y0 - r, color);
  DrawPixel(x0 + r, y0, color);
  DrawPixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    DrawPixel(x0 + x, y0 + y, color);
    DrawPixel(x0 - x, y0 + y, color);
    DrawPixel(x0 + x, y0 - y, color);
    DrawPixel(x0 - x, y0 - y, color);
    DrawPixel(x0 + y, y0 + x, color);
    DrawPixel(x0 - y, y0 + x, color);
    DrawPixel(x0 + y, y0 - x, color);
    DrawPixel(x0 - y, y0 - x, color);
  }
}

void Gfx::DrawCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint16_t corners, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (corners & 0x4) { // lower right
      DrawPixel(x0 + x, y0 + y, color);
      DrawPixel(x0 + y, y0 + x, color);
    }

    if (corners & 0x2) { // upper right
      DrawPixel(x0 + x, y0 - y, color);
      DrawPixel(x0 + y, y0 - x, color);
    }
    if (corners & 0x8) { // lower left
      DrawPixel(x0 - y, y0 + x, color);
      DrawPixel(x0 - x, y0 + y, color);
    }

    if (corners & 0x1) { // upper left
      DrawPixel(x0 - y, y0 - x, color);
      DrawPixel(x0 - x, y0 - y, color);
    }
  }
}

void Gfx::FillCircle(int16_t x0, int16_t y0, uint16_t r, uint8_t color)
{
  DrawFastVLine(x0, y0 - r, 2 * r + 1, color);
  FillCircleHelper(x0, y0, r, 3, 0, color);
}

void Gfx::FillCircleHelper(int16_t x0, int16_t y0, uint16_t r, uint16_t sides, int16_t delta, uint8_t color)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }

    x++;
    ddF_x += 2;
    f += ddF_x;

    if (sides & 0x1) { // right side
      DrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
      DrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
    }

    if (sides & 0x2) { // left side
      DrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
      DrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
    }
  }
}

void Gfx::DrawRoundRect(int16_t x, int16_t y, uint16_t w, uint8_t h, uint16_t r, uint8_t color)
{
  DrawFastHLine(x + r, y, w - 2 * r, color);         // Top
  DrawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
  DrawFastVLine(x, y + r, h - 2 * r, color);         // Left
  DrawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
  // Draw four corners
  DrawCircleHelper(x + r, y + r, r, 1, color);
  DrawCircleHelper(x + w - r - 1, y + r, r, 2, color);
  DrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  DrawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void Gfx::FillRoundRect(int16_t x, int16_t y, uint16_t w, uint8_t h, uint16_t r, uint8_t color)
{
  FillRect(x + r, y, w - 2 * r, h, color);
  FillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  FillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}