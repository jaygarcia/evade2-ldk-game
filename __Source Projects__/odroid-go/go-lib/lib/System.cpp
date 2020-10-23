//System.cpp
#include "System.h"



int ledState = 0;


void System::init() {
  printf("System::init()\n");
  //Todo: Add any SD Card initialization calls

  rtc_gpio_deinit(ODROID_GAMEPAD_IO_MENU);

  // Turn OFF the Blue LED
  gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_2, 0);
}

void System::toggleBlueLED() {
  ledState = (ledState == 0) ? 1 : 0;

  // printf("LED is %s\n", (ledState == 0) ? "OFF" : "ON");
  gpio_set_level(GPIO_NUM_2, ledState);
}

