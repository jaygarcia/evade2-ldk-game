#include "Game.h"
#include "GPlayer.h"

TUint32 GPlayer::mNextLevel;
TUint32 GPlayer::mExperience;
TInt16 GPlayer::mHitPoints;
TInt16 GPlayer::mMaxHitPoints;
TBool GPlayer::mInvulnerable;

TInt32 GPlayer::mMaxMana;

TBool GPlayer::mGameOver;

GPlayerProcess *GPlayer::mProcess;
GPlayerSprite *GPlayer::mSprite;
GGameState *GPlayer::mGameState;
GBossProcess *GPlayer::mActiveBoss;
GAnchorSprite *GPlayer::mTargeted;

void GPlayer::WriteToStream(BMemoryStream &stream) {
}

void GPlayer::ReadFromStream(BMemoryStream &stream) {
}

void GPlayer::Dump() {
  printf("GPlayer\n");
}

TUint16 GPlayer::GetSpellSlot() {
  return 0;
}
