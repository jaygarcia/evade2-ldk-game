// Game State
#include "GGameState.h"
#include "GGamePlayfield.h"
#include "GResources.h"
#include "GameState/player/GGameOver.h"
#include "GameState/player/GPlayerProcess.h"
#include "GHud.h"

#include "GPlayer.h"

#include <BMemoryStream.h>

#define DEBUGME
//#undef DEBUGME

// DEBUG_FINALBOSS causes wizard spawn to become final boss spawn so we can debug the final boss
#define DEBUG_FINALBOSS
#undef DEBUG_FINALBOSS

// info about the dungeons
#include "DungeonDefs.h"
const TInt NUM_DUNGEONS = sizeof(gDungeonDefs) / sizeof(TDungeonInfo);

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::Init() {
  strcpy(mText, "");
  strcpy(mName, "");
  mLevel = 0;
  mNextLevel = 0;
  mTileMapId = 0;

  mNextTileMapId = 0;
  mNextObjectsId = 0;
  mTimer = FRAMES_PER_SECOND * 1;
  mCameraTimer = 0;
  mGameOver = ENull;

  mGamePlayfield = mNextGamePlayfield = ENull;

  // Clear BObject programs
  GGamePlayfield::ResetCache();
  gViewPort->SetRect(TRect(0, 16, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));
  gViewPort->Offset(0, 16);

  gDisplay.SetColor(COLOR_TEXT_BG, 0, 0, 0);
  gDisplay.SetColor(COLOR_TEXT, 255, 255, 255);
  GPlayer::Init();
}

// Constructor
GGameState::GGameState() : BGameEngine(gViewPort) {

}

// Constructor
GGameState::GGameState(const char *aName) : BGameEngine(gViewPort) {
}

GGameState::~GGameState() = default;

GProcess *GGameState::AddProcess(GProcess *p) {
  mProcessList.AddProcess(p);
  return p;
}

void GGameState::TryAgain(TBool aExitDungeon) {

}

TBool GGameState::IsBossRoom() {
  return EFalse;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::PreRender() {
  gDisplay.SetColor(COLOR_WHITE, 255, 255, 255);
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::PositionCamera() {
}

static void fuel_gauge(BViewPort *vp, TInt x, TInt y, TInt stat, TInt stat_max, TUint8 color) {

}

void GGameState::PostRender() {
//  GHud::Render();
//
//  if (mGameOver) {
//    mGameOver->Run();
//    if (mGamePlayfield->MosaicActive()) {
//      delete mGameOver;
//      mGameOver = ENull;
//    } else {
//      return;
//    }
//  }
//
//  if (mGamePlayfield->MosaicActive()) {
//  }
//  else if (mGamePlayfield->MosaicDone()) {
//  }
//
//#ifdef DEBUG_MODE
//  if (mTimer-- < 0) {
//    mTimer = FRAMES_PER_SECOND * 1;
//  }
//
//  if (mText[0]) {
//    gDisplay.renderBitmap->DrawString(gViewPort, mText, gFont8x8, 4, 4, COLOR_TEXT, COLOR_TEXT_TRANSPARENT);
//  }
//#endif
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

TUint16 GGameState::MapWidth() {
  return 0;
}

TUint16 GGameState::MapHeight() {
  return 0;
}

void GGameState::GameLoop() {
//  for (bool &s : mGamePlayfield->mGroupState) {
//    s = ETrue;
//  }

  BGameEngine::GameLoop();
//
//  for (TInt s = 0; s < 16; s++) {
//    if (mGamePlayfield->mGroupState[s] == ETrue) {
//      mGamePlayfield->mGroupDone[s] = ETrue;
//    }
//  }
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

/**
 * This is safe to call from BProcess context.
 *
 * @param aDungeon  ID of dungeon (in gDungeonDefs)
 * @param aLevel    Level in dungeon
 */
void GGameState::NextLevel(const TInt16 aDungeon, const TInt16 aLevel) {

}

void GGameState::SetPlayfieldXYFromPlayer(TFloat aPlayerX, TFloat aPlayerY) {

}

/**
  * This is NOT safe to call from BProcess context
  */
void GGameState::LoadLevel(const char *aName, const TInt16 aLevel, TUint16 aTileMapId, TBool aSpawnObjects) {

}

TBool GGameState::PlayMusicForCurrentLevel() {
  return PlayLevelMusic(mDungeon, -1);
}

TBool GGameState::PlayLevelMusic(TInt16 aNextDungeon, TInt16 aSpawnedBoss) {

  TUint16 song = EMPTYSONG_XM;
  printf("aNextDungeon = %i\n", aNextDungeon);
  // For levels -- bosses get their own treatment!
//  if (aSpawnedBoss == -1) {
//    if (aNextDungeon == 0) {
//      song = OVERWORLD_XM;
//    }
//    else if (aNextDungeon == 1) {
//     song = DUNGEON_HOME_XM;
//    }
//    else if (aNextDungeon >= 2 && aNextDungeon <= 4) {
//      song = DUNGEON1_XM;
//    }
//    else if (aNextDungeon >= 13 && aNextDungeon <= 18) {
//      song = DUNGEON4_XM;
//    }
//    else if (aNextDungeon >= 36 && aNextDungeon <= 40) {
//      song = DUNGEON9_XM;
//    }
//  }
//  else {
//    if (aSpawnedBoss == ATTR_MID_BOSS_WATER || aSpawnedBoss == ATTR_MID_BOSS_ENERGY || aSpawnedBoss == ATTR_MID_BOSS_EARTH || aSpawnedBoss == ATTR_MID_BOSS_FIRE) {
//      song = BOSS_1_XM;
//    }
//    else if (aSpawnedBoss == ATTR_WIZARD_ENERGY || aSpawnedBoss == ATTR_WIZARD_WATER || aSpawnedBoss == ATTR_WIZARD_EARTH || aSpawnedBoss == ATTR_WIZARD_FIRE) {
//      song = BOSS_2_XM;
//    } else {
//      song = BOSS_3_XM;
//    }
//  }


  gSoundPlayer.PlayMusic(song);

  return ETrue;
}
/**
  * modify BObjectProgram, like when a door or chest has opened.
  *
  * aCode is the code to change the program step to, OR ATTR_KEEP to leave the code in the BObjectProgram unchanged.
  * aAttr is the attribute to change the program step to, OR ATTR_KEEP
  */
void GGameState::EndProgram(TInt aIp, TUint16 aCode, TUint16 aAttr) {

}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::GameOver() {
  mGameOver = new GGameOver(this);
  gSoundPlayer.PlayMusic(S10_GAME_OVER_XM);
  //gSoundPlayer.TriggerSfx(SFX_PLAYER_DEATH_WAV);
  gControls.Reset();
  GPlayer::mGameOver = ETrue;
}

/*******************************************************************************
 *******************************************************************************
 *******************************************************************************/

void GGameState::InitRemapSlots() {
}

// Load aBMP, and remap it to playfield's tilemap palette
void GGameState::RemapSlot(TUint16 aBMP, TUint16 aSlot, TInt16 aImageSize) {
}

TBool GGameState::SaveState() {
  return ETrue;
}

TBool GGameState::LoadState(const char *aGameName) {
  return ETrue;
}
