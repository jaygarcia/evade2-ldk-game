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

const TInt DEFAULT_PLAYER_HITPOINTS = 100;


struct GPlayer {
  static void Init() {
    printf("Init GPlayer\n");
//    mBoost = DEFAULT_PLAYER_HITPOINTS;
    mHitPoints = mMaxHitPoints = mBoost = DEFAULT_PLAYER_HITPOINTS;
    mInvulnerable = EFalse;
    mGameOver = EFalse;
    mActiveBoss = ENull;

  }



  static void WriteToStream(BMemoryStream &stream);
  static void ReadFromStream(BMemoryStream &stream);

  static TInt16 mHitPoints, mBoost, mMaxHitPoints;

  static TBool mInvulnerable;
  static GPlayerProcess *mProcess;
  static GPlayerSprite *mSprite;
  static GGameState *mGameState;
  static TBool mGameOver;
  static GBossProcess *mActiveBoss;

  static void Dump();
};

#endif //MODITE_GPLAYER_H
