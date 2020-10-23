#define DEBUGME

#include "Evade2.h"

#include "../go-lib/Gfx.h"

#include "img/radar_blip_img.h"
#include <stdio.h>

void Object::init()
{
  color = COLOR_WHITE;
  x = y = z = 0;
  vx = vy = vz = 0;
  flags = state = timer = theta = 0;
  lines = NULL;
}

void Object::move()
{
  x += vx;
  y += vy;
  z += vz;
}

void Object::draw()
{
  if (!lines || z <= Camera::z) {
    // nothing to draw
    return;
  }

  FLOAT zz = (z - Camera::z) * 2;
  FLOAT ratio = 128 / (zz + 128);

  bool isEnemy = get_type() == OTYPE_ENEMY;
  // printf("is enemey = %i\n", isEnemy);
  register FLOAT cx = (Camera::x - x) * ratio + SCREEN_WIDTH / 2;
  register FLOAT cy = (Camera::y - y) * ratio + SCREEN_HEIGHT / 2;

  // uint8_t color = isEnemy ? 5 : 255;

  if (flags & OFLAG_EXPLODE) {
    Graphics::explodeVectorGraphic(lines, cx, cy, FLOAT(theta), 1 / ratio, state, this->color);
  } else {
    bool drawn = Graphics::drawVectorGraphic(lines, cx, cy, FLOAT(theta), 1 / ratio, this->color);
    if ((!drawn) && isEnemy) {
      // draw radar blip
      FLOAT dx = Camera::x - x,
            dy = Camera::y - y,
            angle = atan2(dy, dx);


      Gfx::FillCircle(
        (int16_t)(SCREEN_WIDTH / 2 + cos(angle) * 75),
        (int16_t)(SCREEN_HEIGHT / 2 + sin(angle) * 75), 
        3, 
        EBULLET_COLOR
      );

    }
  }
}
