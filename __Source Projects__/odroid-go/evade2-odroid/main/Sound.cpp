#define DEBUGME
#include "Evade2.h"

#ifdef ENABLE_AUDIO
// #include "../game_music_emu/gme.h"
// #include "../game_music_emu/Blip_Buffer.h"
#include "../go-lib/Audio.h"
#include "../go-lib/SDCard.h"

#include "../libxmp/xmp.h"

#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "freertos/task.h"

#include "Music.h"


#define SAMPLE_RATE (22050)

bool playing = false;

#define TIMER_LENGTH 75
#define SOUND_BUFF_SIZE 12 
short *buff;

volatile BYTE current_song = -1;
bool musicFileLoaded = false;

esp_timer_create_args_t timer_args;
esp_timer_handle_t timer;

xmp_context ctx;

bool WARNED_OF_PLAY_BUFFER = false;

static void timerCallback(void *arg) {
  if (musicFileLoaded && current_song > -1) {
    int result = xmp_play_buffer(ctx, buff, SOUND_BUFF_SIZE, 0);
    if (result != 0) {
      if (!WARNED_OF_PLAY_BUFFER) {
        // Something really bad happened, and audio stopped :(
        printf("play_buffer not zero (result = %i)!\n", result);fflush(stdout);
        WARNED_OF_PLAY_BUFFER = true;
      }
      memset(buff, 0, SOUND_BUFF_SIZE);      
    }
  }
  else {
    memset(buff, 0, SOUND_BUFF_SIZE);
  }
  
  Audio::Submit(buff, SOUND_BUFF_SIZE >> 2);
}



void loadSamples() {
  xmp_start_smix(ctx, 4, 9);
  xmp_smix_load_sample_from_memory(ctx, 0, (void *)_SFX_player_shoot_wav_start, _SFX_player_shoot_wav_start - _SFX_player_shoot_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 1, (void *)_SFX_enemy_shoot_wav_start, _SFX_enemy_shoot_wav_start - _SFX_enemy_shoot_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 2, (void *)_SFX_player_hit_wav_start, _SFX_player_hit_wav_start - _SFX_player_hit_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 3, (void *)_SFX_next_attract_screen_wav_start, _SFX_next_attract_screen_wav_start - _SFX_next_attract_screen_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 4, (void *)_SFX_next_attract_char_wav_start, _SFX_next_attract_char_wav_start - _SFX_next_attract_char_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 5, (void *)_SFX_enemy_explode_wav_start, _SFX_enemy_explode_wav_start - _SFX_enemy_explode_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 6, (void *)_SFX_speed_boost_wav_start, _SFX_speed_boost_wav_start - _SFX_speed_boost_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 7, (void *)_SFX_enemy_flyby_wav_start, _SFX_enemy_flyby_wav_start - _SFX_enemy_flyby_wav_end);
  xmp_smix_load_sample_from_memory(ctx, 8, (void *)_SFX_boss_explode_wav_start, _SFX_boss_explode_wav_start - _SFX_boss_explode_wav_end);
}


void Sound::init() {

  Audio::Init(SAMPLE_RATE);
  buff = (short *)heap_caps_malloc(sizeof(short) * SOUND_BUFF_SIZE, MALLOC_CAP_8BIT);
  memset(buff, 0, SOUND_BUFF_SIZE);
  Audio::Mute();

  ctx = xmp_create_context();

  //*** CREATE TIMER ***//
  timer_args.callback = &timerCallback;
  timer_args.name = "audioTimer";

  esp_timer_create(&timer_args, &timer);
  esp_timer_start_periodic(timer, TIMER_LENGTH);
}



int sfxChannel = 0;
BOOL speedBoostSfxPlaying = false;

