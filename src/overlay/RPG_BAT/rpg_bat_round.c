/* @SymbolStart: 0x80151108 */
/* @RodataBase: 0x8015D618 */
/* @SymbolSize: 632 */
/* rpg_bat_round - RPG_BAT overlay, Address: 0x80151110, Size: 624 bytes (156 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 rpg_bat_manzai_msg_pick();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_timer_sub();
extern s32 rpg_bat_state_gate();

extern u8 rpg_bat_msg_tbl[];
extern s32 pad_trig;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_round(void)
{
    switch ((u32)rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_manzai_msg_pick();
        rpg_bat_msg_set(rpg_bat_msg_tbl, 3, 90);
        rpg_bat_anim_set(52, 3, 1, 1);
        rpg_bat_state_next();
        break;
    case 1:
        if ((pad_trig & 0x40) != 0) {
            rpg_bat_state_next();
        } else {
            rpg_bat_timer_sub(60);
        }
        break;
    case 2:
        rpg_bat_anim_set(52, 4, 5, 1);
        rpg_bat_state_next();
        break;
    case 3:
        if (class_w[148].frame == 1) {
            rpg_bat_se_set2(1297, 0, 3);
        }
        rpg_bat_state_gate(1);
        break;
    case 4:
        rpg_bat_anim_set(52, 3, 1, 1);
        rpg_bat_state_next();
        break;
    case 5:
        if ((pad_trig & 0x40) != 0) {
            rpg_bat_state_next();
        } else {
            rpg_bat_timer_sub(50);
        }
        break;
    case 6:
        rpg_bat_anim_set(52, 4, 5, 1);
        rpg_bat_state_next();
        break;
    case 7:
        if (class_w[148].frame == 1) {
            rpg_bat_se_set2(1297, 0, 1);
        }
        rpg_bat_state_gate(2);
        break;
    case 8:
        rpg_bat_anim_set(52, 3, 1, 1);
        rpg_bat_state_next();
        break;
    case 9:
        if ((pad_trig & 0x40) != 0) {
            rpg_bat_state_next();
        } else {
            rpg_bat_timer_sub(50);
        }
        break;
    case 10:
        rpg_bat_anim_set(52, 4, 5, 1);
        rpg_bat_state_next();
        break;
    case 11:
        if (class_w[148].frame == 1) {
            rpg_bat_se_set2(1297, 0, 2);
        }
        rpg_bat_state_gate(3);
        break;
    case 12:
        rpg_bat_step_next();
        break;
    }
}
