/* get_rpg_bat_wait - RPG_BAT overlay, Address: 0x801446F8, real size 192 bytes. */
/* @RodataBase: 0x8015C4DC */
/* @SymbolSize: 192 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_girl;
extern RPG_BAT_UNIT rpg_bat_unit2;

s32 get_rpg_bat_wait()
{
    switch (rpg_bat_girl) {
    case 1: return 10;
    case 0: return 200;
    case 7: return 240;
    case 9: return 260;
    case 6: return 260;
    case 8: return 260;
    case 10: return 260;
    case 4: return 300;
    case 2: return 300;
    case 3: return 300;
    case 5: return 300;
    case 13:
        if (rpg_bat_unit2.hp < 10) {
            return 110;
        }
        return 170;
    case 14:
        return 0x7FFFFFFF;
    }
}
