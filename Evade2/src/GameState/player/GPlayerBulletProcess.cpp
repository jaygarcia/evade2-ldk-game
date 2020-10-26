#include <GResources.h>
#include "GPlayerBulletProcess.h"
#include "GPlayer.h"
#include "Camera.h"

#include "player_bullet_img.h"

static const TInt16 BULLET_VZ = 15;

class GPlayerBulletSprite : public GVectorSprite {
public:
  EXPLICIT GPlayerBulletSprite(TInt16 aDeltaX, TInt16 aDeltaY, TBool alt = EFalse) : GVectorSprite() {
    mZ = Camera::mZ - 175;
    mColor = COLOR_HUD_FG;
    mTimer = 0;
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
    alt = !alt;
    mVZ = Camera::mVZ + BULLET_VZ;
    mLines = player_bullet_img;
  }

  TBool Render(BViewPort *aViewPort) OVERRIDE {
    mTimer++;

//    if (mZ - Camera::mZ > 512) {
//
//      return EFalse;
//    }
//    else {
    mTheta += mState;
//    }

    TFloat zz = (mZ - Camera::mZ);

    if (zz > 512) {
      return EFalse;
    }
    printf("zz=%0.2f\n", zz);

    TFloat ratio = 128 / (zz + 128);

    // printf("is enemy = %i\n", isEnemy);
    TFloat cx = (Camera::mX - mX) * ratio + SCREEN_WIDTH / 2;
    TFloat cy = (Camera::mY - mY) * ratio + SCREEN_HEIGHT / 2;


    // uint8_t color = isEnemy ? 5 : 255;
    TBool drawn = EFalse;

    if (flags & OFLAG_EXPLODE) {
      drawn = ExplodeVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, mState, this->mColor);
    }
    else {
      drawn = DrawVectorGraphic(mLines, cx, cy, mTheta, 1 / ratio, this->mColor);
    }

    if (! drawn) {
      GPlayer::mNumBullets--;
    }

    printf("GPlayerBullet Render = %i\n", drawn);
    return drawn;
  }
};

GPlayerBulletProcess::GPlayerBulletProcess(GGameState *aGameState)
    : GProcess(ATTR_GONE) {

  mGameState = aGameState;
  TInt16 deltaX = 0, deltaY = 0;

  deltaX = gControls.IsPressed(CONTROL_JOYRIGHT) ? -12 : deltaX;
  deltaX = gControls.IsPressed(CONTROL_JOYLEFT) ? 12 : deltaX;

  deltaY = gControls.IsPressed(CONTROL_JOYUP) ? -11 : deltaY;
  deltaY = gControls.IsPressed(CONTROL_JOYDOWN) ? 13 : deltaY;

//  Bullet::fire(deltaX, deltaY, Player::flags & PLAYER_FLAG_ALT);

  mSprite = new GPlayerBulletSprite(deltaX, deltaY);
  mGameState->AddSprite(mSprite);
  GPlayer::mNumBullets++;
  gSoundPlayer.TriggerSfx(SFX_PLAYER_SHOOT_WAV, 3);
}

GPlayerBulletProcess::~GPlayerBulletProcess() {
  if (mSprite) {
    GPlayer::mNumBullets--;
    if (GPlayer::mNumBullets < 0) {
      GPlayer::mNumBullets = 0;
    }
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GPlayerBulletProcess::RunBefore() {
//  mSprite->Render(gViewPort);
  return ETrue;
}

TBool GPlayerBulletProcess::RunAfter() {
//  TBool drawn = mSprite->Render(gViewPort);;
//  if (! drawn) {
//    printf("Oh shit\n");
//  }
  return ETrue;//drawn;
}
