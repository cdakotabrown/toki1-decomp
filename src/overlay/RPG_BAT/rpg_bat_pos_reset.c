/* @SymbolSize: 72 */
/* /* rpg_bat_pos_reset - RPG_BAT overlay, Address: 0x8014EEF4, Size: 72 bytes (18 insns) */
#include "types.h"

extern void rpg_bat_sprite_pos_set(s32, s32, s32);

void rpg_bat_pos_reset(void)
{
    s32 i;
    for (i = 40; i < 48; i++) {
        rpg_bat_sprite_pos_set(i, 0, 0);
    }
}