void Sound::play_sound(BYTE id) {
  if (musicFileLoaded == false || current_song == -1) {
    return;
  }

  if (id == SFX_SPEED_BOOST && speedBoostSfxPlaying == false) {
    xmp_smix_play_sample(ctx, id, 40, 128, 4);
    speedBoostSfxPlaying = true;
  }
  else {
    xmp_smix_play_sample(ctx, id, 60, 96, sfxChannel);
    sfxChannel++;
    if (sfxChannel >= 3) {
      sfxChannel = 0;
    }    
  }
}

// Shut down audio
void Sound::stfu() { 
  // esp_timer_stop(timer);
  current_song = -1; 
}

int loadSong(int temp) {

  xmp_end_player(ctx);
  xmp_end_smix(ctx);
  xmp_stop_module(ctx);
  xmp_free_context(ctx);


  ctx = xmp_create_context();
  loadSamples();
  printf("Loading Song: %i\n", temp); fflush(stdout);
  int loadResult = 0;
    //Todo: converted to an array.
  switch(temp) {

    case 0:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_00_Intro_xm_start, _00_Intro_xm_start - _00_Intro_xm_end);
    break;
    case 1:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_01_Stage_1_xm_start, _01_Stage_1_xm_start - _01_Stage_1_xm_end);
    break;
    case 2:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_02_Boss_1_xm_start, _02_Boss_1_xm_start - _02_Boss_1_xm_end);
    break;
    case 3:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_03_Stage_2_xm_start, _03_Stage_2_xm_start - _03_Stage_2_xm_end);
    break;
    case 4:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_04_Boss_2_xm_start, _04_Boss_2_xm_start - _04_Boss_2_xm_end);
    break;
    case 5:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_05_Stage_3_xm_start, _05_Stage_3_xm_start - _05_Stage_3_xm_end);
    break;
    case 6:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_06_Boss_3_xm_start, _06_Boss_3_xm_start - _06_Boss_3_xm_end);
    break;
    case 7:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_07_Stage_4_xm_start, _07_Stage_4_xm_start - _07_Stage_4_xm_end);
    break;
    case 8:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_08_Boss_4_xm_start, _08_Boss_4_xm_start - _08_Boss_4_xm_end);
    break;
    case 9:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_09_Stage_5_xm_start, _09_Stage_5_xm_start - _09_Stage_5_xm_end);
    break;
    case 10:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_10_Game_Over_xm_start, _10_Game_Over_xm_start - _10_Game_Over_xm_end);
    break;
    case 11:
      loadResult = xmp_load_module_from_memory(ctx, (void *)_11_Get_Ready_xm_start, _11_Get_Ready_xm_start - _11_Get_Ready_xm_end);
    break;

    default:
    break;
  }  
  printf("loadResult = %i\n", loadResult); fflush(stdout);
  return loadResult;
}

void Sound::play_score(BYTE id) {
  if (id == -1) {
    current_song = -1;
    return;
  }

  if (current_song != id) {
    int temp = id;
    current_song = -1;
    musicFileLoaded = false;
    Audio::Mute();

    xmp_set_player(ctx, XMP_PLAYER_VOLUME, 0);
    // esp_timer_stop(timer);
    
    int loadResult = loadSong(temp);  

    if (loadResult < 0) {
      // printf("Could not open song %i! %i\n", temp, loadResult);
      // Sometimes XMP fails for no obvious reason. Try one more time for good measure.
      loadResult = loadSong(temp);  
    }  
    
    if (loadResult == 0) {
      musicFileLoaded = true;
    }

    if (!musicFileLoaded) {
      return;
    }

    xmp_start_player(ctx, SAMPLE_RATE, 0);
    xmp_set_player(ctx, XMP_PLAYER_VOLUME, 256);
    xmp_set_player(ctx, XMP_PLAYER_MIX, 0);


    // esp_timer_start_periodic(timer, TIMER_LENGTH);
    Audio::Unmute();
    current_song = temp;
  }
}
#else
void Sound::init() {}
void Sound::play_sound(BYTE id) {}
void Sound::play_score(BYTE id) {}
void Sound::stfu() {}
#endif
