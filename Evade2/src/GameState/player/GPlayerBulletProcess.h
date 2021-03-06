#ifndef MODITE_GPLAYERBULLETPROCESS_H
#define MODITE_GPLAYERBULLETPROCESS_H


#include <GameState/GProcess.h>

class GGameState;
class BulletSprite;

class GPlayerBulletProcess : public GProcess {
public:
  GPlayerBulletProcess(GGameState *aGameState, TFloat aMultiplier);
  ~GPlayerBulletProcess() OVERRIDE;

public:
  TBool RunBefore() OVERRIDE;
  TBool RunAfter() OVERRIDE;

protected:
  GGameState *mGameState;
  BulletSprite *mSprite;
  TFloat mAge;
  TFloat mRange;
};


#endif //MODITE_GPLAYERBULLETPROCESS_H
