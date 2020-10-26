#ifndef MODITE_GSTARFIELDPROCESS_H
#define MODITE_GSTARFIELDPROCESS_H

#include "BProcess.h"
//#include "Game.h"
static const TInt8 NUM_STARS = 90;

//
//bool faster;
//bool slower;

static TUint32 actualTime = 0;
static TInt32 speedMills;

static const TInt16 STAR_SPEED_MIN = 1; // Minimum movement in pixels per update. (value is inclusive)
static const TInt16 STAR_SPEED_MAX = 15; // Maximum movement in pixels per update. (value is inclusive)

static const TUint16 RANDOM_Z_MIN = 400;
static const TUint16 RANDOM_Z_MAX = 600;


class GStar {
public:
  GStar() {
    mX = 0;
    mY = 0;
    mZ = 0;
    mSpeed = 0;
    mScreenX = 0;
    mScreenY = 0;
    mOldScreenX = 0;
    mOldScreenY = 0;
  }

  void Randomize(TInt16 xMin, TInt16 xMax, TInt16 yMin, TInt16 yMax, TInt16 zMin, TInt16 zMax, TInt16 speedMin, TInt16 speedMax)  {
    mX = Random(xMin, xMax);
    mY = Random(yMin, yMax);
    mZ = Random(zMin, zMax);
    mSpeed = Random(speedMin, speedMax + 1);
  }
  TFloat mX, mY, mZ, mSpeed;
  TInt16 mScreenX, mScreenY, mOldScreenX, mOldScreenY;
};



static GStar stars[NUM_STARS];  // An array of star instances.
static TUint8 colors[NUM_STARS];

class GStarFieldProcess {
public:
  explicit GStarFieldProcess() {
    mCurrSpeed = 10;
    mMinSpeed = 1;
    mBoostSpeed = EFalse;
    mWarp = EFalse;

    for (TInt i = 0; i < NUM_STARS; i++) {
      InitStar(i, 0);
    }
  }

  ~GStarFieldProcess() = default;

  void Render();

  void Animate() {
//    for (TInt i = 0; i < NUM_STARS; i++) {
//      if (mStarX[i] < -10) {
//        InitStar(i);
//      }
//      else {
//        mStarX[i] -= mStarSpeed[i];
//      }
//    }
  }
  TBool mWarp;
  TBool mBoostSpeed;

protected:

  void InitStar(TInt aIndex, TInt32 aXStartIndex = 319) {
    stars[aIndex].Randomize(
      -500,
      1000,
      -500,
      1000,
      0,
      1,
      STAR_SPEED_MIN,
      STAR_SPEED_MAX
    );
//    colors[aIndex] = 255 - Random(0,128);

  }

//  TFloat mStarX[NUM_STARS]{};
//  TFloat mStarY[NUM_STARS]{};
//  TFloat mStarZ[NUM_STARS]{};
//  TFloat mStarWidth[NUM_STARS]{};
//  TFloat mStarSpeed[NUM_STARS]{};
  TInt mCurrSpeed;
  TInt mMinSpeed;

};


#endif //MODITE_GSTARFIELDPROCESS_H
