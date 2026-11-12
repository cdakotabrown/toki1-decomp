/* @SymbolStart: 0x80151FC0 */
/* @SymbolSize: 192 */
/* rpg_bat_mode_show - RPG_BAT overlay, Address: 0x80151FC8, Size: 184 bytes (46 insns) */
#include "types.h"

extern s32 rpg_bat_mode;

extern s32 rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);

void rpg_bat_mode_show(void)
{
    if (rpg_bat_mode == 0x40000) {
        rpg_bat_anim_set(56, 3, 5, 1);
    } else {
        rpg_bat_anim_set(56, 4, 5, 1);
    }

    switch (rpg_bat_mode) {
    case 0x10000:
        rpg_bat_se_set(1285, 1, 0);
        break;
    case 0x20000:
        rpg_bat_se_set(1286, 1, 0);
        break;
    case 0x40000:
        rpg_bat_se_set(1285, 1, 0);
        break;
    default:
        break;
    }
}
