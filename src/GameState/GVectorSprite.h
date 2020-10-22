#ifndef EVADE2_GVECTORSPRITE_H
#define EVADE2_GVECTORSPRITE_H

#include "Game.h"


struct vec_segment_u8 {
  int8_t x0;
  int8_t y0;
  int8_t x1;
  int8_t y1;
};

class GVectorSprite : public BSprite {

public:
  EXPLICIT GVectorSprite(TInt aPri = 0, TUint16 aBM = 0, TUint16 aIMG = 0, TUint32 aType = STYPE_DEFAULT) : BSprite(aPri, aBM, aIMG, aType) {
    mColor = COLOR_WHITE;
    mPad = 0;
    mX = mY = mZ = 0;
    mVX = mVY = mVZ = 0;
    mFlags = mState = mTheta = 0;
    mTimer = 0;
    lines = ENull;

  }

  void StartAnimation() {
    printf("GVectorSprite::StartAnimation()\n");
  }

  TBool DrawVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y,
                                    TFloat theta, TFloat scaleFactor, TUint8 color) {
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
  const TInt8 *lines;
  //
  TFloat mX, mY, mZ;    // coordinates
  TFloat mVX, mVY, mVZ; // velocity in x,y,z
  TUint8 mFlags;
  TInt8 mTimer;
  TInt16 mState; // arbitrary data TInt8 for AI use (can be explosion step, etc.)
  TInt16 mTheta; // rotation around Z (in degrees, 0-60)
  TUint8 mColor;
  TUint8 mPad;

  inline void SetType(TUint8 aType) {
    mFlags = (mFlags & ~OFLAG_TYPE_MASK) | aType;
  }

  inline TUint8 Type() {
    return mFlags & OFLAG_TYPE_MASK;
  }
};


#endif //EVADE2_GVECTORSPRITE_H
