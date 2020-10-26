#ifndef EVADE2_GVECTORSPRITE_H
#define EVADE2_GVECTORSPRITE_H

#include "BSprite.h"
#include "GResources.h"

struct vec_segment_u8 {
  TInt8 x0;
  TInt8 y0;
  TInt8 x1;
  TInt8 y1;
};

#define FIRE_TIME (60 / Game::difficulty + random(1, 60 / Game::Game::difficulty))


class GVectorSprite : public BSprite {

public:
  GVectorSprite() : BSprite(0 , 0, ENull, STYPE_DEFAULT) {
    mColor = COLOR_WHITE;
    mPad = 0;
    mX = mY = mZ = 0;
    mVX = mVY = mVZ = 0;
    mFlags = mState = mTheta = 0;
    mTimer = 0;
    mLines = ENull;
  }

  void StartAnimation() {
    printf("GVectorSprite::StartAnimation()\n");
  }

  TBool DrawVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y, TFloat theta, TFloat scaleFactor, TUint8 color) {
    return ExplodeVectorGraphic(graphic, x, y, theta, scaleFactor, 0, color);
  }

  TBool ExplodeVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y,
                             TFloat theta, TFloat scaleFactor, TInt8 step, TUint8 color);

  TBool Render(BViewPort *aViewPort) OVERRIDE;

  void Animate() OVERRIDE {
    mX += mVX;
    mY += mVY;
    mZ += mVZ;
  }

  // if lines is NULL, then the variables in the Object structure can be used
  // for any purpose
  const TInt8 *mLines;

  TFloat mX, mY, mZ;    // coordinates
  TFloat mVX, mVY, mVZ; // velocity in x,y,z
  TUint8 mFlags;
  TInt16 mTimer;
  TInt16 mState; // arbitrary data TInt8 for AI use (can be explosion step, etc.)
  TFloat mTheta; // rotation around Z (in degrees, 0-60)
  TUint8 mColor;
  TUint8 mPad;

  inline void SetType(TUint8 aType) {
    mFlags = (mFlags & ~OFLAG_TYPE_MASK) | aType;
  }

  TBool BehindCamera();

  TBool do_death() {
//    if (o->flags & OFLAG_COLLISION) {
//      Game::kills++;
//      o->flags &= OFLAG_EXPLODE;
//      o->state = 0;
//      // o->vz = Camera::vz - 3;
//      Sound::play_sound(SFX_ENEMY_EXPLODE);
//      return TRUE;
//    }
    return EFalse;
  }

  inline TUint8 Type() {
    return mFlags & OFLAG_TYPE_MASK;
  }
};


#endif //EVADE2_GVECTORSPRITE_H
