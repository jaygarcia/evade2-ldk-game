#include "Game.h"
#include "GPlayer.h"

TInt16 GPlayer::mHitPoints;
TInt16 GPlayer::mMaxHitPoints;
TBool GPlayer::mInvulnerable;

TBool GPlayer::mGameOver;

GPlayerProcess *GPlayer::mProcess;
GPlayerSprite *GPlayer::mSprite;
GGameState *GPlayer::mGameState;
GBossProcess *GPlayer::mActiveBoss;

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
