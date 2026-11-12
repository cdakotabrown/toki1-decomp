/* @SymbolStart: 0x80136CCC */
/* @SymbolSize: 128 */
/* rpgbat_blink_wait - RPG_BAT overlay, Address: 0x80136CD4, Size: 120 bytes (30 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit0;


extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_step_next();

void rpgbat_blink_wait(void)
{
    if ((rpg_bat_unit0.cnt & 1) == 0) {
        rpg_bat_anim_set2(43, 255, 1, 0);
    } else {
        rpg_bat_anim_set2(43, 255, 1, 1);
    }
    rpg_bat_unit0.cnt++;
    if (rpg_bat_unit0.cnt >= 61) {
        rpg_bat_step_next();
    }
}
