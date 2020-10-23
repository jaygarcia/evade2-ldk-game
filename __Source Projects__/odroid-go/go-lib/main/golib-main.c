/* 
    go-lib main.
    This file is here to satisify the compiler.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"


void app_main()
{
    while (true) {
        printf("What are you doing, Dave?\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    // Should never get here!

    esp_restart();
}
