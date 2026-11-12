/* @SymbolSize: 188 */
/* rpg_bat_sprite_enter - RPG_BAT overlay, Address: 0x80137148, Size: 188 bytes (47 insns) */
#include "types.h"

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();

extern s32 rpg_bat_seq_work;

void rpg_bat_sprite_enter(s32 a0, s32 a1)
{
    s32 v0;

    if (a1 == 7) {
        if ((rpg_bat_seq_work & 1) == 0) {
            v0 = get_rand_mod(16);
            rpg_bat_sprite_pos_set(a0, v0 + 264, 64);
        } else {
            v0 = get_rand_mod(16);
            rpg_bat_sprite_pos_set(a0, v0 + 264, 88);
        }
        rpg_bat_seq_work = rpg_bat_seq_work + 1;
    } else {
        v0 = get_rand_mod(16);
        rpg_bat_sprite_pos_set(a0, v0 + 264, 64);
    }
    rpg_bat_anim_set2(a0, a1, 5, 1);
}
