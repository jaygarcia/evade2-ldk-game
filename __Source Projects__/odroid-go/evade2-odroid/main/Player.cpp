#define DEBUGME
//#undef DEBUGME

#include "Evade2.h"
#include "Starfield.h"

#include "../go-lib/Gfx.h"
#include "img/hud_console_img.h"

#include <stdio.h>

// TODO: Put in own files
PROGMEM const unsigned char crosshair_left_4x8[] = {
    // width, height4, 8,
    0x81, 0x42, 0x24, 0x99};

// crosshair_right.png
// 4x8
PROGMEM const unsigned char crosshair_right_4x8[] = {
    // width, height 4, 8,
    0x99, 0x24, 0x42, 0x81};

#define MAX_POWER 100
#define MAX_LIFE 100

BYTE Player::shield = -1, Player::power = -1, Player::num_bullets = 0;

UBYTE Player::flags = 0;

void Player::init() {
  Camera::vz = CAMERA_VZ;
  power = MAX_POWER;
  shield = MAX_LIFE;
  num_bullets = 0;
  flags = 0;
}

void Player::hit(BYTE amount) {
  shield -= amount;
  if (shield <= 0) {
    ProcessManager::birth(GameOver::entry);
  } else {
    Player::flags |= PLAYER_FLAG_HIT;
    Sound::play_sound(SFX_PLAYER_HIT_BY_ENEMY);
  }
}

void Player::recharge_shield() {
  if (shield < MAX_LIFE) {
    shield++;
  }
}

void Player::recharge_power() {
  if (power < MAX_POWER) {
    power++;
  }
}

void Player::before_render() {
  if (game_mode != MODE_GAME) {
    Camera::vx = Camera::vy = 0;
    return;
  }

  if (Controls::debounced(BUTTON_A)) {
    BYTE deltaX = 0, deltaY = 0;

    deltaX = Controls::pressed(JOYSTICK_RIGHT) ? -12 : deltaX;
    deltaX = Controls::pressed(JOYSTICK_LEFT) ? 12 : deltaX;

    deltaY = Controls::pressed(JOYSTICK_UP) ? -11 : deltaY;
    deltaY = Controls::pressed(JOYSTICK_DOWN) ? 13 : deltaY;

    Bullet::fire(deltaX, deltaY, Player::flags & PLAYER_FLAG_ALT);
    Player::flags ^= PLAYER_FLAG_ALT;
  }

  if (Controls::pressed(BUTTON_B)) {
    if (power > 0) {
      Camera::vz = CAMERA_VZ * 2;
      power--;
      if (power < 0) {
        power = 0;
      }
      else {
        Sound::play_sound(SFX_SPEED_BOOST);
        Starfield::boostSpeed = true;
      }
    } else {
      Camera::vz = CAMERA_VZ;
    }
  } else {
    Starfield::boostSpeed = false;
    Camera::vz = CAMERA_VZ;
    power++;
    if (power > MAX_POWER) {
      power = MAX_POWER;
    }
  }

  if (Controls::pressed(JOYSTICK_RIGHT)) {
    Camera::vx = -DELTACONTROL;
  } else if (Controls::pressed(JOYSTICK_LEFT)) {
    Camera::vx = DELTACONTROL;
  } else {
    Camera::vx = 0;
  }

  if (Controls::pressed(JOYSTICK_DOWN)) {
    Camera::vy = DELTACONTROL;
  } else if (Controls::pressed(JOYSTICK_UP)) {
    Camera::vy = -DELTACONTROL;
  } else {
    Camera::vy = 0;
  }
}

/************************************************************************/
/** HUD */
/************************************************************************/

#ifdef ENABLE_HUD_MOVEMENTS

// 13 == full. Anything less, and we draw "less meter"
static void drawMeter(BYTE side, WORD value, WORD deltaXMeter,
                      BYTE deltaYMeter) {

  // start at X:14
  // Draw 2 lines, skip one line, iterate 13 total times
  // if left, X:0, else X:128
  // Y Step is 3

  // TODO: Tighten up!
  WORD y = 160;
  short yStep = 10;

  value /= 10;
  if (side == 0) { // LEFT
    for (BYTE i = 0; i < 10; i++) {
      if (i >= value) {
        Gfx::FillRect(1 + deltaXMeter, y + deltaYMeter, 3, 5, 255);
      } 
      else {
        Gfx::FillRect(1 + deltaXMeter, y + deltaYMeter, 4, 3, 255);
        Gfx::FillRect(1 + deltaXMeter, y + deltaYMeter + 2, 6, 3, 255);
      }

      y -= yStep;
    }
  } else { // RIGHT
    for (BYTE i = 0; i < 10; i++) {
      if (i >= value) {
        Gfx::FillRect(310 + deltaXMeter, y + deltaYMeter, 3, 5, 255);
        // Gfx::DrawPixel(316 + deltaXMeter, y + deltaYMeter, 255);
        // Gfx::DrawPixel(316 + deltaXMeter, y + 1 + deltaYMeter, 255);
      } else {
        Gfx::FillRect(310 + deltaXMeter, y + deltaYMeter, 4, 3, 255);
        Gfx::FillRect(308 + deltaXMeter, y + deltaYMeter + 2, 6, 3, 255);
        // Gfx::DrawLine(314 + deltaXMeter, y + deltaYMeter, 316 + deltaXMeter,
        // y + deltaYMeter, 255); Gfx::DrawLine(313 + deltaXMeter, y + 1 +
        // deltaYMeter, 316+ deltaXMeter, y + 1 + deltaYMeter, 255);
      }
      y -= yStep;
    }
  }
}

