/**
 * Portions of this class were lifted from the Arduboy2 library/class.
 * See https://github.com/MLXXXp/Arduboy2
 */

#define DEBUGME
#include "Evade2.h"

#include <stdio.h>

static UBYTE sBuffer[WIDTH * HEIGHT / 8];

static inline void swap(WORD &a, WORD &b) {
  WORD temp = a;
  a = b;
  b = temp;
}

void Graphics::display(BOOL clear) {
  // printf("Graphics::display %i\n", (int) clear);

  Gfx::Paint(clear);
}

static const uint8_t bitshift_left[] PROGMEM = {_BV(0), _BV(1), _BV(2), _BV(3),
                                                _BV(4), _BV(5), _BV(6), _BV(7)};

BOOL Graphics::drawPixel(WORD x, WORD y, UBYTE color) {
  Gfx::DrawPixel(x, y, color);
  return TRUE;
}

BOOL Graphics::drawCircle(WORD x0, WORD y0, BYTE r) {
  // TODO: Add Gfx::DrawCircle to the Display class:
  Gfx::DrawCircle(x0, y0, r, COLOR_WHITE);
  return true;
}

#ifdef FAST_LINE_ENABLE
BOOL Graphics::drawLine(WORD x, WORD y, WORD x2, WORD y2, UBYTE color) {
  const int PRECISION = 8;
  BOOL drawn = false;

#ifdef INLINE_PLOT
  WORD row_offset;
  UBYTE bit;
  UBYTE row;
#endif

  bool yLonger = false;
  WORD incrementVal, endVal;
  WORD shortLen = y2 - y;
  WORD longLen = x2 - x;

  if (abs(shortLen) > abs(longLen)) {
    swap(shortLen, longLen);
    yLonger = true;
  }

  endVal = longLen;

  if (longLen < 0) {
    incrementVal = -1;
    longLen = -longLen;
  } else {
    incrementVal = 1;
  }

  WORD decInc = longLen == 0 ? 0 : (shortLen << PRECISION) / longLen;
  WORD j = 0;
  if (yLonger) {
    for (WORD i = 0; i != endVal; i += incrementVal, j += decInc) {
#ifdef INLINE_PLOT
      WORD xx = x + (j >> PRECISION), yy = y + i;

      if (xx & ~0x7f || yy & ~0x3f) {
        continue;
      }
      drawn = TRUE;
      row = (uint8_t)yy / 8;
      row_offset = (row * WIDTH) + (uint8_t)xx;
      bit = _BV((UBYTE)yy % 8);
      sBuffer[row_offset] |= bit;
#else
      drawn |= drawPixel(x + (j >> PRECISION), y + i, color);
#endif
    }
  } else {
    for (WORD i = 0; i != endVal; i += incrementVal, j += decInc) {
#ifdef INLINE_PLOT
      WORD xx = x + i, yy = y + (j >> PRECISION);

      if (xx & ~0x7f || yy & ~0x3f) {
        continue;
      }
      drawn = TRUE;
      row = (uint8_t)yy / 8;
      row_offset = (row * WIDTH) + (uint8_t)xx;
      bit = _BV((UBYTE)yy % 8);
      sBuffer[row_offset] |= bit;
#else
      drawn |= drawPixel(x + i, y + (j >> PRECISION), color);
#endif
    }
  }
  return drawn;
}
#else
BOOL Graphics::drawLine(WORD x0, WORD y0, WORD x1, WORD y1, UBYTE color) {
  BOOL xInBounds = (x0 >= 0) && (x0 <= DISPLAY_WIDTH) && (y0 >= 0) && (y0 < DISPLAY_HEIGHT),
       yInBounds = (x1 >= 0) && (x1 <= DISPLAY_WIDTH) && (y1 >= 0) && (y1 < DISPLAY_HEIGHT);

  if ((! xInBounds) && !(yInBounds)) {
    return false;
  }
  Gfx::DrawLine((int16_t)x0, (int16_t)y0, (int16_t)x1, (int16_t)y1, color);
  return true;
}

