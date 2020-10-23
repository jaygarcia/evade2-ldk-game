#ifndef TYPES_H
#define TYPES_H

//#include <Arduboy2Core.h>

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t UBYTE;
typedef int8_t BYTE;
typedef uint16_t UWORD;
typedef int16_t WORD;
typedef uint8_t BOOL;
typedef void *APTR;
typedef uint32_t ULONG;
typedef int32_t LONG;
typedef float FLOAT;

#ifndef TRUE
#define TRUE (!0)
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL (void *)0
#endif

#ifndef PI
#define PI 3.1415926
#endif

#ifndef RADIANS
#define RADIANS(degrees) (FLOAT(degrees) * PI / 180)
#endif

extern int random(int rmin, int rmax);
extern int random();
#define srandom srand
#define _BV(b) (1UL << (b))
// TODO: remove usage of PROGMEM, PGM_P, pgm_get_byte(), etc., everywhere
#define PROGMEM
#define PGM_P const char *
#define pgm_read_byte(addr) *addr

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

#endif
