#ifndef MODITE_GAME_H
#define MODITE_GAME_H

#define pgm_read_byte(addr) *addr


#define FRAME_RATE_INFO 0
#undef FRAME_RATE_INFO

#define ENABLE_AUDIO
//#undef ENABLE_AUDIO

#define ENABLE_ROTATING_TEXT
//#undef ENABLE_ROTATING_TEXT

#define ENABLE_OPTIONS
// #undef ENABLE_OPTIONS

// Debug mode
#define DEBUG_MODE
// #undef DEBUG_MODE

// special characters
#define STR_LEFT_ARROW "\xf"
#define STR_RIGHT_ARROW "\xe"

#ifndef PI
#define PI 3.1415926
#endif

#ifndef RADIANS
#define RADIANS(degrees) (FLOAT(degrees) * PI / 180)
#endif

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <CreativeEngine.h>

const TUint16 FACTOR = FRAMES_PER_SECOND / 30;

#include "GResources.h"
#include "GGame.h"
#include "common/GDialogWidget.h"
#include "common/GButtonWidget.h"
#include "TOptions.h"
#include "GSoundPlayer.h"

// GGameEngine states
#include "GSplashState.h"
#include "GGameState.h"
#include "GMainMenuState.h"
#include "GMainOptionsState.h"
#include "GResetOptionsState.h"
#include "GGameMenuState.h"
#include "GAttractModeState.h"
#include "GVictoryState.h"
#include "DebugMenuState/GDebugMenuState.h"
#include "GCreditsState.h"
//#include "GInventory.h"

extern BViewPort   *gViewPort;
extern TOptions    *gOptions;


/**
 * Control bits definitions
 */
const TUint16 CONTROL_JOYUP = JOYUP;
const TUint16 CONTROL_JOYDOWN = JOYDOWN;
const TUint16 CONTROL_JOYLEFT = JOYLEFT;
const TUint16 CONTROL_JOYRIGHT = JOYRIGHT;
const TUint16 CONTROL_SHOOT = BUTTONA | BUTTONB;
const TUint16 CONTROL_BOOST = BUTTONX | BUTTONY;
const TUint16 CONTROL_INVENTORY = BUTTONR;
const TUint16 CONTROL_DEBUG = BUTTON_SELECT;

struct EnemyConfig {
  EnemyConfig() {
    mX = mY = mZ = 0;
    mVX = mVY = mVZ = 0;
  }
  TInt16 mX;
  TInt16 mY;
  TInt16 mZ;
  TInt16 mVX;
  TInt16 mVY;
  TInt16 mVZ;
};

#endif //MODITE_GAME_H
