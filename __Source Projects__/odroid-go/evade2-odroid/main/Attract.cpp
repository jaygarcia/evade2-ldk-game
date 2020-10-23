#define DEBUGME

#include "../go-lib/Gfx.h"
#include "Evade2.h"

static const BYTE TYPEWRITER_SPEED = 3;
static const BYTE LINE_HEIGHT = 26;

static const char scout_text[] = "SCOUT";
static const char bomber_text[] = "BOMBER";
static const char assault_text[] = "ASSAULT";

static const char credits1[] = "CRAFTED BY:\nMODUS CREATE\nDECEMBER 2017";
static const char credits2[] = "MUSIC and SFX:\nJ. GARCIA";
static const char credits3[] =
    "ART:\nM. TINTIUC\nJV DALEN\nJD JONES\nJ. GARCIA";
static const char credits4[] =
    "PROGRAMMING:\nM. SCHWARTZ\nJ. GARCIA\nM. TINTIUC\n";
static const char credits5[] =
    "PROGRAMMING:\nD. BRIGNOLI\nS. LEMMONS\nA. DENNIS";
static const char credits6[] = "PROGRAMMING:\nV. POPA\nL. STILL\nG. GRISOGONO";

const BYTE MAX_SCREEN = 2;
const BYTE MAX_CREDITS = 5;

struct attract_data {
  BYTE screen;
  BYTE x;
  BYTE y;
  BYTE offset;
  WORD timer;
  BOOL done;
  PGM_P text;
  BYTE enemy;
};

static void init_screen(attract_data *ad, WORD x = 20, WORD y = 30) {
  if (game_mode == MODE_ATTRACT) {
    switch (ad->screen) {
      case 0:
        ad->enemy = ENEMY_SCOUT;
        ad->text = scout_text;
        x = 125;
        y = 60;
        break;
      case 1:
        ad->enemy = ENEMY_BOMBER;
        ad->text = bomber_text;
        x = 117;
        y = 60;
        break;
      case 2:
        ad->enemy = ENEMY_ASSAULT;
        ad->text = assault_text;
        x = 110;
        y = 60;
        break;
    }
  }
  else {
    ad->enemy = -1;
    switch (ad->screen) {
      case 0:
        ad->text = credits1;
        break;
      case 1:
        ad->text = credits2;
        break;
      case 2:
        ad->text = credits3;
        break;
      case 3:
        ad->text = credits4;
        break;
      case 4:
        ad->text = credits5;
        break;
      case 5:
        ad->text = credits6;
        break;
    }
  }

  ad->offset = 1;
  ad->x = x;
  ad->y = y;
  ad->timer = TYPEWRITER_SPEED;
  ad->done = FALSE;
}

void Attract::next(Process *me, Object *o) {
  attract_data *ad = (attract_data *)&o->x;

  ad->timer--;
  if (ad->timer < 0) {
    ad->screen++;
    if ((game_mode == MODE_ATTRACT && ad->screen > MAX_SCREEN) ||
        (game_mode == MODE_CREDITS && ad->screen > MAX_CREDITS)) {
      ProcessManager::birth(Splash::entry);
      me->suicide();
      return;
    }
    else {
      Sound::play_sound(SFX_NEXT_ATTRACT_SCREEN);
      init_screen(ad);
      me->sleep(1, typewriter);
    }
  }
  else {
    me->sleep(1);
  }
}

void Attract::typewriter(Process *me, Object *o) {
  attract_data *ad = (attract_data *)&o->x;

  ad->timer--;

  if (Controls::debounced(BUTTON_A)) {
    ProcessManager::birth(Game::entry);
    me->suicide();
    return;
  }
  if (Controls::debounced(BUTTON_START)) {
    ad->timer = -1;
    me->sleep(1, next);
    return;
  }

  if (ad->timer < 0) {
    if (ad->done) {
      ad->timer = 5;
      me->sleep(1, next);
      return;
    }
    ad->timer = TYPEWRITER_SPEED;
    ad->offset++;
    Sound::play_sound(SFX_NEXT_ATTRACT_CHAR);
  }

  if (ad->enemy != -1) {
    Graphics::drawVectorGraphic(Enemy::enemy_graphic(ad->enemy),
                                SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1, 0.0,
                                .75, // Originally 2.0
                                COLOR_WHITE);
  }
  if (game_mode == MODE_CREDITS) {
    Font::scale = 2 * 256;
  }

  PGM_P p = ad->text;
  WORD x = ad->x, y = ad->y;

  for (BYTE i = 0; i < ad->offset;) {
    char c = *p++;
    if (c == '\0') {
      if (!ad->done) {
        ad->timer = 60; // 2 seconds
        ad->done = TRUE;
      }
      break;
    }
    else if (c == '\n') {
      x = 20;
      y += LINE_HEIGHT;
    }
    else {
      x += Font::write(x, y, c);
      i++;
    }
  }
  // if (game_mode == MODE_CREDITS) {
  Font::scale = 512;
  // }
  me->sleep(1);
}

void Attract::entry(Process *me, Object *o) {
  attract_data *ad = (attract_data *)&o->x;
  ad->screen = 0;
  init_screen(ad);
  Sound::play_sound(SFX_NEXT_ATTRACT_SCREEN);

  me->sleep(1, typewriter);
}
