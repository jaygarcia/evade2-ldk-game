#include "GPlayerProcess.h"
#include "GPlayer.h"
#include "GPlayerSprite.h"
#include "GGamePlayfield.h"
#include "GStatProcess.h"
#include "GResources.h"
#include "GPlayerBulletProcess.h"
#include "GBossProcess.h"
#include "Camera.h"
#define DEBUGME
#undef DEBUGME


const TUint16 IDLE_STATE = 0;
const TUint16 FALL_STATE = 4;
const TInt16 BLINK_TIME = FRAMES_PER_SECOND;


GPlayerProcess::GPlayerProcess(GGameState *aGameState) : GProcess(ATTR_PLAYER_IN1), mGameState(aGameState) {
  mState = IDLE_STATE;
  mStep = 0;
  mStepFrame = 0;
  mBlinkTimer = 0;
  GPlayer::mInvulnerable = EFalse;
  GPlayer::mSprite = mSprite = ENull;

  // initialize player sprite
  GPlayer::mSprite = mSprite = new GPlayerSprite();
  mGameState->AddSprite(mSprite);


  NewState(IDLE_STATE);
  printf("GPlayerProcess()\n");
}

GPlayerProcess::~GPlayerProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    GPlayer::mSprite = mSprite = ENull;
    GPlayer::mProcess = ENull;
  }
}

void GPlayerProcess::NewState(TUint16 aState) {
//  mStat
}

/*
  ____ _   _    _    _   _  ____ _____   ____ _____  _  _____ _____
 / ___| | | |  / \  | \ | |/ ___| ____| / ___|_   _|/ \|_   _| ____|
| |   | |_| | / _ \ |  \| | |  _|  _|   \___ \ | | / _ \ | | |  _|
| |___|  _  |/ ___ \| |\  | |_| | |___   ___) || |/ ___ \| | | |___
 \____|_| |_/_/   \_\_| \_|\____|_____| |____/ |_/_/   \_\_| |_____|
 */
TBool GPlayerProcess::MaybeHit() {


  return EFalse;
}

/*____ _____  _  _____ _____ ____
/ ___|_   _|/ \|_   _| ____/ ___|
\___ \ | | / _ \ | | |  _| \___ \
 ___) || |/ ___ \| | | |___ ___) |
|____/ |_/_/   \_\_| |_____|____/
*/
TBool GPlayerProcess::IdleState() {
  // collision?
  mStepFrame = 0;
//  if (MaybeHit()) {
//    return ETrue;
//  }
//
//  if (MaybeSpell()) {
//    return ETrue;
//  }
//
//  if (MaybeSword()) {
//    return ETrue;
//  }
//
//  MaybeWalk();
  return ETrue;
}

TBool GPlayerProcess::RunBefore() {



  return ETrue;
//  switch (mState) {
//    case IDLE_STATE:
//      return IdleState();
//    case WALK_STATE:
//      return WalkState();
//    case SWORD_CHARGE_STATE:
//    case SWORD_ATTACK_STATE:
//      return SwordState();
//    case FALL_STATE:
//      return FallState();
//    case HIT_HARD_STATE:
//    case HIT_MEDIUM_STATE:
//    case HIT_LIGHT_STATE:
//      return HitState();
//    case SPELL_STATE:
//      return SpellState();
//    default:
//      return ETrue;
//  }
}

TBool GPlayerProcess::RunAfter() {
//  if (GPlayer::mTargeted) {
//    if (GPlayer::mTargeted->Clipped() || !gControls.IsPressed(CONTROL_TARGET)) {
//      GPlayer::mTargeted = ENull;
//    }
  if (gControls.WasPressed(CONTROL_SHOOT) && GPlayer::mNumBullets < GPlayer::mMaxBullets) {
    printf("CONTROL_SHOOT\n");
//    new GPlayerSprite();
    mGameState->AddProcess(new GPlayerBulletProcess(mGameState));
  }

  if (gControls.IsPressed(CONTROL_BOOST)) {
    if (GPlayer::mBoost > 0) {
      Camera::mVZ = CAMERA_VZ * 2;
      GPlayer::mBoost--;
      if (GPlayer::mBoost < 0) {
        GPlayer::mBoost = 0;
      }
      else {
        if (! gGame->mStarField->mBoostSpeed) {
          gGame->mStarField->mBoostSpeed = ETrue;
          gSoundPlayer.TriggerSfx(SFX_SPEED_BOOST_WAV, 4);
        }
      }
    } else {
      Camera::mVZ = CAMERA_VZ;
    }
  }
  else {
    gGame->mStarField->mBoostSpeed = EFalse;
    Camera::mVZ = CAMERA_VZ;
    GPlayer::mBoost++;
    if (GPlayer::mBoost > GPlayer::mMaxHitPoints) {
      GPlayer::mBoost = GPlayer::mMaxHitPoints;
    }
  }


  return ETrue;
}

void GPlayerProcess::WriteCustomToStream(BMemoryStream &aStream) {
}

void GPlayerProcess::ReadCustomFromStream(BMemoryStream &aStream) {
}
