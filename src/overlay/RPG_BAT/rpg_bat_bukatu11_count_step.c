/* @SymbolStart: 0x8014A3DC */
/* @RodataBase: 0x8015CD1C */
/* @SymbolSize: 632 */
/* @PostProc: scheduler_normalizer */
/* rpg_bat_bukatu11_count_step - RPG_BAT overlay, Address: 0x8014A3E4, Size: 624 bytes (156 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern volatile s32 rpg_bat_seq_work;
extern RPG_BAT_UNIT rpg_bat_unit0;

extern void rpg_bat_anim_set();
extern void rpg_bat_anim_set2();
extern void rpg_bat_sprite_pos_set();
extern void rpg_bat_se_set2();
extern void rpg_bat_step_next();
extern void rpg_bat_state_next();
extern void rpg_bat_timer_sub();
extern void rpg_bat_rec_sub();

void rpg_bat_bukatu11_count_step(void)
{
    s32 v1;

    if (rpg_bat_unit0.sub < 5) {
        rpg_bat_se_set2(1283, 30, 1);
    }
    switch ((u32)rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_anim_set(52, 3, 1, 1);
        rpg_bat_anim_set(47, 13, 5, 1);
        rpg_bat_state_next();
        break;
    case 1:
        rpg_bat_rec_sub(47);
        break;
    case 2:
        rpg_bat_anim_set(47, 7, 1, 1);
        rpg_bat_anim_set(42, 8, 1, 1);
        rpg_bat_anim_set(43, 9, 1, 1);
        rpg_bat_anim_set(44, 10, 1, 1);
        rpg_bat_anim_set(45, 11, 1, 1);
        rpg_bat_anim_set(46, 12, 1, 1);
        rpg_bat_state_next();
        break;
    case 3:
        rpg_bat_sprite_pos_set(41, 240, 56);
        rpg_bat_sprite_pos_set(40, 240, 56);
        rpg_bat_anim_set2(41, 15, 1, 1);
        rpg_bat_state_next();
        break;
    case 4:
        if (rpg_bat_unit0.wait >= 31) {
            rpg_bat_unit0.wait = 0;
            rpg_bat_unit0.aux10 = rpg_bat_unit0.aux10 + 1;
            if (rpg_bat_unit0.aux10 == rpg_bat_seq_work) {
                rpg_bat_state_next();
            } else {
                rpg_bat_anim_set2(41, rpg_bat_unit0.aux10 + 15, 1, 1);
                v1 = rpg_bat_unit0.aux10;
                if (v1 == 6 || v1 == 7) {
                    rpg_bat_anim_set2(40, 6, 1, 1);
                }
                v1 = rpg_bat_unit0.aux10;
                if (v1 == 8 || v1 == 9) {
                    rpg_bat_anim_set2(40, 25, 1, 1);
                }
            }
        }
        rpg_bat_unit0.wait = rpg_bat_unit0.wait + 1;
        break;
    case 5:
        rpg_bat_timer_sub(30);
        break;
    case 6:
        rpg_bat_step_next();
        break;
    }
}