#else

// 13 == full. Anything less, and we draw "less meter"
static void drawMeter(BYTE side, BYTE value) {

  BYTE y = 45;
  value /= 10;
  if (side == 0) { // LEFT
    for (BYTE i = 0; i < 10; i++) {
      if (i >= value) {
        Gfx::DrawPixel(0, y);
        Gfx::DrawPixel(0, y + 1);
      } else {
        Gfx::DrawLine(0, y, 2, y);
        Gfx::DrawLine(0, y + 1, 3, y + 1);
      }
      y -= 3;
    }
  } else { // RIGHT
    for (BYTE i = 0; i < 10; i++) {
      if (i >= value) {
        Gfx::DrawPixel(127, y);
        Gfx::DrawPixel(127, y + 1);
      } else {
        Gfx::drawLine(126, y, 128, y);
        Gfx::drawLine(125, y + 1, 128, y + 1);
      }
      y -= 3;
    }
  }
}

#endif // #if ENABLE_HUD_MOVEMENTS

static void drawHud(WORD x, WORD y) {

  const uint8_t color = COLOR_WHITE;
  const WORD width = 0x30, height = 0x08;
  const uint8_t *bitmap = hud_console_img;

  for (WORD xx = 0, xxx = 0; xx < width; xx++, xxx += 2) {
    for (WORD yy = 0, yyy = 0; yy < height; yy++, yyy += 2) {
      if (y + yy > 239)
        continue;
      uint8_t byte = bitmap[xx + (yy / 8)];
      uint8_t bit = (yy % 8);
      if (byte & (1 << bit)) {
        //        Graphics::drawPixel(x + xx, y + yy, color);
        Graphics::drawPixel(x + xxx, y + yyy, color);
        Graphics::drawPixel(x + xxx + 1, y + yyy, color);
        Graphics::drawPixel(x + xxx, y + yyy + 1, color);
        Graphics::drawPixel(x + xxx + 1, y + yyy + 1, color);
        //        Graphics::drawPixel(x + xx * 2 + 1, y + yy * 2 + 1, color);
      }
    }
  }
}
void Player::after_render() {

  if (flags & PLAYER_FLAG_HIT) {
    Gfx::FillScreen(255);
    Graphics::display(false);
  }
  flags &= ~PLAYER_FLAG_HIT;

#ifdef ENABLE_HUD_MOVEMENTS
  WORD consoleX = 0, consoleY = 0, deltaXMeter = 0, deltaYMeter = 0,
       deltaXCrossHairs = 0, deltaYCrossHairs = 0;


  if (game_mode == MODE_GAME) {

    if (Controls::pressed(JOYSTICK_RIGHT)) {
      consoleX = -4;
      deltaXMeter = -1;
      deltaXCrossHairs = 4;
    } else if (Controls::pressed(JOYSTICK_LEFT)) {
      consoleX = 4;
      deltaXMeter = 1;
      deltaXCrossHairs = -4;
    }

    if (Controls::pressed(JOYSTICK_UP)) {
      consoleY = -4;
      deltaYMeter = -1;
      deltaYCrossHairs = 4;
    } else if (Controls::pressed(JOYSTICK_DOWN)) {
      consoleY = 4;
      deltaYMeter = 1;
      deltaYCrossHairs = -4;
    }
  }

  const uint16_t screenMidX = 160, screenMidY = 120;

  drawHud(screenMidX - (0x30) + consoleX, (240 + consoleY) - 12);

  /** Reticle **/
  // Top left
  Gfx::DrawLine(screenMidX + deltaXCrossHairs - 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs - 12,
                screenMidY + deltaYCrossHairs - 7, 255);

  Gfx::DrawPixel(screenMidX + deltaXCrossHairs - 5,
                 screenMidY + deltaYCrossHairs - 5, 255);

  // Top Right
  Gfx::DrawLine(screenMidX + deltaXCrossHairs + 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs + 12,
                screenMidY + deltaYCrossHairs - 7, 255);

  Gfx::DrawPixel(screenMidX + deltaXCrossHairs + 5,
                 screenMidY + deltaYCrossHairs - 5, 255);

  // Bottom Right
  Gfx::DrawLine(screenMidX + deltaXCrossHairs + 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs + 12,
                screenMidY + deltaYCrossHairs + 7, 255);

  Gfx::DrawPixel(screenMidX + deltaXCrossHairs + 5,
                 screenMidY + deltaYCrossHairs + 5, 255);

  // Bottom left
  Gfx::DrawLine(screenMidX + deltaXCrossHairs - 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs - 12,
                screenMidY + deltaYCrossHairs + 7, 255);

  Gfx::DrawPixel(screenMidX + deltaXCrossHairs - 5,
                 screenMidY + deltaYCrossHairs + 5, 255);


  drawMeter(0, shield, deltaXMeter, deltaYMeter);
  drawMeter(1, power, deltaXMeter, deltaYMeter);

#else
  Graphics::drawBitmap(40, 58, hud_console_img, 0x30, 0x08);

  drawMeter(0, shield);
  drawMeter(1, power);
#endif
}
