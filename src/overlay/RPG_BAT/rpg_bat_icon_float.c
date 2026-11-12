/* @SymbolStart: 0x8014ADD0 */
/* @SymbolSize: 1212 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

typedef struct {
    u8 f0; u8 f1; u8 f2; u8 f3; u8 f4; u8 f5; u8 f6; u8 f7;
    s32 f8; s32 f12; s32 f16; u8 pad20[4]; s16 f24; u8 pad26[12];
    s16 f38; s16 pad40; s16 f42;
    u8 pad44[8]; s32 f52; s32 f56; u8 pad60[7]; u8 f67;
} Rec;
extern Rec class_w[];

extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;

rpg_bat_icon_float(x, y)
s32 x;
s32 y;
{
    if (rpg_bat_unit0.cnt == 0) {
        rpg_bat_se_set2(1281, 1, 0);
        rpg_bat_sprite_pos_set(42, x, y);
        rpg_bat_sprite_pos_set(43, x, y);
        rpg_bat_sprite_pos_set(47, x, y);
        rpg_bat_anim_set2(43, 14, 5, 1);
    }
    if (rpg_bat_unit0.cnt == 1) {
        rpg_bat_anim_set2(42, 9, 1, 1);
    }

    if (rpg_bat_seq_work[30] > 0 && rpg_bat_seq_work[30] < 15) {
        if ((rpg_bat_seq_work[33] == 0 || rpg_bat_seq_work[33] >= 9) &&
            (rpg_bat_seq_work[32] == 0 || rpg_bat_seq_work[32] >= 9) &&
            rpg_bat_seq_work[31] == 0 && get_rand_mod(5) == 0) {
            rpg_bat_seq_work[31] = 1;
            rpg_bat_sprite_pos_set(44, get_rand_mod(32) + x - 16, y - 8);
            rpg_bat_anim_set2(44, 10, 1, 1);
        }
        if ((rpg_bat_seq_work[31] == 0 || rpg_bat_seq_work[31] >= 9) &&
            (rpg_bat_seq_work[33] == 0 || rpg_bat_seq_work[33] >= 9) &&
            rpg_bat_seq_work[32] == 0 && get_rand_mod(5) == 0) {
            rpg_bat_seq_work[32] = 1;
            rpg_bat_sprite_pos_set(45, get_rand_mod(32) + x - 16, y - 8);
            rpg_bat_anim_set2(45, 11, 1, 1);
        }
        if ((rpg_bat_seq_work[32] == 0 || rpg_bat_seq_work[32] >= 9) &&
            (rpg_bat_seq_work[31] == 0 || rpg_bat_seq_work[31] >= 9) &&
            rpg_bat_seq_work[33] == 0 && get_rand_mod(5) == 0) {
            rpg_bat_seq_work[33] = 1;
            rpg_bat_sprite_pos_set(46, get_rand_mod(32) + x - 16, y - 8);
            rpg_bat_anim_set2(46, 12, 1, 1);
        }
    }

    if (rpg_bat_seq_work[31] != 0) {
        class_w[140].f42 = y - rpg_bat_seq_work[31] * 4 - 128;
        rpg_bat_seq_work[31]++;
        if (rpg_bat_seq_work[31] >= 33) {
            rpg_bat_seq_work[31] = 0;
        }
    }
    if (rpg_bat_seq_work[32] != 0) {
        class_w[141].f42 = y - rpg_bat_seq_work[32] * 4 - 128;
        rpg_bat_seq_work[32]++;
        if (rpg_bat_seq_work[32] >= 33) {
            rpg_bat_seq_work[32] = 0;
        }
    }
    if (rpg_bat_seq_work[33] != 0) {
        class_w[142].f42 = y - rpg_bat_seq_work[33] * 4 - 128;
        rpg_bat_seq_work[33]++;
        if (rpg_bat_seq_work[33] >= 33) {
            rpg_bat_seq_work[33] = 0;
        }
    }

    if ((class_w[139].f2 & 1) == 0) {
        rpg_bat_anim_set2(43, 15, 5, 1);
        rpg_bat_seq_work[30]++;
        if (rpg_bat_seq_work[30] == 5) {
            rpg_bat_se_set2(1282, 1, 0);
            rpg_bat_anim_set2(47, 16, 5, 1);
        }
        if (rpg_bat_seq_work[30] == 16) {
            rpg_bat_anim_set2(43, 17, 5, 1);
            rpg_bat_anim_set2(44, 255, 1, 0);
            rpg_bat_anim_set2(45, 255, 1, 0);
            rpg_bat_anim_set2(46, 255, 1, 0);
        }
        if (rpg_bat_seq_work[30] == 17) {
            rpg_bat_anim_set2(43, 255, 1, 0);
            rpg_bat_step_next();
        }
    }
    if (rpg_bat_seq_work[30] == 16 && class_w[139].f24 == 16) {
        rpg_bat_anim_set2(42, 255, 1, 0);
    }
    if ((class_w[143].f2 & 1) == 0) {
        rpg_bat_anim_set2(47, 255, 1, 0);
    }
    rpg_bat_unit0.cnt++;
}
