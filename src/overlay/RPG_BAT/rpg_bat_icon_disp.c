/* rpg_bat_icon_disp -- RPG_BAT overlay, Address: 0x8014AC74, 348 bytes (87 insns) */
#include "types.h"

extern u8 class_w[];
extern s32 rpg_bat_target_no[];

extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);

void rpg_bat_icon_disp(void)
{
    s32 i;
    s32 n;
    u8 *rec;
    s32 *slot;
    s32 v;
    s32 arg5;

    for (i = 0; i != 4; i++) {
        if (rpg_bat_target_no[i + 50] == 1) {
            n = i * 3;
            arg5 = n + 5;
            slot = &rpg_bat_target_no[i * 2];
            rec = &class_w[n * 68];

            if ((rec[6802] & 1) == 0) {
                rpg_bat_anim_set2(n + 4, 7, 1, 1);
            }

            v = *(s16 *)&rec[6960];
            if (v >= 16) {
                v = 30 - v;
            }
            rpg_bat_sprite_pos_set(arg5, slot[55], slot[56] - (v << 3));

            if ((rec[6938] & 1) == 0) {
                rpg_bat_anim_set2(n + 4, 255, 1, 0);
                rpg_bat_anim_set2(arg5, 255, 1, 0);
                rpg_bat_anim_set2(n + 6, 255, 1, 0);
            }
        }
    }
}
