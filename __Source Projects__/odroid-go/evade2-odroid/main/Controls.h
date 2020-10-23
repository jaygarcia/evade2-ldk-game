#ifndef CONTROLS_H
#define CONTROLS_H

class Controls;

#include "Evade2.h"
#include "../go-lib/Gfx.h"

#define ODROID_GAMEPAD_IO_X ADC1_CHANNEL_6
#define ODROID_GAMEPAD_IO_Y ADC1_CHANNEL_7
#define ODROID_GAMEPAD_IO_SELECT GPIO_NUM_27
#define ODROID_GAMEPAD_IO_START GPIO_NUM_39
#define ODROID_GAMEPAD_IO_A GPIO_NUM_32
#define ODROID_GAMEPAD_IO_B GPIO_NUM_33
#define ODROID_GAMEPAD_IO_MENU GPIO_NUM_13
#define ODROID_GAMEPAD_IO_VOLUME GPIO_NUM_0

#define JOYSTICK_UP (1 << 0)
#define JOYSTICK_DOWN (1 << 1)
#define JOYSTICK_LEFT (1 << 2)
#define JOYSTICK_RIGHT (1 << 3)
#define BUTTON_A (1 << 4)
#define BUTTON_B (1 << 5)
#define BUTTON_MENU (1 << 6)
#define BUTTON_VOLUME (1 << 7)
#define BUTTON_SELECT (1 << 8)
#define BUTTON_START (1 << 9)
#define JOYSTICK_ANY                                                           \
  (JOYSTICK_UP | JOYSTICK_DOWN | JOYSTICK_LEFT | JOYSTICK_RIGHT)
#define BUTTON_ANY (BUTTON_A | BUTTON_B)

/**
 * Controls
 *
 * run() is called once per game loop to read the keys.
 *
 * ckeys = currrent keys
 * dkeys = debounced keys
 * rkeys = TRUE keys
 *
 * If journal is playing back, ckeys and dkeys will be the values in the
 * journal.  If you are
 * truly interested in the true buttons, use rkeys.
 *
 * ckeys bits are true if the button is currently down.
 * dkeys bits are latched.  They get set true if the button is down and stay
 * latched even if the button is released.  The code must reset the bits to
 * detect the next button down. You can use the debounced() method to test and
 * reset button bit(s).
 */
class Controls {
public:
  static UWORD ckeys, dkeys, rkeys;

public:
  static void init();
  static void reset();
  static void run();
  static BOOL debounced(UWORD key);
  static BOOL pressed(UWORD key);
};

#endif
