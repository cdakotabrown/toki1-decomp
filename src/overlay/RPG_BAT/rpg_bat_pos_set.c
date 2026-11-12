/* @CompileEnv: IDO52_COPYPROP_SITE=lod:*:deny */
/* @RodataBase: 0x8015AA4C */
/* @SymbolSize: 432 */
/* rpg_bat_pos_set - RPG_BAT overlay, Address: 0x80132B34, Size: 432 bytes (108 insns) */
#include "types.h"

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_girl_away_x;
extern s32 rpg_bat_girl_away_y;
extern s32 rpg_bat_girl_home_x;
extern s32 rpg_bat_girl_home_y;
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;

void rpg_bat_pos_set(void)
{
    s32 x;
    s32 y;

    switch ((u32)rpg_bat_target_no) {
    case 0:
        rpg_bat_tgt_x = 115;
        x = 132;
        rpg_bat_tgt_y = 87;
        y = 108;
        rpg_bat_girl_away_x = x + 48;
        rpg_bat_girl_away_y = y - 176;
        rpg_bat_girl_home_x = x;
        rpg_bat_girl_home_y = y;
        return;
    case 1:
        rpg_bat_tgt_x = 59;
        x = 76;
        rpg_bat_tgt_y = 71;
        y = 92;
        rpg_bat_girl_away_x = x + 48;
        rpg_bat_girl_away_y = y - 176;
        rpg_bat_girl_home_x = x;
        rpg_bat_girl_home_y = y;
        return;
    case 2:
        rpg_bat_tgt_x = 51;
        x = 68;
        rpg_bat_tgt_y = 119;
        y = 140;
        rpg_bat_girl_away_x = x + 48;
        rpg_bat_girl_away_y = y - 176;
        rpg_bat_girl_home_x = x;
        rpg_bat_girl_home_y = y;
        return;
    case 3:
        rpg_bat_tgt_x = 72;
        x = 64;
        rpg_bat_tgt_y = 39;
        y = 140;
        rpg_bat_girl_away_x = x + 48;
        rpg_bat_girl_away_y = y - 176;
        rpg_bat_girl_home_x = x;
        rpg_bat_girl_home_y = y;
        return;
    case 4:
        rpg_bat_tgt_x = 73;
        x = 74;
        rpg_bat_tgt_y = 58;
        y = 140;
        rpg_bat_girl_away_x = x + 48;
        rpg_bat_girl_away_y = y - 176;
        rpg_bat_girl_home_x = x;
        rpg_bat_girl_home_y = y;
        break;
    }
}
