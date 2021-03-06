#define DEBUGME

/**
 * Some of this code lifted from Arduino serial Print class and modified.
 * See:
 * https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Print.cpp
 */

#include "Evade2.h"
#include "charset.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdarg.h>
#include <stdio.h>

#ifdef MIKE_REMOVED_THHIS
static BYTE *pgm_read_word(const void *addr) {
  const UWORD *p = (const UWORD *)addr;
  return (BYTE *)*p;
}
#endif

static const BYTE *charset[] = {
    NULL, // space
    font_emark,
#ifdef FULL_CHARSET
    font_dquote,
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_pound, // #
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_dollar, // $
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_percent, // %
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_amp, // &
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_squote, // '
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_lparen, // (
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_rparen, // )
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_asterisk, // *
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_plus,
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_comma,
#else
    NULL,
#endif
#ifdef FULL_CHARSET
    font_minus,
#else
    NULL,
#endif
    font_period,    font_fslash, font_0, font_1, font_2, font_3,     font_4,
    font_5,         font_6,      font_7, font_8, font_9, font_colon,
#ifdef full_charset
    font_semicolon,
#else
    NULL,
#endif
#ifdef full_charset
    font_lt, // <
#else
    NULL,
#endif
#ifdef full_charset
    font_eq, // =
#else
    NULL,
#endif
#ifdef full_charset
    font_gt, // >
#else
    NULL,
#endif
#ifdef full_charset
    font_qmark,
#else
    NULL,
#endif
#ifdef full_charset
    font_at, // @
#else
    NULL,
#endif
    font_a,         font_b,      font_c, font_d, font_e, font_f,     font_g,
    font_h,         font_i,      font_j, font_k, font_l, font_m,     font_n,
    font_o,         font_p,      font_q, font_r, font_s, font_t,     font_u,
    font_v,         font_w,      font_x, font_y, font_z,
#ifdef full_charset
    font_lt, // [
#else
    NULL,
#endif
#ifdef full_charset
    font_bslash, // '\'
#else
    NULL,
#endif
#ifdef full_charset
    font_gt, // ]
#else
    NULL,
#endif
#ifdef full_charset
    font_caret, // ^
#else
    NULL,
#endif
#ifdef full_charset
    font_uscore, // _
#else
    NULL,
#endif
    NULL, // ``
};

WORD Font::scale = 0x100;

#ifdef ENABLE_ROTATING_TEXT
BYTE Font::print_string_rotatedx(UBYTE color, WORD x, WORD y, FLOAT theta,
                                 const char *p) {
  theta = float(theta) * 3.1415926 / 180;
  FLOAT cost = cos(theta), sint = sin(theta);

  FLOAT fscale = FLOAT(scale >> 8) + FLOAT(scale & 0xff) / 256.0;

  const BYTE size = 9;

  BYTE xo = x;
  while (char c = *p++) {
    const BYTE *glyph = charset[toupper(c) - 32];
    if (glyph) {
      BYTE lines = *glyph++;

      for (BYTE i = 0; i < lines; i++) {
        FLOAT x0 = (BYTE)*glyph++ * fscale + x,
              y0 = (BYTE)*glyph++ * fscale + y,
              x1 = (BYTE)*glyph++ * fscale + x,
              y1 = (BYTE)*glyph++ * fscale + y;

        Graphics::drawLine(x0, ((y0 - y) * sint + cost + y), x1,
                           ((y1 - y) * sint + cost + y), color);
      }
      x += size * fscale;
    } else {
      x += 6 * fscale;
    }
  }
  return x - xo;
}

BYTE Font::print_string_rotatedx(WORD x, WORD y, FLOAT theta, const char *p) {
  theta = float(theta) * 3.1415926 / 180;
  FLOAT cost = cos(theta), sint = sin(theta);

  FLOAT fscale = FLOAT(scale >> 8) + FLOAT(scale & 0xff) / 256.0;

  const BYTE size = 9;

  BYTE xo = x;
  while (char c = *p++) {
    const BYTE *glyph = charset[toupper(c) - 32];
    if (glyph) {
      BYTE lines = *glyph++;

      for (BYTE i = 0; i < lines; i++) {
        FLOAT x0 = (BYTE)*glyph++ * fscale + x,
              y0 = (BYTE)*glyph++ * fscale + y,
              x1 = (BYTE)*glyph++ * fscale + x,
              y1 = (BYTE)*glyph++ * fscale + y;

        Graphics::drawLine(x0, ((y0 - y) * sint + cost + y), x1,
                           ((y1 - y) * sint + cost + y), SHMOO_COLOR);
      }
      x += size * fscale;
    } else {
      x += 6 * fscale;
    }
  }
  return x - xo;
}
#endif

