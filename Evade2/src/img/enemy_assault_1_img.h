#ifndef ENEMY_ASSAULT_1_IMG_H
#define ENEMY_ASSAULT_1_IMG_H
// SVG Graphic source: svg_docs/output_from_ai/assault-1.svg
// Number bytes 59
#include "BTypes.h"

const struct enemy_assault_1_img {
	TUint8 w;
  TUint8 h;
  TUint8 r;
	TInt8 data[14*4];
} enemy_assault_1_img = {
	.w = 128,    // Width (128 px)
	.h = 46,    // Height (54 px)
	.r = 8,    // Number of rows of coords (14)
	.data = {
//  x0,     y0,    x1,    y1
    -9,    -23,    37,    23,
    -37,    23,    9,    -23,
    -9,    -23,    -27,    -5,
    -27,    -5,    -9,    14,
    9,    14,    27,    -5,
    27,    -5,    9,    -23,
    -37,    23,    -64,    -5,
    37,    23,    64,    -5,
},
};
#endif

