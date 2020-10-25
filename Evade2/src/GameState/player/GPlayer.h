/**
 * GPlayer
 *
 * Contains player info like experience, strength, gold, inventory, etc.
 */
#ifndef MODITE_GPLAYER_H
#define MODITE_GPLAYER_H

class GPlayerProcess;
class GBossProcess;

#include <BMemoryStream.h>
#include <BResourceManager.h>
#include "GGame.h"
#include "GGameState.h"
#include "GPlayerSprite.h"
#include "GAnchorSprite.h"
#include "GResources.h"
#include "GStatProcess.h"
#include "GSoundPlayer.h"

const TInt DEFAULT_PLAYER_HITPOINTS = 200;


struct GPlayer {
  static void Init() {
    printf("Init GPlayer\n");
    mNextLevel = 100;
    mMaxHitPoints = DEFAULT_PLAYER_HITPOINTS;
    mHitPoints = mMaxHitPoints;
    mInvulnerable = EFalse;

    //
    mGameOver = EFalse;
    mActiveBoss = ENull;
  }



  static void WriteToStream(BMemoryStream &stream);
  static void ReadFromStream(BMemoryStream &stream);
  static TUint16 GetSpellSlot();

  static TUint32 mNextLevel, mExperience;
  static TInt16 mHitPoints, mMaxHitPoints;
  static TBool mInvulnerable;
  static GPlayerProcess *mProcess;
  static GPlayerSprite *mSprite;
  static GGameState *mGameState;
  static TBool mGameOver;
  static GBossProcess *mActiveBoss;

  static void Dump();
};

#endif //MODITE_GPLAYER_H
