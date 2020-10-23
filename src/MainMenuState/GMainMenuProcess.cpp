#include "GMainMenuProcess.h"

GMainMenuProcess::GMainMenuProcess() : BProcess() {
  mTimer = 10 * FRAMES_PER_SECOND;
  // mShowHighScores = ETrue;
  mContainer = new GMainMenuContainer(0, 0, this);
}

GMainMenuProcess::~GMainMenuProcess() {
  delete mContainer;
}

void GMainMenuProcess::ResetTimer() {
//  mTimer = TIMEOUT;
}

TBool GMainMenuProcess::RunBefore() {
  return ETrue;
}

TBool GMainMenuProcess::RunAfter() {
  mContainer->Render(120, 130);
  mContainer->Run();
  mTimer --;
//  printf("mTimer = %i\n", mTimer);
  if (mTimer < TIMEOUT) {
    gGame->SetState(GAME_STATE_ATTRACT_MODE);
    return EFalse;
  }

  if (gControls.WasPressed(BUTTON_START)) {
    gGame->SetState(GAME_STATE_GAME);
    return EFalse;
  }

  if (gControls.WasPressed(BUTTON_MENU)) {
    gGame->SetState(GAME_STATE_MAIN_OPTIONS);
    return EFalse;
  }
  // if (mShowHighScores && --mTimer < 0) {
    // gGame->SetState(GAME_STATE_HIGH_SCORES);
    // return EFalse;
  // }
  return ETrue;
}
