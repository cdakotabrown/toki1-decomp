/* get_rpg_bat_mode_limit - RPG_BAT overlay, Address: 0x80144640, Size: 184 bytes. */
/* @SymbolSize: 184 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:5 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern volatile s32 rpg_bat_act_delay;
extern RPG_BAT_UNIT rpg_bat_unit1;

s32 get_rpg_bat_mode_limit(void)
{
    s32 v0 = rpg_bat_unit1.flag & 0xFFFF0000;

    switch (v0) {
    case 0x00010000:
    case 0x00020000:
    case 0x00040000:
        return 330;
    case 0x08000000:
        return 360;
    case 0x80000000:
        return rpg_bat_act_delay + 150;
    case 0x00100000:
        return rpg_bat_act_delay + 150;
    case 0x00200000:
        return rpg_bat_act_delay + 150;
    case 0x00400000:
        return rpg_bat_act_delay + 150;
    }

    return v0;
}