#endif

struct vec_segment_u8 {
  int8_t x0;
  int8_t y0;
  int8_t x1;
  int8_t y1;
};

BOOL Graphics::drawVectorGraphic(const BYTE *graphic, float x, float y,
                                 float theta, float scaleFactor, UBYTE color) {
  return explodeVectorGraphic(graphic, x, y, theta, scaleFactor, 0, color);
}

BOOL Graphics::explodeVectorGraphic(const BYTE *graphic, float x, float y,
                                    float theta, float scaleFactor, BYTE step,
                                    UBYTE color) {
  graphic += 2;
  BOOL drawn = false;

  BYTE numRows = pgm_read_byte(graphic++);

  float rad = float(theta) * 3.1415926 / 180, sint = sin(rad), cost = cos(rad);

  for (BYTE i = 0; i < numRows; i++) {
    struct vec_segment_u8 seg;
    float x0, y0, x1, y1;

    memcpy(&seg, graphic, sizeof(seg));
    graphic += sizeof(seg);

    x0 = seg.x0;
    y0 = seg.y0;
    x1 = seg.x1;
    y1 = seg.y1;

    if (scaleFactor) {
      x0 /= scaleFactor;
      y0 /= scaleFactor;
      x1 /= scaleFactor;
      y1 /= scaleFactor;
    }

    if (step) {
      x0 = x0 + (seg.x0 / 8) * step;
      y0 = y0 + (seg.y0 / 8) * step;
      x1 = x1 + (seg.x0 / 8) * step;
      y1 = y1 + (seg.y0 / 8) * step;
    }

    drawn |=
        drawLine(x0 * cost - y0 * sint + x, y0 * cost + x0 * sint + y,
                 x1 * cost - y1 * sint + x, y1 * cost + x1 * sint + y, color);
  }
  return drawn;
}

// TODO: Jay - I think this is wrong...   It shouldn't be oring and anding bits
// into the bitmap array
void Graphics::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
                          uint8_t w, uint8_t h, uint8_t color) {
  // no need to draw at all if we're offscreen
  if (x + w < 0 || x > WIDTH - 1 || y + h < 0 || y > HEIGHT - 1)
    return;

  int yOffset = abs(y) % 8;
  int sRow = y / 8;
  if (y < 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }
  int rows = h / 8;
  if (h % 8 != 0)
    rows++;
  for (int a = 0; a < rows; a++) {
    int bRow = sRow + a;
    if (bRow > (HEIGHT / 8) - 1)
      break;
    if (bRow > -2) {
      for (int iCol = 0; iCol < w; iCol++) {
        if (iCol + x > (WIDTH - 1))
          break;
        if (iCol + x >= 0) {
          const uint8_t bmp_bits = pgm_read_byte(bitmap + (a * w) + iCol);
          if (bRow >= 0) {
            if (color == WHITE)
              sBuffer[(bRow * WIDTH) + x + iCol] |= bmp_bits << yOffset;
            else if (color == BLACK)
              sBuffer[(bRow * WIDTH) + x + iCol] &= ~(bmp_bits << yOffset);
            else
              sBuffer[(bRow * WIDTH) + x + iCol] ^= bmp_bits << yOffset;
          }
          if (yOffset && bRow < (HEIGHT / 8) - 1 && bRow > -2) {
            if (color == WHITE)
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] |=
                  bmp_bits >> (8 - yOffset);
            else if (color == BLACK)
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] &=
                  ~(bmp_bits >> (8 - yOffset));
            else
              sBuffer[((bRow + 1) * WIDTH) + x + iCol] ^=
                  bmp_bits >> (8 - yOffset);
          }
        }
      }
    }
  }
}

void Graphics::fillScreen(UBYTE color) { Gfx::FillScreen(color); }
