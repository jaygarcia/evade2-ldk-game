#include "Game.h"
#include "GSoundPlayer.h"
#include "GResources.h"
#include "Memory.h"

#define DISABLE_AUDIO
#undef DISABLE_AUDIO


GSoundPlayer gSoundPlayer;

static const TUint16 effectsList[] = {
//  SFX_EMPTY_WAV,
  SFX_BOSS_EXPLODE_WAV,
  SFX_ENEMY_EXPLODE_WAV,
  SFX_ENEMY_FLYBY_WAV,
  SFX_ENEMY_SHOOT_WAV,
  SFX_PLAYER_SHOOT_WAV,
  SFX_PLAYER_HIT_WAV,
  SFX_NEXT_ATTRACT_CHAR_WAV,
  SFX_NEXT_ATTRACT_SCREEN_WAV,
  SFX_BOSS_EXPLODE_WAV,
  SFX_SPEED_BOOST_WAV
};

static TUint16 FindSfxNumber(TUint16 aSfxFile) {

  for (size_t i = 0; i < sizeof(effectsList); i++) {
    if (aSfxFile == effectsList[i]) {
      return (TUint16) i; // Should not go above UINT16_MAX
    }
  }
  printf("WARNING :: Could not find SFX for ID %i\n", aSfxFile);


  return UINT16_MAX;
}

static const TUint16 allSongs[] = {
  EMPTYSONG_XM,
  S00_INTRO_XM,
  S01_STAGE_1_XM,
  S02_BOSS_1_XM,
  S03_STAGE_2_XM,
  S04_BOSS_2_XM,
  S05_STAGE_3_XM,
  S06_BOSS_3_XM,
  S07_STAGE_4_XM,
  S08_BOSS_4_XM,
  S09_STAGE_5_XM,
  S10_GAME_OVER_XM,
  S11_GET_READY_XM,
  S12_LOGO_REVEAL_XM,
};

GSoundPlayer::~GSoundPlayer() {
  // Release songs memory
  FreeMem(mSongSlots);
}

void GSoundPlayer::Init(TUint8 aNumberFxChannels) {
  mMaxSongs = sizeof(allSongs) / sizeof(TUint16);
  mMaxEffects = sizeof(effectsList) / sizeof(TUint16);

  soundEngine.InitAudioEngine(aNumberFxChannels, mMaxEffects);

//  SDL_ClearError();
  LoadEffects();
//  SDL_ClearError();



  mSongSlots = (SongSlot *)AllocMem(sizeof(SongSlot) * mMaxSongs, MEMF_SLOW);

//  for (TUint8 i = 0; i < mMaxSongs; i++) {
//    auto *slot = (SongSlot *)AllocMem(sizeof(SongSlot), MEMF_SLOW);
//
//    slot->mResourceNumber = allSongs[i];
//    slot->mSlotNumber = SONG0_SLOT + i;
//
//    gResourceManager.LoadRaw(allSongs[i], slot->mSlotNumber);
//    slot->mRaw = gResourceManager.GetRaw(slot->mSlotNumber);
//
//    mSongSlots[i] = *slot;
//    FreeMem(slot);
//  }
  PlayMusic(EMPTYSONG_XM);


  soundEngine.SetMusicVolume(gOptions->music);
  soundEngine.SetEffectsVolume(gOptions->sfx);
  soundEngine.MuteMusic(gOptions->muted);
}

TBool GSoundPlayer::PlayMusic(TInt16 aResourceId) {
//    aResourceId = EMPTYSONG_XM;
//  printf("%s %i\n", __PRETTY_FUNCTION__, aResourceId);

  BRaw *songToLoad = FindRawSongFileById(aResourceId);

  TBool music = soundEngine.PlayMusic(songToLoad, aResourceId);
//   BSoundEngine::PlayMusic un-mutes the music
// We have to re-mute it in case of mute == true

  soundEngine.SetMusicVolume(gOptions->music);
  soundEngine.SetEffectsVolume(gOptions->sfx);
  soundEngine.MuteMusic(gOptions->muted);

  return music;
}


BRaw *GSoundPlayer::FindRawSongFileById(TInt16 aResourceId) {

  for (TUint8 i = 0; i < mMaxSongs; i++) {
//    printf("allSongs[%i] = %i\n", i, aResourceId);
//    fflush(stdout);
    if (allSongs[i] == aResourceId) {
      if (gResourceManager.GetRaw(SONG0_SLOT)) {
        gResourceManager.ReleaseRawSlot(SONG0_SLOT);
      }
      gResourceManager.LoadRaw(allSongs[i], SONG0_SLOT);
      return gResourceManager.GetRaw(SONG0_SLOT);;
    }
  }

  printf("WARNING :: Could not find song %i\n", aResourceId);

  return ENull;
}


TBool GSoundPlayer::LoadEffects() {
  for (TUint8 index = 0; index < mMaxEffects; index++) {
    soundEngine.LoadEffect(index, effectsList[index], SFX1_SLOT + index);
//    return ETrue;
  }
  return ETrue;
}


void  GSoundPlayer::MuteMusic(TBool aMuted) {
  return soundEngine.MuteMusic(aMuted);
}



void GSoundPlayer::TriggerSfx(TUint16 aSfxNumber, TInt8 aChannel) {
//  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
  soundEngine.PlaySfx(FindSfxNumber(aSfxNumber), aChannel);
}

void GSoundPlayer::SfxOptionSelect() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_OPTION_SELECT_WAV), 1);
}

void GSoundPlayer::SfxMenuNavDown() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_NAV_DOWN_WAV), 1);
}

void GSoundPlayer::SfxMenuNavUp() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_NAV_UP_WAV), 1);
}

void GSoundPlayer::SfxMenuIn() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_IN_WAV), 1);
}

void GSoundPlayer::SfxMenuOut() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_MENU_OUT_WAV), 1);
}


void GSoundPlayer::SfxSaveGame() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_SAVE_GAME_WAV), 1);
}

void GSoundPlayer::SfxStartGame() {
  SfxSaveGame();
}

// SFX Player //
void GSoundPlayer::SfxPlayerSlash(){
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_PLAYER_SLASH_WAV), 1);
}


void GSoundPlayer::SfxItemHeart() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_ITEM_HEART_WAV), 2);
}
void GSoundPlayer::SfxItemPickupGeneric() {
  printf("GSoundPlayer::%s()\n", __FUNCTION__ );
//  soundEngine.PlaySfx(FindSfxNumber(SFX_ITEM_PICKUP_GENERIC_WAV), 3);
}