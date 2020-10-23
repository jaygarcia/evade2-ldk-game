#ifndef PALETTE_H
#define PALETTE_H

// Borrowed from: https://jonasjacek.github.io/colors/data.json



extern void set_color(uint8_t index, uint16_t red, uint16_t green,
                      uint16_t blue);
extern void register_palette();
#endif
