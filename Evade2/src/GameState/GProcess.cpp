#include "GProcess.h"



// DEBUG_FINALBOSS causes wizard span to be final boss spawn instead, to debug the final boss
#define DEBUG_FINALBOSS
#undef DEBUG_FINALBOSS

void GProcess::WriteToStream(BMemoryStream &aStream) {
//  aStream.Write(&mAttribute, sizeof(mAttribute));
//  WriteCustomToStream(aStream); // write additional data, if any
//  mSprite->WriteToStream(aStream);
}

void GProcess::ReadFromStream(BMemoryStream &aStream) {
  // we assume mAttribute was already read in
//  ReadCustomFromStream(aStream);
//  mSprite->ReadFromStream(aStream);
}

GProcess *GProcess::Spawn(GGameState *aGameState, TInt16 mAttribute, TInt aIp, TFloat aX, TFloat aY, TUint16 aParams) {
  TInt ip = aIp;
  TFloat xx = aX,
         yy = aY;
  TUint16 params = aParams;

  switch (mAttribute) {
    case ATTR_GONE:
    default:
      return ENull;
//
//    case ATTR_DUNGEON_EXIT_FROM_LEVEL_1:
//      return aGameState->AddProcess(new GOverworldLevelTransitionProcess(aGameState, ip, DIRECTION_RIGHT, params, xx, yy, " "));
//
//    case ATTR_STONE_STAIRS_UP:
//      return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_UP, aParams, xx, yy, "STONE"));
//
//    case ATTR_OW_LEVEL_ENTRANCE:
//      return aGameState->AddProcess(new GOverworldLevelTransitionProcess(aGameState, ip, DIRECTION_DOWN, params, xx, yy, "OVERWORLD"));
//
//    case ATTR_STONE_STAIRS_DOWN:
//      if (!strcasecmp(aName, "DUNGEON")) {
//        return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_DOWN, params, xx, yy, "DUNGEON"));
//      }
//      else {
//        return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_DOWN, params, xx, yy, "STONE"));
//      }
//
//    case ATTR_WOOD_STAIRS_UP:
//      return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_UP, aParams, xx, yy, "WOOD"));
//
//    case ATTR_WOOD_STAIRS_DOWN:
//      return aGameState->AddProcess(new GStairsProcess(aGameState, ip, DIRECTION_DOWN, aParams, xx, yy, "WOOD"));
//
//    case ATTR_POT:
//      return aGameState->AddProcess(new GPotProcess(aGameState, ip, params, xx, yy));
//
//    case ATTR_POT_GONE:
//      break;
//
//    case ATTR_CHEST:
//      return aGameState->AddProcess(new GChestProcess(aGameState, ip, params, xx, yy, EFalse));
//    case ATTR_CHEST_OPEN:
//      return aGameState->AddProcess(new GChestProcess(aGameState, ip, params, xx, yy, ETrue));
//    case ATTR_SPIKES:
//      return aGameState->AddProcess(new GSpikesProcess(aGameState, ip, xx, yy, params));
//    case ATTR_WOOD_DOOR_H:
//      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, ETrue, ETrue));
//    case ATTR_WOOD_DOOR_V:
//      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, ETrue, EFalse));
//    case ATTR_METAL_GATE_H:
//      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, EFalse, ETrue));
//    case ATTR_METAL_GATE_V:
//      return aGameState->AddProcess(new GDoorProcess(aGameState, ip, params, xx, yy, EFalse, EFalse));
//    case ATTR_LEVER:
//      return aGameState->AddProcess(new GLeverProcess(aGameState, ip, params, xx, yy));
//    case ATTR_FLOOR_SWITCH:
//      return aGameState->AddProcess(new GFloorSwitchProcess(aGameState, ip, params, xx, yy, EFalse));
//    case ATTR_FLOOR_SWITCH_WOOD:
//      return aGameState->AddProcess(new GFloorSwitchProcess(aGameState, ip, params, xx, yy, ETrue));
//    case ATTR_PLAYER_IN1:
//      // ignore
//      break;
//    case ATTR_SPIDER:
//      return aGameState->AddProcess(new GSpiderProcess(aGameState, ip, xx, yy, params));
//    case ATTR_BAT:
//      return aGameState->AddProcess(new GBatProcess(aGameState, ip, xx, yy, params));
//    case ATTR_GOBLIN:
//      return aGameState->AddProcess(new GGoblinProcess(aGameState, ip, xx, yy, params));
//    case ATTR_GOBLIN_SNIPER:
//      return aGameState->AddProcess(new GGoblinSniperProcess(aGameState, ip, xx, yy, params));
//    case ATTR_TURRET:
//      return aGameState->AddProcess(new GTurretProcess(aGameState, ip, xx, yy, params));
//    case ATTR_ORC:
//      return aGameState->AddProcess(new GOrcProcess(aGameState, ip, xx, yy, params));
//    case ATTR_RAT:
//      return aGameState->AddProcess(new GRatProcess(aGameState, ip, xx, yy, params));
//    case ATTR_SLIME:
//      return aGameState->AddProcess(new GSlimeProcess(aGameState, ip, xx, yy, params));
//    case ATTR_TROLL:
//      return aGameState->AddProcess(new GTrollProcess(aGameState, ip, xx, yy, params));
//
//      // mid bosses
//    case ATTR_MID_BOSS_ENERGY:
//      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_MID_BOSS_ENERGY, params, MID_BOSS_ENERGY_BMP_SPRITES));
//    case ATTR_MID_BOSS_FIRE:
//      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_MID_BOSS_FIRE, params, MID_BOSS_FIRE_BMP_SPRITES));
//    case ATTR_MID_BOSS_EARTH:
//      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_MID_BOSS_EARTH, params, MID_BOSS_EARTH_BROWN_BMP_SPRITES));
//    case ATTR_MID_BOSS_WATER:
//      return aGameState->AddProcess(new GMidBossGenericProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_MID_BOSS_WATER, params, MID_BOSS_WATER_BMP_SPRITES));
//
//      // wizards
//    case ATTR_WIZARD_ENERGY:
//      return aGameState->AddProcess(new GWizardProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_WIZARD_ENERGY, params, ENERGY_WIZARD_BMP_SPRITES));
//    case ATTR_WIZARD_FIRE:
//      return aGameState->AddProcess(new GWizardProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_WIZARD_FIRE, params, FIRE_WIZARD_BMP_SPRITES));
//    case ATTR_WIZARD_WATER:
//      return aGameState->AddProcess(new GWizardProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_WIZARD_WATER, params, WATER_WIZARD_BMP_SPRITES));
//    case ATTR_WIZARD_EARTH:
//#ifndef DEBUG_FINALBOSS
//      return aGameState->AddProcess(new GWizardProcess(aGameState, xx, yy, BOSS_SLOT, ip, ATTR_WIZARD_EARTH, params, EARTH_WIZARD_BMP_SPRITES));
//#endif
//    case ATTR_FINAL_BOSS:
//      return aGameState->AddProcess(new GFinalBossProcess(aGameState, xx, yy, ip, params));
  }
  return ENull;
}
