#ifndef CAMERA_H
#define CAMERA_H

#include "Game.h"
#include "GVectorSprite.h"


class Camera {
public:
  static TFloat mX, mY, mZ;
  static TFloat mVX, mVY, mVZ;

public:
  static void Move() {
    Camera::mX += Camera::mVX;
    Camera::mY += Camera::mVY;
    Camera::mZ += Camera::mVZ;
  }

  static TBool CollidesWith(GVectorSprite *aVSprite) {
    // If enemy bullet collides with player
    if (abs(aVSprite->mZ - Camera::mZ) < abs(aVSprite->mVZ) && abs(aVSprite->mX - Camera::mX) < 64 && abs(aVSprite->mY - Camera::mY) < 64) {
      return ETrue;
    }
    return EFalse;
  }
};



#endif
