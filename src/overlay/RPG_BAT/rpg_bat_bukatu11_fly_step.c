/* @SymbolStart: 0x80149C5C */
/* @SymbolSize: 1296 */
/* @PostProc: patch_insn_reg_field 0x4e8:rt:v0:a1 0x4f4:rt:v0:a1 0x4f8:rt:v0:a1 0x4fc:rs:v0:a1 */
/* pseudo-C with `@PostProc: pad_trailing_nop --count 192` on a 312-insn */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rsin();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_bukatu11_fly_step(void)
{
    s32 xo;
    s32 yo;
    s32 n;

    xo = (rpg_bat_target_x - 224) * rpg_bat_unit0.cnt / 1000 + 224;
    yo = (rpg_bat_target_y - 88) * rpg_bat_unit0.cnt / 1000 + 88;
    if (rpg_bat_target_no == 0) {
        if (rpg_bat_unit0.cnt * 6144 / 1000 > 0) {
            rpg_bat_se_set2(1284, 0, 2);
        }
        if (rpg_bat_unit0.cnt * 6144 / 1000 >= 1025) {
            rpg_bat_se_set2(1284, 0, 3);
        }
        if (rpg_bat_unit0.cnt * 6144 / 1000 >= 2049) {
            rpg_bat_se_set2(1284, 0, 4);
        }
        if (rpg_bat_unit0.cnt * 6144 / 1000 >= 3073) {
            rpg_bat_se_set2(1284, 0, 5);
        }
        if (rpg_bat_unit0.cnt * 6144 / 1000 >= 4097) {
            rpg_bat_se_set2(1284, 0, 6);
        }
        if (rpg_bat_unit0.cnt * 6144 / 1000 >= 5121) {
            rpg_bat_se_set2(1284, 0, 7);
        }
    } else {
        if (rpg_bat_unit0.cnt * 8192 / 1000 > 0) {
            rpg_bat_se_set2(1284, 0, 2);
        }
        if (rpg_bat_unit0.cnt * 8192 / 1000 >= 1025) {
            rpg_bat_se_set2(1284, 0, 3);
        }
        if (rpg_bat_unit0.cnt * 8192 / 1000 >= 2049) {
            rpg_bat_se_set2(1284, 0, 4);
        }
        if (rpg_bat_unit0.cnt * 8192 / 1000 >= 3073) {
            rpg_bat_se_set2(1284, 0, 5);
        }
        if (rpg_bat_unit0.cnt * 8192 / 1000 >= 4097) {
            rpg_bat_se_set2(1284, 0, 6);
        }
        if (rpg_bat_unit0.cnt * 8192 / 1000 >= 5121) {
            rpg_bat_se_set2(1284, 0, 7);
        }
        if (rpg_bat_unit0.cnt * 8192 / 1000 >= 6145) {
            rpg_bat_se_set2(1284, 0, 8);
        }
        if (rpg_bat_unit0.cnt * 8192 / 1000 >= 7169) {
            rpg_bat_se_set2(1284, 0, 9);
        }
    }
    if (rpg_bat_target_no == 0) {
        n = rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = n;
        rpg_bat_sprite_pos_set(41, xo, (rsin(n * 6144 / 1000) >> 8) + yo);
        rpg_bat_sprite_pos_set(40, xo,
                      (rsin(rpg_bat_unit0.cnt * 6144 / 1000) >> 8) + yo);
    } else {
        n = rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = n;
        rpg_bat_sprite_pos_set(41, xo, (rsin(n * 8192 / 1000) >> 8) + yo);
        rpg_bat_sprite_pos_set(40, xo,
                      (rsin(rpg_bat_unit0.cnt * 8192 / 1000) >> 8) + yo);
    }
    if (rpg_bat_target_no == 0) {
        n = rpg_bat_unit0.cnt + 12;
    } else {
        n = rpg_bat_unit0.cnt + 8;
    }
    rpg_bat_unit0.cnt = n;
    if (n >= 1001) {
        rpg_bat_step_next();
    }
}
