#include "../go-lib/Gfx.h" // Dirty hack. we should use Graphics, but i'm out of time!
#include "Evade2.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include <time.h>


#define STAR_SPEED_MIN 10 // Minimum movement in pixels per update. (value is inclusive)
#define STAR_SPEED_MAX 30 // Maximum movement in pixels per update. (value is inclusive)

bool faster;
bool slower;

long actualTime = 0;
long speedMills;
int starCount = NUM_STARS;


#define RANDOM_Z_MIN 400
#define RANDOM_Z_MAX 600

Star stars[NUM_STARS];  // An array of star instances.
UBYTE colors[NUM_STARS];

void Star::randomize(int xMin, int xMax, int yMin, int yMax, int zMin, int zMax, int speedMin, int speedMax) {
    x = random(xMin, xMax);
    y = random(yMin, yMax);
    z = random(zMin, zMax);
    speed = random(speedMin, speedMax + 1);
}

int Starfield::curSpeed = 10;
int Starfield::minSpeed = 1;
BOOL Starfield::boostSpeed = false;
BOOL Starfield::warp = false;

void Starfield::init() {

  for (int i = 0; i < starCount; i++) {
    stars[i].randomize(
        -500, 
        1000, 
        -500, 
        1000, 
        0, 
        1, 
        STAR_SPEED_MIN, 
        STAR_SPEED_MAX
    );
    colors[i] = 255 - random(0,128);
  }
}

void Starfield::render() {
    float travelX = 0,
          travelY = 0,
          travelZ = 0;

    // Todo: this needs to be in Player.cpp
    BOOL jsRight = Controls::pressed(JOYSTICK_RIGHT),
         jsLeft  = Controls::pressed(JOYSTICK_LEFT),
         jsUp    = Controls::pressed(JOYSTICK_UP),
         jsDown  = Controls::pressed(JOYSTICK_DOWN);

    if (game_mode == MODE_GAME) {
        // rotate
        if (jsUp) {
           travelY = .02;
        }

        if (jsDown) {
           travelY = -.02;
        }

        if (jsLeft) {
            travelZ = -.02;
        }

        if (jsRight) {
            travelZ = .02;
        }
 
    }


    if (warp) {
        travelX = .03;
    }

    // Loop through each star.
    for (int i = 0; i < starCount; i++) {
      
      stars[i].z -= curSpeed;

      if (travelY != 0) {
        float temp_y = stars[i].y;
        float temp_z = stars[i].z;
        stars[i].y = temp_y * cos(travelY) - temp_z * sin(travelY);
        stars[i].z = temp_z * cos(travelY) + temp_y * sin(travelY);
      }

      if (travelX != 0) {
        float temp_x = stars[i].x;
        float temp_y = stars[i].y;
        stars[i].x = temp_x * cos(travelX) - temp_y * sin(travelX);
        stars[i].y = temp_y * cos(travelX) + temp_x * sin(travelX);
      }

      if (travelZ != 0) {
        float temp_x = stars[i].x;
        float temp_z = stars[i].z;
        stars[i].x = temp_x * cos(travelZ) - temp_z * sin(travelZ);
        stars[i].z = temp_z * cos(travelZ) + temp_x * sin(travelZ);
      }      

      stars[i].screen_x = stars[i].x / stars[i].z * 100 + WIDTH / 2;
      stars[i].screen_y = stars[i].y / stars[i].z * 100 + HEIGHT / 2;
  
      actualTime = xTaskGetTickCount();
      if (boostSpeed && curSpeed <= STAR_SPEED_MAX && actualTime - speedMills >= 25) {
        curSpeed = curSpeed + 5;
        speedMills = actualTime;
      }

      if (!boostSpeed && curSpeed > STAR_SPEED_MIN && actualTime - speedMills >= 10) {
        curSpeed -= 5;
        speedMills = actualTime;
      }
      if (curSpeed < STAR_SPEED_MIN) {
        curSpeed = STAR_SPEED_MIN;
      }


      //If the stars go off the screen remove them and re-draw. If the stars hang out in the center remove them also
      if (stars[i].screen_x > WIDTH || stars[i].screen_x < 0 || stars[i].screen_y > HEIGHT || stars[i].screen_y < 0 || (stars[i].screen_x == WIDTH >> 1 && stars[i].screen_y == HEIGHT >> 1) ) {               
        int xMin = -1000,
            xMax = 1000,
            yMin = -500,
            yMax = 500;

        if ((game_mode == MODE_GAME) && (jsLeft || jsRight)) {
            xMin = -1000;
            xMax = 1000;
        }

        stars[i].randomize(
            xMin, 
            xMax, 
            yMin, 
            yMax, 
            RANDOM_Z_MIN,
            RANDOM_Z_MAX,
            STAR_SPEED_MIN, 
            STAR_SPEED_MAX
        );
  
        stars[i].screen_x = stars[i].x / stars[i].z * 100 + WIDTH / 2;
        stars[i].screen_y = stars[i].y / stars[i].z * 100 + HEIGHT / 2;
        stars[i].old_screen_x = stars[i].screen_x;
        stars[i].old_screen_y = stars[i].screen_y;
      }


      // Draw the star at its new coordinate.
      Gfx::DrawLine(
        stars[i].screen_x, 
        stars[i].screen_y, 
        stars[i].old_screen_x, 
        stars[i].old_screen_y, 
        colors[i]
      );
  
      //keep track of the old spot
      stars[i].old_screen_x = stars[i].screen_x;
      stars[i].old_screen_y = stars[i].screen_y;
  }
}
