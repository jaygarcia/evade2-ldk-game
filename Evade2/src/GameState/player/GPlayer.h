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
const TInt DEFAULT_PLAYER_MANA = 100;


struct GPlayer {
  static void Init() {
    printf("Init GPlayer\n");
    mLevel = 1;
    mNextLevel = 100;
    mExperience = 0;
    mMaxHitPoints = DEFAULT_PLAYER_HITPOINTS;
    mHitPoints = mMaxHitPoints;
    mInvulnerable = EFalse;
    mAttackStrength = 35;
    mMaxMana = DEFAULT_PLAYER_MANA;
    mManaPotion = mMaxMana;

    //
    mGameOver = EFalse;
    mActiveBoss = ENull;
    mTargeted = mClosestEnemy = ENull;
  }

  static void AddExperience(TInt aExperience) {
    mExperience += aExperience;
    while (mExperience >= mNextLevel) {
      mLevel++;
      mExperience -= mNextLevel;
      mNextLevel += 100 + (mLevel - 1) * 50;
      mMaxHitPoints += 40;
      mMaxMana += 20;
      mHitPoints = mMaxHitPoints;
      mManaPotion = mMaxMana;
      mAttackStrength += 7;
//      //gSoundPlayer.TriggerSfx(SFX_PLAYER_LEVEL_UP_WAV, 2);
    }
  }

  static void AddHitPoints(TInt aMoreHitpoints, const TPoint &aPoint = GPlayer::mSprite->Center()) {
    gGame->CurrentState()->AddProcess(new GStatProcess(STAT_HEAL, aPoint,
      "%d", MIN(aMoreHitpoints, mMaxHitPoints - mHitPoints)));

    mHitPoints += aMoreHitpoints;
    if (mHitPoints > mMaxHitPoints) {
      mHitPoints = mMaxHitPoints;
    }
  }

  static void AddMana(TInt aMoreMana, const TPoint &aPoint = GPlayer::mSprite->Center()) {
    gGame->CurrentState()->AddProcess(new GStatProcess(STAT_MANA, aPoint,
      "%d", MIN(aMoreMana, mMaxMana - mManaPotion)));

    mManaPotion += aMoreMana;
    if (mManaPotion > mMaxMana) {
      mManaPotion = mMaxMana;
    }
  }

  static void WriteToStream(BMemoryStream &stream);
  static void ReadFromStream(BMemoryStream &stream);
  static TUint16 GetSpellSlot();

  static TUint32 mLevel;
  static TUint32 mNextLevel, mExperience;
  static TInt16 mHitPoints, mMaxHitPoints;
  static TBool mInvulnerable;
  static TInt32 mAttackStrength;
  static TInt32 mManaPotion, mMaxMana;
  static GPlayerProcess *mProcess;
  static GPlayerSprite *mSprite;
  static GGameState *mGameState;
  static TBool mGameOver;
  static GBossProcess *mActiveBoss;
  static GAnchorSprite *mClosestEnemy, *mTargeted;

  static void Dump();
};

#endif //MODITE_GPLAYER_H
