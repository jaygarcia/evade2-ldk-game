#include "Game.h"
#include "GPlayer.h"

TInt16 GPlayer::mHitPoints;
TInt16 GPlayer::mBoost;
TInt16 GPlayer::mMaxHitPoints;
TInt8 GPlayer::mMaxBullets;
TInt8 GPlayer::mNumBullets;
TBool GPlayer::mInvulnerable;

TBool GPlayer::mGameOver;

GPlayerProcess *GPlayer::mProcess;
GPlayerSprite *GPlayer::mSprite;
GGameState *GPlayer::mGameState;
GBossProcess *GPlayer::mActiveBoss;


void GPlayer::Dump() {
  printf("GPlayer\n");
}
