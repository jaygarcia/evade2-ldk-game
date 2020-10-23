#ifndef FONT_H
#define FONT_H

#include "Game.h"

// this doesn't seem right, but it will probably work
// as in, Font::fprint(fmt, args...)

class Font {
  public:
  static TInt16 scale; // 8.8 fixed point

  public:
  // these routine return the width of whatever is printed to the screen
  static TInt8 write(TUint8 color, TInt16 x, TInt16 y, char c);
  static TInt8 write(TInt16 x, TInt16 y, char c);
  static TInt8 printf(TInt16 x, TInt16 y, const char* ifsh, ...);
  static TInt8 printf(TUint8 color, TInt16 x, TInt16 y, const char* ifsh, ...);

#ifdef ENABLE_ROTATING_TEXT
  static TInt8 print_string_rotatedx(TUint8 color, TInt16 x, TInt16 y, TFloat angle, const char* ifsh);
  static TInt8 print_string_rotatedx(TInt16 x, TInt16 y, TFloat angle, const char* ifsh);
#endif
  static TInt8 print_string(TUint8 color, TInt16 x, TInt16 y, char* s);
  static TInt8 print_long(TUint8 color, TInt16 x, TInt16 y, TInt32 n, TInt8 base = 10);

#ifdef PRINTF_TFloat
  static TInt8 print_float(TUint8 color, TInt16 x, TInt16 y, double number, TInt8 digits = 2);
#endif
};

#endif
