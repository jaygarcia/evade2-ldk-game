#ifndef MODITE_GPLAYERPROCESS_H
#define MODITE_GPLAYERPROCESS_H

class GGamePlayfield;
class GGameState;
class BMemoryStream;

//class GAnchorSprite;

#include "Game.h"
#include "GPlayerSprite.h"
#include "GProcess.h"
#include "GGame.h"


class GPlayerProcess : public GProcess {
public:
  EXPLICIT GPlayerProcess(GGameState *aGameState);

  ~GPlayerProcess() OVERRIDE;

private:
  void StartBlink(TUint16 aTime) {
    mBlinkTimer = aTime;
  }

public:
  void StartLevel(GGamePlayfield *aPlayfield, TFloat aX, TFloat aY, TInt16 aExitingDungeon = OVERWORLD_DUNGEON, TInt16 aExitingLevel = -1);

  GVectorSprite *Sprite() { return mSprite; }

public:
  TBool RunBefore() OVERRIDE;

  TBool RunAfter() OVERRIDE;

protected:
  TBool IsLedge();

  TBool CanWalk(TFloat aVx, TFloat aVy);

  void StartKnockback();

protected:
  void NewState(TUint16 aState);

  TBool MaybeHit();


protected:
  TBool IdleState();


public:
  void WriteCustomToStream(BMemoryStream &aStream) OVERRIDE;
  void ReadCustomFromStream(BMemoryStream &aStream) OVERRIDE;

public:
//  static TUint16 mLastDirection;

protected:
  GGameState *mGameState;
//  GAnchorSprite *mSprite, *mSprite2;
  GPlayerSprite *mSprite;
  TUint16 mState;
  TUint16 mStep;
  TUint16 mStepFrame;
  TUint16 mBlinkTimer;
  TBool mAltBullet;
};

#endif // MODITE_GPLAYERPROCESS_H
