#ifndef FONT_H
#define FONT_H

#include "Evade2.h"

// this doesn't seem right, but it will probably work
// as in, Font::fprint(fmt, args...)

class Font
{
  public:
  static WORD scale; // 8.8 fixed point

  public:
  // these routine return the width of whatever is printed to the screen
  static BYTE write(UBYTE color, WORD x, WORD y, char c);
  static BYTE write(WORD x, WORD y, char c);
  static BYTE printf(WORD x, WORD y, const char* ifsh, ...);
  static BYTE printf(UBYTE color, WORD x, WORD y, const char* ifsh, ...);
#ifdef ENABLE_ROTATING_TEXT
  static BYTE print_string_rotatedx(UBYTE color, WORD x, WORD y, FLOAT angle, const char* ifsh);
  static BYTE print_string_rotatedx(WORD x, WORD y, FLOAT angle, const char* ifsh);
#endif
  static BYTE print_string(UBYTE color, WORD x, WORD y, char* s);
  static BYTE print_long(UBYTE color, WORD x, WORD y, LONG n, BYTE base = 10);
  static BYTE print_float(UBYTE color, WORD x, WORD y, double number, BYTE digits = 2);
};

#endif
