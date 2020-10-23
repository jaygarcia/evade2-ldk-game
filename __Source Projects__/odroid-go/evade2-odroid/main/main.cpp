#define DEBUGME

#include "Evade2.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <stdlib.h>
#include "../go-lib/Audio.h"

#include "time.h"

#define SHOW_FPS
// #undef SHOW_FPS

#ifdef SHOW_FPS
long previousTime = 0;
uint8_t fps = 0, fpsCounter = 0;
#endif

unsigned long millis() { return (unsigned long)(esp_timer_get_time() / 1000); }

// Global variables.
UBYTE game_mode;
bool warpMode;
int random(int rmin, int rmax) { return (rand() % (rmax - rmin) + rmin); }
int random() { return rand(); }
// using const saves RAM - we know what the frame rate is, so we may as well
// hard code it, saving the RAM in the process.
static const BYTE eachFrameMillis = 1000 / FRAMERATE;
// We only need this one variable to track when to idle() while waiting for the
// next frame to start.
static ULONG nextFrameStart = 0;


#ifdef MIKE_REMOVED_THIS
static BOOL pressed(UBYTE buttons) {
  return (false);
}

#endif

BOOL paused = false;

extern "C" {

void app_main() {
  Controls::init();

  Gfx::CreateFrameBuffers();
  
  Sound::init();

  Gfx::Init();
  Gfx::Clear();

  // initialize color palette 128-255 to greys
  int c = 50;
  for (int i = 128; i < 255; i++) {
    c += 10;
    // printf("Gfx::SetColor(%i, %i,%i,%i)\n",i,i,i,i);
    if (c > 200) {
      c = 5;
    }
    Gfx::SetColor(i, c,c,c);
  }

  Gfx::SetColor(COLOR_BLACK, 0, 0, 0);
  Gfx::SetColor(COLOR_BLUE, 255, 0, 0);
  Gfx::SetColor(COLOR_GREEN, 0, 0, 255);
  Gfx::SetColor(COLOR_CYAN, 0, 255, 255);
  Gfx::SetColor(COLOR_RED, 0,  255, 0);
  Gfx::SetColor(COLOR_MAGENTA, 255, 255, 0);
  Gfx::SetColor(COLOR_BROWN, 0, 75, 150);
  Gfx::SetColor(COLOR_WHITE, 255, 255, 255);

  Gfx::SetColor(BULLET_COLOR, 0, 0, 255);
  Gfx::SetColor(EBULLET_COLOR, 0, 255, 255);
  Gfx::SetColor(BOSS_COLOR, 0, 255, 0);
  Gfx::SetColor(ASSAULT_COLOR, 255, 0, 255);
  Gfx::SetColor(BOMBER_COLOR, 255, 0, 255);
  Gfx::SetColor(SCOUT_COLOR,255, 128, 255);

  Gfx::SetColor(255, 255, 255, 255);
  Gfx::SetColor(SHMOO_COLOR, 255, 255, 255);

  Starfield::init();
  ProcessManager::init();
  ObjectManager::init();

#ifdef ENABLE_MODUS_LOGO
  ProcessManager::birth(Logo::entry);
#else
  ProcessManager::birth(Splash::entry);
#endif
  nextFrameStart = millis();

  // loop
  while (true) {
#if 0
    Graphics::fillScreen(COLOR_CYAN);
    Graphics::display(TRUE);
    continue;
#endif
    ULONG now = millis();
    // pause render until it's time for the next frame
    // if (now < nextFrameStart) {
    //   continue;
    // }

    nextFrameStart = now + eachFrameMillis;

    Controls::run();
    if (Controls::debounced(BUTTON_MENU) || Controls::debounced(BUTTON_START)) {
      paused = !paused;
      if (paused) {
        Audio::Mute();
      }
      else {
        Audio::Unmute();
      }
    }

    if (paused) {
      vTaskDelay(5);
      continue;
    }

    // shmoo += (128 + 31);
    // shmoo &= 0xffffff;
    // Gfx::SetColor(SHMOO_COLOR, (shmoo >> 16) & 0xff, (shmoo >> 8) & 0xff, shmoo & 0xff);
    // Gfx::SetColor(SHMOO_COLOR, 255, 255, 255);

    Camera::move();
    if (game_mode == MODE_GAME || game_mode == MODE_NEXT_WAVE) {
      Player::before_render();
    }
#ifdef ENABLE_MODUS_LOGO
    if (game_mode != MODE_LOGO) {
      Starfield::render();
    }
#else
    Starfield::render();
#endif
    ProcessManager::run();
    ObjectManager::run();

    if (game_mode == MODE_GAME || game_mode == MODE_NEXT_WAVE) {
      // process player bullets
      Bullet::run();
      
      // process enemy bullets
      EBullet::run();
      
      if (game_mode != MODE_NEXT_WAVE) {
        // process wave status
        Game::run();
      }

      // handle any player logic needed to be done after guts of game loop (e.g.
      // render hud, etc.)
      Player::after_render();
    }

    if (Controls::debounced(BUTTON_VOLUME)) {
      // printf("Audio Volume Change\n"); fflush(stdout);
      Audio::SetVolume(Audio::GetVolume() + .031f);
    }

#ifdef SHOW_FPS
    fpsCounter++;
    long actualTime = millis();
    
    if ((fpsCounter % FRAMERATE) == 0) {
      if (previousTime != 0) {
        fps = (FRAMERATE * 1000 / (actualTime - previousTime));
      }
      previousTime = actualTime;
      fpsCounter = 0;
    }

    float fontScale = Font::scale;
    Font::scale = 256;
    Font::printf(10, 10, "%d", fps);
    Font::scale = fontScale;
    // if (fpsCounter % 3 == 0)
    //   printf("FPS:%d\r", fps);
#endif

    // then we finaly we tell the arduboy to display what we just wrote to the
    // display
    // TODO instead of erasing the entire screen to black here, maybe we can
    // erase the stars and lines
    Graphics::display(TRUE);
    // vTaskDelay(1);
  }
}

// }
}
