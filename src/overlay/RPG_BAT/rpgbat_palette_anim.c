/* @SymbolStart: 0x80140E70 */
/* @SymbolSize: 492 */
/* @CompileFlags: -cckr */
/* Address: 0x80140E70 (cross-prologue head absorbed), Size: 492 bytes (123 insns) */
#include "types.h"

extern int rpg_bat_unit1_sub;
extern int rpg_bat_unit1_wait;
extern s32 rpg_bat_res_pal[];

void rpgbat_palette_anim()
{
    switch (rpg_bat_unit1_sub) {
    case 0:
        rpg_bat_sprite_pos_set(59, 72, 96);
        rpg_bat_anim_set2(59, 2, 1, 1);
        palette_upload_enqueue((*(int *)&rpg_bat_res_pal[6]), 18, 1, 4, 0);
        rpg_bat_substep_next();
        break;
    case 1:
        rpg_bat_unit1_wait++;
        if (rpg_bat_unit1_wait == 30) {
            rpg_bat_se_set(1284, 1, 0);
        }
        if ((rpg_bat_unit1_wait & 3) == 0) {
            palette_upload_enqueue((*(int *)&rpg_bat_res_pal[6]), 18, 1, 4, rpg_bat_unit1_wait / 4 % 5);
        }
        if (rpg_bat_unit1_wait >= 79) {
            rpg_bat_substep_next();
        }
        break;
    case 2:
        rpg_bat_unit1_wait++;
        if ((rpg_bat_unit1_wait & 3) == 0) {
            palette_upload_enqueue((*(int *)&rpg_bat_res_pal[6]), 18, 1, 4, rpg_bat_unit1_wait / 4 % 6 + 5);
        }
        if (rpg_bat_unit1_wait >= 23) {
            rpg_bat_substep_next();
        }
        break;
    case 3:
        palette_upload_enqueue((*(int *)&rpg_bat_res_pal[1]), 18, 1, 4, 4);
        rpg_bat_seq_step_next();
        break;
    }
}
