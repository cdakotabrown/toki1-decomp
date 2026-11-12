/* rpg_bat_flag_check - RPG_BAT overlay, Address: 0x801447B8, Size: 40 bytes */
/* @SymbolSize: 40 */
#include "types.h"

extern s32 rpg_bat_variant_flag;

s32 rpg_bat_flag_check(void)
{
    if (rpg_bat_variant_flag == 1) {
        return 1;
    }
    return 0;
}
