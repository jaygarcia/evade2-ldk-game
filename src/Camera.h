#ifndef CAMERA_H
#define CAMERA_H

#include "Game.h"
#include "GVectorSprite.h"

//class Object;
//
//class Camera {
//public:
//    Camera() {
//      mX = mY = mZ = 0;
//      mVX = mVY = mVZ = 0;
//    }
//
//public:
//  static TFloat mX, mY, mZ;
//  static TFloat mVX, mVY, mVZ;
//
//  void Move() {
//    mX += mVX;
//    mY += mVY;
//    mZ += mVZ;
//  }
//
//  TBool CollidesWith(GVectorSprite *aVSprite) {
//    // If enemy bullet collides with player
//    if (abs(aVSprite->mZ - mZ) < abs(aVSprite->mVZ) && abs(aVSprite->mX - mX) < 64 && abs(aVSprite->mY - mY) < 64) {
//      return ETrue;
//    }
//    return EFalse;
//  }
//};

//extern Camera *gCamera;


//
//class GVectorSprite;
////class Object;
//
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



//extern Camera *gCamera;

#endif
