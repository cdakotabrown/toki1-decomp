/* @SymbolStart: 0x8014F88C */
/* @SymbolSize: 1328 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @RodataBase: 0x8015D028 */
/* rpg_bat_progress - RPG_BAT overlay, Address: 0x8014F88C, Size: 1328 bytes (332 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 get_rand_mod();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_effect_rec_set();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();

extern s32 rpg_bat_unit2_aux14[2];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;
extern RPG_BAT_UNIT rpg_bat_unit3;

void rpg_bat_progress(void)
{
    s32 i;

    rpg_bat_unit2_aux14[0] = rpg_bat_unit2_aux14[0] + 1;
    class_w[125].prio = 3;
    class_w[126].prio = 3;

    if (rpg_bat_unit2_aux14[1] >= 2 && rpg_bat_unit2_aux14[1] < 21 && class_w[125].tick == 1) {
        switch ((*(u16 *)&class_w[125].frame)) {
        case 0:
        case 2:
            if (get_rand_mod(3) == 0) {
                rpg_bat_se_set(1292, 1, 9);
            }
            if (get_rand_mod(2) == 0) {
                rpg_bat_se_set(1291, 1, 9);
            }
                    break;
        case 4:
        case 8:
        case 12:
            if (get_rand_mod(2) == 0) {
                rpg_bat_se_set(1290, 1, 9);
            }
                    break;
        }
    }

    switch (rpg_bat_unit2_aux14[1]) {
    case 0:
        rpg_bat_effect_rec_set(5);
        i = 4;
        do {
            rpg_bat_sprite_pos_set(i, -32, 0);
            i++;
        } while (i != 16);
        rpg_bat_anim_set(29, 6, 1, 1);
        rpg_bat_anim_set(30, 2, 1, 1);
        rpg_bat_sprite_pos_set(29, -32, 0);
        rpg_bat_se_set(1289, 1, 0);
        rpg_bat_act_next();
        break;
    case 1:
        if (rpg_bat_unit2_aux14[0] < 61) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 2:
        rpg_bat_anim_set(8, 7, 1, 1);
        rpg_bat_act_next();
        break;
    case 3:
        if (rpg_bat_unit2_aux14[0] < 1201) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 4:
        rpg_bat_anim_set(8, 8, 1, 1);
        rpg_bat_act_next();
        break;
    case 5:
        if (rpg_bat_unit2_aux14[0] < 2401) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 6:
        rpg_bat_anim_set(8, 9, 1, 1);
        rpg_bat_act_next();
        break;
    case 7:
        if (rpg_bat_unit2_aux14[0] < 3601) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 8:
        rpg_bat_anim_set(30, 3, 1, 1);
        rpg_bat_anim_set(8, 10, 1, 1);
        rpg_bat_act_next();
        break;
    case 9:
        if (rpg_bat_unit2_aux14[0] < 4801) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 10:
        rpg_bat_anim_set(8, 11, 1, 1);
        rpg_bat_act_next();
        break;
    case 11:
        if (rpg_bat_unit2_aux14[0] < 6001) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 12:
        rpg_bat_anim_set(30, 4, 1, 1);
        rpg_bat_anim_set(8, 12, 1, 1);
        rpg_bat_act_next();
        break;
    case 13:
        if (rpg_bat_unit2_aux14[0] < 7201) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 14:
        rpg_bat_anim_set(8, 13, 1, 1);
        rpg_bat_act_next();
        break;
    case 15:
        if (rpg_bat_unit2_aux14[0] < 8401) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 16:
        rpg_bat_anim_set(30, 5, 1, 1);
        rpg_bat_anim_set(8, 14, 1, 1);
        rpg_bat_act_next();
        break;
    case 17:
        if (rpg_bat_unit2_aux14[0] < 9601) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 18:
        rpg_bat_anim_set(8, 15, 1, 1);
        rpg_bat_act_next();
        break;
    case 19:
        if (rpg_bat_unit2_aux14[0] < 10801) {
            return;
        }
        rpg_bat_act_next();
        break;
    case 20:
        if ((rpg_bat_unit0.flag & 0xFF) != 0) {
            return;
        }
        if ((rpg_bat_unit1.flag & 0xFF) != 0) {
            return;
        }
        if ((rpg_bat_unit3.flag & 0xFF) != 0) {
            return;
        }
        rpg_bat_reset();
        rpg_bat_se_set(1293, 1, 0);
        rpg_bat_unit2.flag = 0x10001001;
        break;
    }
}
