#define DEBUGME

#include "Evade2.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include <driver/adc.h>



static UWORD buttonsState() {
  UWORD state = 0;

  int joyX = adc1_get_raw(ODROID_GAMEPAD_IO_X),
      joyY = adc1_get_raw(ODROID_GAMEPAD_IO_Y);

  if (joyX > 2048 + 1024) {
    state |= JOYSTICK_LEFT;
  }
  else if (joyX > 1024) {
    state |= JOYSTICK_RIGHT;
  }
  if (joyY > 2048 + 1024) {
    state |= JOYSTICK_UP;
  }
  else if (joyY > 1024) {
    state |= JOYSTICK_DOWN;
  }

  state |= gpio_get_level(ODROID_GAMEPAD_IO_START) ? 0 : BUTTON_START;

  state |= gpio_get_level(ODROID_GAMEPAD_IO_SELECT) ? 0 : BUTTON_SELECT;

  state |= gpio_get_level(ODROID_GAMEPAD_IO_A) ? 0 : BUTTON_A;
  state |= gpio_get_level(ODROID_GAMEPAD_IO_B) ? 0 : BUTTON_B;

  state |= gpio_get_level(ODROID_GAMEPAD_IO_MENU) ? 0 : BUTTON_MENU;
  state |= gpio_get_level(ODROID_GAMEPAD_IO_VOLUME) ? 0 : BUTTON_VOLUME;
  return state;
}

UWORD Controls::rkeys = 0;
UWORD Controls::ckeys = 0;
UWORD Controls::dkeys = 0;

void Controls::init() {
  gpio_set_direction(ODROID_GAMEPAD_IO_SELECT, GPIO_MODE_INPUT);
  gpio_set_pull_mode(ODROID_GAMEPAD_IO_SELECT, GPIO_PULLUP_ONLY);

  gpio_set_direction(ODROID_GAMEPAD_IO_START, GPIO_MODE_INPUT);

  gpio_set_direction(ODROID_GAMEPAD_IO_A, GPIO_MODE_INPUT);
  gpio_set_pull_mode(ODROID_GAMEPAD_IO_A, GPIO_PULLUP_ONLY);

  gpio_set_direction(ODROID_GAMEPAD_IO_B, GPIO_MODE_INPUT);
  gpio_set_pull_mode(ODROID_GAMEPAD_IO_B, GPIO_PULLUP_ONLY);

  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(ODROID_GAMEPAD_IO_X, ADC_ATTEN_11db);
  adc1_config_channel_atten(ODROID_GAMEPAD_IO_Y, ADC_ATTEN_11db);

  gpio_set_direction(ODROID_GAMEPAD_IO_MENU, GPIO_MODE_INPUT);
  gpio_set_pull_mode(ODROID_GAMEPAD_IO_MENU, GPIO_PULLUP_ONLY);

  gpio_set_direction(ODROID_GAMEPAD_IO_VOLUME, GPIO_MODE_INPUT);
}

void Controls::reset() { rkeys = ckeys = dkeys = 0; }

void Controls::run() {
  UWORD buttons = buttonsState();
  // printf("buttons = %i | \n", buttons);

  dkeys |= (buttons ^ ckeys) & buttons;
  ckeys = buttons;
  rkeys = buttons;
}

BOOL Controls::debounced(UWORD key) {
  BOOL v = dkeys & key;
  dkeys &= ~key;
  return v;
}

BOOL Controls::pressed(UWORD key) { return ckeys & key; }
