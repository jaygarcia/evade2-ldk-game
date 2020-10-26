#include <Display.h>
#include <BResourceManager.h>
#include "GResources.h"
#include "GPlayerSprite.h"
#include "GPlayer.h"
#include "img/console2_img.h"

enum {
  HUD_SIDE_LEFT,
  HUD_SIDE_RIGHT
};

static void drawMeter(TInt8 side, TInt16 value, TInt16 deltaXMeter, TInt8 deltaYMeter) {

  // start at X:14
  // Draw 2 lines, skip one line, iterate 13 total times
  // if left, X:0, else X:128
  // Y Step is 3

  // TODO: Tighten up!
  TInt16 y = 160;
  TFloat yStep = 10;

  value /= 10;
  if (side == HUD_SIDE_LEFT) { // LEFT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        gDisplay.renderBitmap->FillRect(ENull, 4 + deltaXMeter, y + deltaYMeter, 4 + deltaXMeter + 3,  y + deltaYMeter + 5, COLOR_HUD_FG);
      }
      else {
        gDisplay.renderBitmap->FillRect(ENull, 4 + deltaXMeter, y + deltaYMeter, 4 + deltaXMeter + 4, y + deltaYMeter + 3, COLOR_HUD_FG);
        gDisplay.renderBitmap->FillRect(ENull, 4 + deltaXMeter, y + deltaYMeter + 2, 4 + deltaXMeter + 6, y + deltaYMeter + 3, COLOR_HUD_FG);
      }

      y -= yStep;
    }
  }
  else { // RIGHT
    for (TInt8 i = 0; i < 10; i++) {
      if (i >= value) {
        gDisplay.renderBitmap->FillRect(ENull, 310 + deltaXMeter, y + deltaYMeter, 310 + deltaXMeter + 3, y + deltaYMeter + 5, COLOR_HUD_FG);
      }
      else {
        gDisplay.renderBitmap->FillRect(ENull, 310 + deltaXMeter, y + deltaYMeter, 310 + deltaXMeter + 4, y + deltaYMeter + 3, COLOR_HUD_FG);
        gDisplay.renderBitmap->FillRect(ENull, 308 + deltaXMeter, y + deltaYMeter + 2, 308 + deltaXMeter + 6, y + deltaYMeter + 3, COLOR_HUD_FG);
      }
      y -= yStep;
    }
  }
}


static void drawHud(TInt16 x, TInt16 y) {

  const uint8_t color = COLOR_HUD_FG;
  const TInt16 width = 0x30, height = 0x08;
  const uint8_t *bitmap = hud_console_img;

  for (TInt16 xx = 0, xxx = 0; xx < width; xx++, xxx += 2) {
    for (TInt16 yy = 0, yyy = 0; yy < height; yy++, yyy += 2) {
      if (y + yy > 239)
        continue;
      uint8_t byte = bitmap[xx + (yy / 8)];
      uint8_t bit = (yy % 8);
      if (byte & (1 << bit)) {
        gDisplay.renderBitmap->WritePixel(x + xxx, y + yyy, color);
        gDisplay.renderBitmap->WritePixel(x + xxx + 1, y + yyy, color);
        gDisplay.renderBitmap->WritePixel(x + xxx, y + yyy + 1, color);
        gDisplay.renderBitmap->WritePixel(x + xxx + 1, y + yyy + 1, color);
      }
    }
  }
}


GPlayerSprite::GPlayerSprite() : GVectorSprite() {
  type = STYPE_PLAYER;
  SetCMask(STYPE_ENEMY | STYPE_EBULLET | STYPE_OBJECT); // collide with enemy, enemy attacks, and environment
  SetFlags(SFLAG_ANCHOR | SFLAG_CHECK); // SFLAG_SORTY
}

TBool GPlayerSprite::Render(BViewPort *aViewPort) {
  printf("GPlayerSprite::Render()\n");

//
//  if (flags & PLAYER_FLAG_HIT) {
//    Gfx::FillScreen(COLOR_HUD_FG);
//    Graphics::display(false);
//  }
//  flags &= ~PLAYER_FLAG_HIT;

  TInt16 consoleX = 0, consoleY = 0, deltaXMeter = 0, deltaYMeter = 0,
       deltaXCrossHairs = 0, deltaYCrossHairs = 0;


  if (gGame->GetState() == GAME_STATE_GAME) {

    if (gControls.IsPressed(CONTROL_JOYRIGHT)) {
      consoleX = -4;
      deltaXMeter = -1;
      deltaXCrossHairs = 4;
    }
    else if (gControls.IsPressed(CONTROL_JOYLEFT)) {
      consoleX = 4;
      deltaXMeter = 1;
      deltaXCrossHairs = -4;
    }

    if (gControls.IsPressed(CONTROL_JOYUP)) {
      consoleY = -4;
      deltaYMeter = -1;
      deltaYCrossHairs = 4;
    }
    else if (gControls.IsPressed(CONTROL_JOYDOWN)) {
      consoleY = 4;
      deltaYMeter = 1;
      deltaYCrossHairs = -4;
    }
  }

  const uint16_t screenMidX = 160, screenMidY = 120;

  drawHud(screenMidX - (0x30) + consoleX, (240 + consoleY) - 12);

  /** Reticle **/
  // Top left
  gDisplay.renderBitmap->DrawLine(ENull, screenMidX + deltaXCrossHairs - 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs - 12,
                screenMidY + deltaYCrossHairs - 7, COLOR_HUD_FG);

  gDisplay.renderBitmap->WritePixel(screenMidX + deltaXCrossHairs - 5,
                 screenMidY + deltaYCrossHairs - 5, COLOR_HUD_FG);

  // Top Right
  gDisplay.renderBitmap->DrawLine(ENull, screenMidX + deltaXCrossHairs + 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs + 12,
                screenMidY + deltaYCrossHairs - 7, COLOR_HUD_FG);

  gDisplay.renderBitmap->WritePixel(screenMidX + deltaXCrossHairs + 5,
                 screenMidY + deltaYCrossHairs - 5, COLOR_HUD_FG);

  // Bottom Right
  gDisplay.renderBitmap->DrawLine(ENull, screenMidX + deltaXCrossHairs + 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs + 12,
                screenMidY + deltaYCrossHairs + 7, COLOR_HUD_FG);

  gDisplay.renderBitmap->WritePixel(screenMidX + deltaXCrossHairs + 5,
                 screenMidY + deltaYCrossHairs + 5, COLOR_HUD_FG);

  // Bottom left
  gDisplay.renderBitmap->DrawLine(ENull, screenMidX + deltaXCrossHairs - 5,
                screenMidY + deltaYCrossHairs,
                screenMidX + deltaXCrossHairs - 12,
                screenMidY + deltaYCrossHairs + 7, COLOR_HUD_FG);

  gDisplay.renderBitmap->WritePixel(screenMidX + deltaXCrossHairs - 5,
                 screenMidY + deltaYCrossHairs + 5, COLOR_HUD_FG);


  drawMeter(HUD_SIDE_LEFT, GPlayer::mHitPoints, deltaXMeter, deltaYMeter);
  drawMeter(HUD_SIDE_RIGHT, GPlayer::mBoost, deltaXMeter, deltaYMeter);



  return ETrue;
}

