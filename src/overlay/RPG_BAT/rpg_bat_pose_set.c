/* @SymbolSize: 420 */
/* @RodataBase: 0x8015D5EC */
/* @PostProc: scheduler_normalizer */
#include "types.h"

extern s32 rpg_bat_girl;
extern s32 rpg_bat_girl_away_x;
extern s32 rpg_bat_girl_away_y;
extern s32 rpg_bat_girl_home_x;
extern s32 rpg_bat_girl_home_y;
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;

void rpg_bat_pose_set(void)
{
    switch ((u32)rpg_bat_girl) {
    case 0:
        rpg_bat_tgt_x = 4;
        rpg_bat_tgt_y = 0;
        break;
    case 1:
        rpg_bat_tgt_x = 5;
        rpg_bat_tgt_y = -2;
        break;
    case 2:
        rpg_bat_tgt_x = 6;
        rpg_bat_tgt_y = 2;
        break;
    case 3:
        rpg_bat_tgt_x = 5;
        rpg_bat_tgt_y = 2;
        break;
    case 4:
        rpg_bat_tgt_x = 5;
        rpg_bat_tgt_y = 2;
        break;
    case 5:
        rpg_bat_tgt_x = 5;
        rpg_bat_tgt_y = 2;
        break;
    case 6:
        rpg_bat_tgt_x = 3;
        rpg_bat_tgt_y = 2;
        break;
    case 7:
        rpg_bat_tgt_x = 4;
        rpg_bat_tgt_y = 1;
        break;
    case 8:
        rpg_bat_tgt_x = 5;
        rpg_bat_tgt_y = 1;
        break;
    case 9:
        rpg_bat_tgt_x = 2;
        rpg_bat_tgt_y = 0;
        break;
    case 10:
        rpg_bat_tgt_x = 8;
        rpg_bat_tgt_y = 2;
        break;
    }

    rpg_bat_girl_away_x = 280;
    rpg_bat_girl_away_y = 136;
    rpg_bat_girl_home_x = 280 - rpg_bat_tgt_x;
    rpg_bat_girl_home_y = 96 - rpg_bat_tgt_y;
}
