/* @SymbolStart: 0x8013B2F0 */
/* rpg_bat_hit_step - RPG_BAT overlay, Address: 0x8013B2F8, Size: 572 bytes (143 insns) */
/* @SymbolSize: 580 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v1:v0 0x2c:rs:v1:v0 0x2c:rt:v1:v0 0x34:rs:v1:v0 0x40:rs:v1:v0 0x48:rs:v1:v0 0x54:rs:v1:v0 0x138:rt:v1:v0 0x13c:rs:v1:v0 0x13c:rt:v1:v0 0x144:rs:v1:v0 0x150:rs:v1:v0 0x158:rs:v1:v0 0x164:rs:v1:v0 0x10:rt:t6:v1 0x14:rs:t6:v1 0x14:rt:t6:v1 0x1c:rs:t6:v1 0x1c:rt:t7:t6 0x20:rs:t7:t6 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_seq_work;
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_rec_flag_check();
extern s32 rpg_bat_se_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_timer_sub();

s32 rpg_bat_hit_step(void)
{
    s32 v0;
    s32 v1;

    if ((rpg_bat_seq_work & 0x1) == 0) {
        switch (rpg_bat_unit0.sub) {
        case 0:
            rpg_bat_anim_set(48, 6, 5, 1);
            rpg_bat_state_next();
            break;
        case 1:
            rpg_bat_timer_sub(4);
            break;
        case 2:
            rpg_bat_anim_set2(49, 11, 5, 1);
            rpg_bat_se_set(1281, 1, 0);
            rpg_bat_state_next();
            break;
        case 3:
            v0 = rpg_bat_rec_flag_check(49);
            if (v0 != 0) {
                rpg_bat_anim_set2(49, 255, 1, 0);
            }
            v0 = rpg_bat_rec_flag_check(48);
            if (v0 != 0) {
                rpg_bat_anim_set2(49, 255, 1, 0);
                v1 = rpg_bat_seq_work;
                v1 -= 1;
                rpg_bat_seq_work = v1;
                if (v1 == 0) {
                    rpg_bat_step_next();
                } else {
                    rpg_bat_unit0.sub = 0;
                    return 0;
                }
            }
            break;
        }
    } else {
        switch (rpg_bat_unit0.sub) {
        case 0:
            rpg_bat_anim_set(48, 6, 5, 1);
            rpg_bat_state_next();
            break;
        case 1:
            rpg_bat_timer_sub(4);
            break;
        case 2:
            rpg_bat_anim_set2(49, 12, 5, 1);
            rpg_bat_se_set(1281, 1, 0);
            rpg_bat_state_next();
            break;
        case 3:
            v0 = rpg_bat_rec_flag_check(49);
            if (v0 != 0) {
                rpg_bat_anim_set2(49, 255, 1, 0);
            }
            v0 = rpg_bat_rec_flag_check(48);
            if (v0 != 0) {
                rpg_bat_anim_set2(49, 255, 1, 0);
                v1 = rpg_bat_seq_work;
                v1 -= 1;
                rpg_bat_seq_work = v1;
                if (v1 == 0) {
                    rpg_bat_step_next();
                } else {
                    rpg_bat_unit0.sub = 0;
                    return 0;
                }
            }
            break;
        }
    }
}