BYTE Font::write(UBYTE color, WORD x, WORD y, char c) {
  const BYTE *glyph;
  const BYTE width = 9;

  FLOAT fscale = FLOAT(scale >> 8) + FLOAT(scale & 0xff) / 256.0;

  glyph = charset[toupper(c) - 32];
  //  ::printf("write %x\n", palette[COLOR_RED]);
  //  ::printf("c %c glyph %lx charset %lx\n", c, (long)glyph, (long)charset);
  if (glyph) {
    BYTE lines = *glyph++;

    for (BYTE i = 0; i < lines; i++) {
      WORD x0 = *glyph++, y0 = *glyph++, x1 = *glyph++, y1 = *glyph++;

      Graphics::drawLine(x + x0 * fscale, y + y0 * fscale, x + x1 * fscale,
                         y + y1 * fscale, color);
    }
  }
  return width * fscale;
}

BYTE Font::write(WORD x, WORD y, char c) { return write(SHMOO_COLOR, x, y, c); }

BYTE Font::print_string(UBYTE color, WORD x, WORD y, char *s) {
  BYTE xx = x;
  while (char c = *s++) {
    BYTE width = Font::write(x, y, c);
    x += width;
  }
  return x - xx; // width of string printed
}

BYTE Font::print_long(UBYTE color, WORD x, WORD y, LONG n, BYTE base) {
  // TODO: Fix me --
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2)
    base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (n);

  return print_string(color, x, y, str);
}

#ifdef PRINTF_FLOAT
BYTE Font::print_float(UBYTE color, WORD x, WORD y, double number,
                       BYTE digits) {
  // TODO: Fix me --
  return 1;
  BYTE xx = x;
  if (isnan(number)) {
    x += write(x, y, 'n');
    x += write(x, y, 'a');
    x += write(x, y, 'n');
    return x;
  }
  if (isinf(number)) {
    x += write(x, y, 'i');
    x += write(x, y, 'n');
    x += write(x, y, 'f');
    return x;
  }
  if (number > 4294967040.0 || number < -4294967040.0) {
    x += write(x, y, 'o');
    x += write(x, y, 'v');
    x += write(x, y, 'f');
    return x;
  }

  // Handle negative numbers
  if (number < 0.0) {
    x += write(x, y, '-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  x += print_long(color, x, y, int_part, 10);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    x += write(x, y, '.');
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    x += print_long(color, x, y, toPrint, 10);
    remainder -= toPrint;
  }

  return x - xx;
}
#endif

BYTE Font::printf(UBYTE color, WORD x, WORD y, const char *ifsh, ...) {
  va_list ap;
  BYTE xx = x;
  char c;
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  va_start(ap, ifsh);

  while ((c = *p++)) {
    if (c == '%') {
      c = *p++;
      switch (c) {
      case '\0':
        va_end(ap);
        return x - xx;
#ifdef PRINTF_FLOAT
      case '%':
        x += Font::write(color, x, y, '%');
        break;
      case 'f':
        x += print_float(color, x, y, va_arg(ap, double));
        break;
#endif
      case 'd':
        x += print_long(color, x, y, (long)va_arg(ap, int));
        break;
#ifdef PRINTF_FLOAT
      case 'x':
        x += print_long(color, x, y, (long)va_arg(ap, int) & 0xffff, 16);
        break;
      case 'l':
        x += print_long(color, x, y, va_arg(ap, long));
        break;
#endif
      default:
        x += Font::write(color, x, y, c);
        break;
      }
    } else {
      x += Font::write(color, x, y, c);
    }
  }
  va_end(ap);
  return xx - x;
}

BYTE Font::printf(WORD x, WORD y, const char *ifsh, ...) {
  va_list ap;
  BYTE xx = x;
  char c;
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  va_start(ap, ifsh);

  while ((c = *p++)) {
    if (c == '%') {
      c = *p++;
      switch (c) {
      case '\0':
        va_end(ap);
        return x - xx;
#ifdef PRINTF_FLOAT
      case '%':
        x += Font::write(SHMOO_COLOR, x, y, '%');
        break;
      case 'f':
        x += print_float(SHMOO_COLOR, x, y, va_arg(ap, double));
        break;
#endif
      case 'd':
        x += print_long(SHMOO_COLOR, x, y, (long)va_arg(ap, int));
        break;
#ifdef PRINTF_FLOAT
      case 'x':
        x += print_long(SHMOO_COLOR, x, y, (long)va_arg(ap, int) & 0xffff, 16);
        break;
      case 'l':
        x += print_long(SHMOO_COLOR, x, y, va_arg(ap, long));
        break;
#endif
      default:
        x += Font::write(SHMOO_COLOR, x, y, c);
        break;
      }
    } else {
      x += Font::write(SHMOO_COLOR, x, y, c);
    }
  }
  va_end(ap);
  return xx - x;
}
