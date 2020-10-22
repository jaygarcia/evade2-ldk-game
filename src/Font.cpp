#define DEBUGME

/**
 * Some of this code lifted from Arduino serial Print class and modified.
 * See:
 * https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Print.cpp
 */

#include "Font.h"
#include "charset.h"

#include <stdarg.h>
#include <stdio.h>

#ifdef MIKE_REMOVED_THHIS
static TInt8 *pgm_read_TInt16(const void *addr) {
  const TUint16 *p = (const TUint16 *)addr;
  return (TInt8 *)*p;
}
#endif

static const TInt8 *charset[] = {
    ENull, // space
    font_emark,
#ifdef FULL_CHARSET
    font_dquote,
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_pound, // #
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_dollar, // $
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_percent, // %
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_amp, // &
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_squote, // '
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_lparen, // (
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_rparen, // )
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_asterisk, // *
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_plus,
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_comma,
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_minus,
#else
    ENull,
#endif
    font_period,    font_fslash, font_0, font_1, font_2, font_3,     font_4,
    font_5,         font_6,      font_7, font_8, font_9, font_colon,
#ifdef FULL_CHARSET
    font_semicolon,
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_lt, // <
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_eq, // =
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_gt, // >
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_qmark,
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_at, // @
#else
    ENull,
#endif
    font_a,         font_b,      font_c, font_d, font_e, font_f,     font_g,
    font_h,         font_i,      font_j, font_k, font_l, font_m,     font_n,
    font_o,         font_p,      font_q, font_r, font_s, font_t,     font_u,
    font_v,         font_w,      font_x, font_y, font_z,
#ifdef FULL_CHARSET
    font_lt, // [
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_bslash, // '\'
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_gt, // ]
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_caret, // ^
#else
    ENull,
#endif
#ifdef FULL_CHARSET
    font_uscore, // _
#else
    ENull,
#endif
    ENull, // ``
};

TInt16 Font::scale = 0x100;

#ifdef ENABLE_ROTATING_TEXT
TInt8 Font::print_string_rotatedx(TUint8 color, TInt16 x, TInt16 y, TFloat theta,
                                 const char *p) {
  theta = TFloat(theta) * 3.1415926 / 180;
  TFloat cost = cos(theta), sint = sin(theta);

  TFloat fscale = TFloat(scale >> 8) + TFloat(scale & 0xff) / 256.0;

  const TInt8 size = 9;

  TInt8 xo = x;
  while (char c = *p++) {
    const TInt8 *glyph = charset[toupper(c) - 32];
    if (glyph) {
      TInt8 lines = *glyph++;

      for (TInt8 i = 0; i < lines; i++) {
        TFloat x0 = (TInt8)*glyph++ * fscale + x,
              y0 = (TInt8)*glyph++ * fscale + y,
              x1 = (TInt8)*glyph++ * fscale + x,
              y1 = (TInt8)*glyph++ * fscale + y;

        gDisplay.renderBitmap->DrawLine(ENull,x0, ((y0 - y) * sint + cost + y), x1,
                           ((y1 - y) * sint + cost + y), color);
      }
      x += size * fscale;
    } else {
      x += 6 * fscale;
    }
  }
  return x - xo;
}

TInt8 Font::print_string_rotatedx(TInt16 x, TInt16 y, TFloat theta, const char *p) {
  theta = TFloat(theta) * 3.1415926 / 180;
  TFloat cost = cos(theta), sint = sin(theta);

  TFloat fscale = TFloat(scale >> 8) + TFloat(scale & 0xff) / 256.0;

  const TInt8 size = 9;

  TInt8 xo = x;
  while (char c = *p++) {
    const TInt8 *glyph = charset[toupper(c) - 32];
    if (glyph) {
      TInt8 lines = *glyph++;

      for (TInt8 i = 0; i < lines; i++) {
        TFloat x0 = (TInt8)*glyph++ * fscale + x,
              y0 = (TInt8)*glyph++ * fscale + y,
              x1 = (TInt8)*glyph++ * fscale + x,
              y1 = (TInt8)*glyph++ * fscale + y;

        gDisplay.renderBitmap->DrawLine(ENull,x0, ((y0 - y) * sint + cost + y), x1,
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

TInt8 Font::write(TUint8 color, TInt16 x, TInt16 y, char c) {
  const TInt8 *glyph;
  const TInt8 width = 9;

  TFloat fscale = TFloat(scale >> 8) + TFloat(scale & 0xff) / 256.0;

  glyph = charset[toupper(c) - 32];
  //  ::printf("write %x\n", palette[COLOR_RED]);
  //  ::printf("c %c glyph %lx charset %lx\n", c, (long)glyph, (long)charset);
  if (glyph) {
    TInt8 lines = *glyph++;

    for (TInt8 i = 0; i < lines; i++) {
      TInt16 x0 = *glyph++, y0 = *glyph++, x1 = *glyph++, y1 = *glyph++;

      gDisplay.renderBitmap->DrawLine(
        ENull,
        x + x0 * fscale,
        y + y0 * fscale,
        x + x1 * fscale,
        y + y1 * fscale,
        color
      );
    }
  }
  return width * fscale;
}

TInt8 Font::write(TInt16 x, TInt16 y, char c) { return write(SHMOO_COLOR, x, y, c); }

TInt8 Font::print_string(TUint8 color, TInt16 x, TInt16 y, char *s) {
  TInt8 xx = x;
  while (char c = *s++) {
    TInt8 width = Font::write(x, y, c);
    x += width;
  }
  return x - xx; // width of string printed
}

TInt8 Font::print_long(TUint8 color, TInt16 x, TInt16 y, TInt32 n, TInt8 base) {
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

#ifdef PRINTF_TFloat
TInt8 Font::print_float(TUint8 color, TInt16 x, TInt16 y, double number,
                       TInt8 digits) {
  // TODO: Fix me --
  return 1;
  TInt8 xx = x;
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

TInt8 Font::printf(TUint8 color, TInt16 x, TInt16 y, const char *ifsh, ...) {
  va_list ap;
  TInt8 xx = x;
  char c;
  const char * p = reinterpret_cast<const char *>(ifsh);
  va_start(ap, ifsh);

  while ((c = *p++)) {
    if (c == '%') {
      c = *p++;
      switch (c) {
      case '\0':
        va_end(ap);
        return x - xx;
#ifdef PRINTF_TFloat
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
#ifdef PRINTF_TFloat
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

TInt8 Font::printf(TInt16 x, TInt16 y, const char *ifsh, ...) {
  va_list ap;
  TInt8 xx = x;
  char c;
  const char * p = reinterpret_cast<const char *>(ifsh);
  va_start(ap, ifsh);

  while ((c = *p++)) {
    if (c == '%') {
      c = *p++;
      switch (c) {
      case '\0':
        va_end(ap);
        return x - xx;
#ifdef PRINTF_TFloat
      case '%':
        x += Font::write(SHMOO_COLOR, x, y, '%');
        break;
      case 'f':
        x += print_float(SHMOO_COLOR, x, y, va_arg(ap, double));
        break;
#endif
      case 'd':
        x += print_long(SHMOO_COLOR, x, y, (TInt32)va_arg(ap, TInt));
        break;
#ifdef PRINTF_TFloat
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
