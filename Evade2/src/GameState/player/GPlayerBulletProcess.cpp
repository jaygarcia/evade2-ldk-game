#include <GResources.h>
#include "GPlayerBulletProcess.h"
#include "GPlayer.h"
#include "Camera.h"

#include "player_bullet_img.h"

static const TInt16 BULLET_VZ = 15;
static const TInt16 BULLET_MAX_Z = 512;

class GPlayerBulletSprite : public GVectorSprite {
public:
  EXPLICIT GPlayerBulletSprite(TInt16 aDeltaX, TInt16 aDeltaY, TBool alt = EFalse) : GVectorSprite() {
    mZ = Camera::mZ - 75;
    mColor = COLOR_HUD_FG;
    mTimer = 0;

    SetCMask(STYPE_ENEMY | STYPE_OBJECT | STYPE_EBULLET);
    SetFlags(SFLAG_CHECK);
    if (alt) {
      mX = Camera::mX + 28;
      mY = Camera::mY - 28;
      mState = 20;
    } else {
      mX = Camera::mX - 28;
      mY = Camera::mY - 28;
      mState = -20;
    }

    mVX = aDeltaX;
    mVY = aDeltaY;
    mVZ = Camera::mVZ + BULLET_VZ;
    mLines = player_bullet_img;

  }

  TBool Render(BViewPort *aViewPort) OVERRIDE {
    mTimer++;

    mTheta += mState;

    TFloat zz = (mZ - Camera::mZ);

    if (zz > BULLET_MAX_Z) {
      return EFalse;
    }

    TFloat ratio = 128 / (zz + 128);

    TFloat cx = (Camera::mX - mX) * ratio + SCREEN_WIDTH / 2;
    TFloat cy = (Camera::mY - mY) * ratio + SCREEN_HEIGHT / 2;
    TBool drawn = EFalse;

    if (flags & OFLAG_EXPLODE) {
      drawn = ExplodeVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, mState, this->mColor);
    }
    else {
      drawn = DrawVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, this->mColor);
    }

    return drawn;
  }
};

GPlayerBulletProcess::GPlayerBulletProcess(GGameState *aGameState, TBool aAltBullet = EFalse)
    : GProcess(ATTR_GONE) {

  mGameState = aGameState;
  TInt16 deltaX = 0, deltaY = 0;

  deltaX = gControls.IsPressed(CONTROL_JOYRIGHT) ? -10 : deltaX;
  deltaX = gControls.IsPressed(CONTROL_JOYLEFT) ? 10 : deltaX;
  deltaY = gControls.IsPressed(CONTROL_JOYUP) ? -9 : deltaY;
  deltaY = gControls.IsPressed(CONTROL_JOYDOWN) ? 11 : deltaY;

//  Bullet::fire(deltaX, deltaY, Player::flags & PLAYER_FLAG_ALT);

  mSprite = new GPlayerBulletSprite(deltaX, deltaY, aAltBullet);
  mGameState->AddSprite(mSprite);
  GPlayer::mNumBullets++;
  gSoundPlayer.TriggerSfx(SFX_PLAYER_SHOOT_WAV, 3);
}

GPlayerBulletProcess::~GPlayerBulletProcess() {
  if (mSprite) {
    GPlayer::SubtractBullet();
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GPlayerBulletProcess::RunBefore() {
//  mSprite->Render(gViewPort);
  if ((mSprite->mZ - Camera::mZ) > BULLET_MAX_Z) {
//    mSprite->Remove();
    return EFalse;
  }
printf("GPlayerBulletProcess RunBefore\n");
  return ETrue;
}

TBool GPlayerBulletProcess::RunAfter() {
//  TBool drawn = mSprite->Render(gViewPort);;
//  if (! drawn) {
//    printf("Oh shit\n");
//  }
  return ETrue;//drawn;
}
