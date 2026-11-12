/* rpg_bat_sprites_init - RPG_BAT overlay, Address: 0x8015A6D4, Size: 112 bytes (28 insns) */
#include "types.h"

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_step_next();

void rpg_bat_sprites_init(void)
{
    s32 i;
    s32 id;

    for (i = 0; i < 8; i++) {
        id = i + 40;
        rpg_bat_anim_set2(id, 7, 1, 1);
        rpg_bat_sprite_pos_set(id, -25, 265);
    }
    rpg_bat_step_next();
}
