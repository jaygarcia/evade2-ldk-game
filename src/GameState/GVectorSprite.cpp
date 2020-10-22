#include "GVectorSprite.h"
#include "Camera.h"


TBool GVectorSprite::ExplodeVectorGraphic(const TInt8 *graphic, TFloat x, TFloat y,
                           TFloat theta, TFloat scaleFactor, TInt8 step, TUint8 color) {
  graphic += 2;
  TBool drawn = false;

  TInt8 numRows = pgm_read_byte(graphic++);

  TFloat rad = TFloat(theta) * 3.1415926 / 180, sint = sin(rad), cost = cos(rad);

  for (TInt8 i = 0; i < numRows; i++) {
    struct vec_segment_u8 seg;
    float x0, y0, x1, y1;

    memcpy(&seg, graphic, sizeof(seg));
    graphic += sizeof(seg);

    x0 = seg.x0;
    y0 = seg.y0;
    x1 = seg.x1;
    y1 = seg.y1;

    if (scaleFactor) {
      x0 /= scaleFactor;
      y0 /= scaleFactor;
      x1 /= scaleFactor;
      y1 /= scaleFactor;
    }

    if (step) {
      x0 = x0 + (seg.x0 / 8) * step;
      y0 = y0 + (seg.y0 / 8) * step;
      x1 = x1 + (seg.x0 / 8) * step;
      y1 = y1 + (seg.y0 / 8) * step;
    }

    TBool xInBounds = (x0 >= 0) && (x0 <= DISPLAY_WIDTH) && (y0 >= 0) && (y0 < DISPLAY_HEIGHT),
        yInBounds = (x1 >= 0) && (x1 <= DISPLAY_WIDTH) && (y1 >= 0) && (y1 < DISPLAY_HEIGHT);

    if ((! xInBounds) && !(yInBounds)) {
      return drawn = EFalse;
    }

    gDisplay.renderBitmap->DrawLine(ENull, x0 * cost - y0 * sint + x, y0 * cost + x0 * sint + y,
                                    x1 * cost - y1 * sint + x, y1 * cost + x1 * sint + y, color);
  }
  return drawn;
}

TBool GVectorSprite::Render(BViewPort *aViewPort) {
  if (!lines || mZ <= Camera::mZ) {
    // nothing to draw
    return EFalse;
  }

  TFloat zz = (mZ - Camera::mZ) * 2;
  TFloat ratio = 128 / (zz + 128);

  bool isEnemy = Type() == OTYPE_ENEMY;
  // printf("is enemy = %i\n", isEnemy);
  TFloat cx = (Camera::mX - mX) * ratio + SCREEN_WIDTH / 2;
  TFloat cy = (Camera::mY - mY) * ratio + SCREEN_HEIGHT / 2;

  // uint8_t color = isEnemy ? 5 : 255;

  if (flags & OFLAG_EXPLODE) {
      ExplodeVectorGraphic(lines, cx, cy, TFloat(mTheta), 1 / ratio, mState, this->mColor);
  }
  else {
    bool drawn = DrawVectorGraphic(lines, cx, cy, TFloat(mTheta), 1 / ratio, this->mColor);
    if ((!drawn) && isEnemy) {

      // draw radar blip
      TFloat dx = Camera::mX - mX,
          dy = Camera::mY - mY,
          angle = atan2(dy, dx);


//        Gfx::FillCircle(
//            (int16_t)(SCREEN_WIDTH / 2 + cos(angle) * 75),
//            (int16_t)(SCREEN_HEIGHT / 2 + sin(angle) * 75),
//            3,
//            EBULLET_COLOR
//        );

    }
  }

  return ETrue;
}
