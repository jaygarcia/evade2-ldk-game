#include <GResources.h>
#include "GPlayerBulletProcess.h"
#include "GPlayer.h"

const TInt VELOCITY = 4;

class BulletSprite : public GVectorSprite {
public:
    EXPLICIT BulletSprite() : GVectorSprite() {

    }

    TBool Render(BViewPort *aViewPort) OVERRIDE {
      printf("TODO: BulletSprite::Render()\n");
      return ETrue;
    }
};

GPlayerBulletProcess::GPlayerBulletProcess(GGameState *aGameState, TFloat aMultiplier)
    : GProcess(ATTR_GONE) {
}

GPlayerBulletProcess::~GPlayerBulletProcess() {
  if (mSprite) {
    mSprite->Remove();
    delete mSprite;
    mSprite = ENull;
  }
}

TBool GPlayerBulletProcess::RunBefore() {
  return ETrue;
}

TBool GPlayerBulletProcess::RunAfter() {
  return ETrue;
}
