#include "GStarFieldProcess.h"
#include "Game.h"

void GStarFieldProcess::Render() {


  float travelX = 0,
      travelY = 0,
      travelZ = 0;

  // Todo: this needs to be in Player.cpp or GGame.cpp
  TBool jsRight = gControls.IsPressed(JOYRIGHT),
      jsLeft = gControls.IsPressed(JOYLEFT),
      jsUp = gControls.IsPressed(JOYUP),
      jsDown = gControls.IsPressed(JOYDOWN),
      jsRButton = gControls.IsPressed(BUTTONR),
      jsLButton = gControls.IsPressed(BUTTONL);


  //Todo: ROTATE via L or R Buttons in Z dimension
  if (gGame->GetState() == GAME_STATE_GAME) {
    // rotate
    if (jsUp) {
      travelY = .02;
    }

    if (jsDown) {
      travelY = -.02;
    }

    if (jsLeft) {
      travelZ = -.02;
    }

    if (jsRight) {
      travelZ = .02;
    }

    if (jsLButton) {
      printf("TODO: Rotate stars & playfield on L button");
    }
    if (jsRButton) {
      printf("TODO: Rotate stars & playfield on R button");
    }

  }


    if (mWarp) {
      travelX = .03;
    }

    // Loop through each star.
    for (int i = 0; i < NUM_STARS; i++) {
      stars[i].mZ -= mCurrSpeed;

      if (travelY != 0) {
        float temp_y = stars[i].mY;
        float temp_z = stars[i].mZ;
        stars[i].mY = temp_y * cos(travelY) - temp_z * sin(travelY);
        stars[i].mZ = temp_z * cos(travelY) + temp_y * sin(travelY);
      }

      if (travelX != 0) {
        float temp_x = stars[i].mX;
        float temp_y = stars[i].mY;
        stars[i].mX = temp_x * cos(travelX) - temp_y * sin(travelX);
        stars[i].mY = temp_y * cos(travelX) + temp_x * sin(travelX);
      }

      if (travelZ != 0) {
        float temp_x = stars[i].mX;
        float temp_z = stars[i].mZ;
        stars[i].mX = temp_x * cos(travelZ) - temp_z * sin(travelZ);
        stars[i].mZ = temp_z * cos(travelZ) + temp_x * sin(travelZ);
      }

      stars[i].mScreenX = stars[i].mX / stars[i].mZ * 100 + SCREEN_WIDTH / 2;
      stars[i].mScreenY = stars[i].mY / stars[i].mZ * 100 + SCREEN_HEIGHT / 2;

      actualTime = Milliseconds();
      if (mBoostSpeed && mCurrSpeed <= STAR_SPEED_MAX && actualTime - speedMills >= 25) {
        mCurrSpeed = mCurrSpeed + 5;
        speedMills = actualTime;
      }

      if (!mBoostSpeed && mCurrSpeed > STAR_SPEED_MIN && actualTime - speedMills >= 10) {
        mCurrSpeed -= 5;
        speedMills = actualTime;
      }
      if (mCurrSpeed < STAR_SPEED_MIN) {
        mCurrSpeed = STAR_SPEED_MIN;
      }


      //If the stars go off the screen remove them and re-draw. If the stars hang out in the center remove them also
      if (stars[i].mScreenX > SCREEN_WIDTH || stars[i].mScreenX < 0 || stars[i].mScreenY > SCREEN_HEIGHT ||
          stars[i].mScreenY < 0 ||
          (stars[i].mScreenX == SCREEN_WIDTH >> 1 && stars[i].mScreenY == SCREEN_HEIGHT >> 1)) {
        int xMin = -1000,
            xMax = 1000,
            yMin = -500,
            yMax = 500;

        if ((gGame->GetState() == GAME_STATE_GAME) && (jsLeft || jsRight)) {
          xMin = -1000;
          xMax = 1000;
        }

        stars[i].Randomize(
            xMin,
            xMax,
            yMin,
            yMax,
            RANDOM_Z_MIN,
            RANDOM_Z_MAX,
            STAR_SPEED_MIN,
            STAR_SPEED_MAX
        );

        stars[i].mScreenX = stars[i].mX / stars[i].mZ * 100 + SCREEN_WIDTH / 2;
        stars[i].mScreenY = stars[i].mY / stars[i].mZ * 100 + SCREEN_HEIGHT / 2;
        stars[i].mOldScreenX = stars[i].mScreenX;
        stars[i].mOldScreenY = stars[i].mScreenY;
      }


      // Draw the star at its new coordinate.
      gDisplay.renderBitmap->DrawLine(
          ENull,
          stars[i].mScreenX,
          stars[i].mScreenY,
          stars[i].mOldScreenX,
          stars[i].mOldScreenY,
          STAR_COLOR
      );

      //keep track of the old spot
      stars[i].mOldScreenX = stars[i].mScreenX;
      stars[i].mOldScreenY = stars[i].mScreenY;
    }

}