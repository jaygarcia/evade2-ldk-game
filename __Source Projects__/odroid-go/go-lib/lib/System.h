#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "driver/rtc_io.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"

#include "Constants.h"
#include "Input.h" // Do we need this?

class System {



public:

  // This doesn't work!
  // static int ledState;

  //
  static void init();

  static void toggleBlueLED();

};

// this doesn't work!
// int System::ledState = 0;

#endif