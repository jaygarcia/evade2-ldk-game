#ifndef MODITE_GPLAYERBULLETPROCESS_H
#define MODITE_GPLAYERBULLETPROCESS_H


#include <GameState/GProcess.h>

class GGameState;
class GPlayerBulletSprite;

class GPlayerBulletProcess : public GProcess {
public:
  GPlayerBulletProcess(GGameState *aGameState,  TBool aAltBullet);
  ~GPlayerBulletProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  GGameState *mGameState;
  GPlayerBulletSprite *mSprite;
  TFloat mAge;
  TFloat mRange;
  TFloat mX, mY, mZ, mState;
  TFloat mVX, mVY, mVZ, mTheta;
};


#endif //MODITE_GPLAYERBULLETPROCESS_H
