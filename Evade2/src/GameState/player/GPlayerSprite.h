#ifndef MODITE_GPLAYERSPRITE_H
#define MODITE_GPLAYERSPRITE_H

#include "GVectorSprite.h"

class GPlayerSprite : public GVectorSprite {
public:
  EXPLICIT GPlayerSprite();

public:
  TBool Render(BViewPort *aViewPort) OVERRIDE;
public:
};

#endif //MODITE_GPLAYERSPRITE_H
