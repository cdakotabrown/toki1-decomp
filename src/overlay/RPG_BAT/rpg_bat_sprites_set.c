/* @SymbolSize: 160 */
/* /* rpg_bat_sprites_set - RPG_BAT overlay, Address: 0x80132A94, Size: 160 bytes (40 insns) */
#include "types.h"

extern u8  class_w[];
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();

void rpg_bat_sprites_set(s32 arg0, s32 arg1)
{
    s32 i;
    s32 id;
    u8 *ptr;

    ptr = class_w;
    for (i = 0; i < 8; i++) {
        id = i + 40;
        rpg_bat_sprite_pos_set(id, arg0, arg1);
        rpg_bat_anim_set2(id, 9, 1, 1);
        ptr += 68;
        ptr[9181] = 4;
    }
}
