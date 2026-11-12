/* kagami_target_set - RPG_BAT overlay, Address: 0x8014E654, Size: 296 bytes (74 insns) */
/* @SymbolStart: 0x8014E64C */
/* @SymbolSize: 304 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_toriko_flag[];
extern s32 rpg_bat_target_no;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

s32 get_rand_mod(s32 arg0);

void kagami_target_set(void)
{
    s32 v0;

    if ((rpg_bat_unit1.flag << 11) < 0) {
        v0 = get_rand_mod(3);
        switch (v0) {
        case 0:
            if (rpg_bat_toriko_flag[0] == 0) {
                rpg_bat_target_no = 0;
            } else if (rpg_bat_toriko_flag[1] == 0) {
                rpg_bat_target_no = 1;
            } else {
                rpg_bat_target_no = 2;
            }
            break;
        case 1:
            if (rpg_bat_toriko_flag[1] == 0) {
                rpg_bat_target_no = 1;
            } else if (rpg_bat_toriko_flag[2] == 0) {
                rpg_bat_target_no = 2;
            } else {
                rpg_bat_target_no = 0;
            }
            break;
        case 2:
            if (rpg_bat_toriko_flag[2] == 0) {
                rpg_bat_target_no = 2;
            } else if (rpg_bat_toriko_flag[0] == 0) {
                rpg_bat_target_no = 0;
            } else {
                rpg_bat_target_no = 1;
            }
            break;
        }
    } else {
        rpg_bat_target_no = 3;
        rpg_bat_unit2.flag = 2;
    }
}
