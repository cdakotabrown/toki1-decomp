/* @SymbolStart: 0x80148B08 */
/* @RodataBase: 0x8015CB8C */
/* @SymbolSize: 1120 */
/* @PostProc: swap_instructions --offset1 0x2c4 --offset2 0x2cc */
/* @PostProc: swap_instructions --offset1 0x2c8 --offset2 0x2d0 */
/* @PostProc: swap_instructions --offset1 0x2cc --offset2 0x2d4 */
/* @PostProc: swap_instructions --offset1 0x2d0 --offset2 0x2d4 */
/* @PostProc: swap_instructions --offset1 0x310 --offset2 0x314 */
/* @PostProc: patch_insn_reg_field 0x320:rt:t0:t2 0x324:rs:t0:t2 0x324:rt:t0:t2 0x328:rt:t3:t0 0x32c:rs:t3:t0 0x32c:rt:t3:t0 0x380:rs:t0:t2 0x384:rd:t2:t1 0x390:rs:t2:t1 0x3ac:rs:t2:t1 0x3b8:rd:t1:t3 0x3bc:rs:t1:t0 */
/* rpg_bat_bukatu7_arc_step - RPG_BAT overlay, Address: 0x80148B10, Size: 1112 bytes. */
/* `@RodataBase: 0x8015CB8C` (the address the target's %hi/%lo pair encodes; */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 rpg_bat_target_no;
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;
extern s32 rpg_bat_move_dx;
extern s32 rpg_bat_move_dy;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern s32 rpg_bat_timer_sub();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_state_next();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_row_disp();
extern s32 rsin();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_step_next();

void rpg_bat_bukatu7_arc_step(void)
{
    s32 t;
    s32 v1;
    s32 ret;
    s32 aa1, aa2;

    switch ((u32)rpg_bat_unit0.sub) {
    case 0:
        switch ((u32)rpg_bat_target_no) {
        case 0:
            rpg_bat_tgt_x = 152;
            rpg_bat_tgt_y = 104;
            break;
        case 1:
            rpg_bat_tgt_x = 96;
            rpg_bat_tgt_y = 88;
            break;
        case 2:
            rpg_bat_tgt_x = 88;
            rpg_bat_tgt_y = 136;
            break;
        case 3:
        case 4:
            rpg_bat_tgt_x = 128;
            rpg_bat_tgt_y = 128;
            break;
        }
        rpg_bat_sprite_pos_set(50, rpg_bat_tgt_x, rpg_bat_tgt_y);
        rpg_bat_anim_set2(50, 9, 1, 1);
        rpg_bat_state_next();
        break;
    case 1:
        rpg_bat_timer_sub(10);
        break;
    case 2:
        t = rpg_bat_unit1.flag;
        if ((t << 11) < 0) {
            rpg_bat_anim_set2(50, 11, 5, 1);
        } else {
            rpg_bat_anim_set2(50, 10, 5, 1);
        }
        rpg_bat_se_set2(1287, 1, 0);
        rpg_bat_state_next();
        break;
    case 3:
        t = rpg_bat_unit1.flag;
        if ((t << 11) < 0) {
            if (class_w[146].frame == 9) {
                rpg_bat_se_set2(1289, 0, 5);
                rpg_bat_sprite_pos_set(40, rpg_bat_tgt_x - 35, rpg_bat_tgt_y - 20);
                rpg_bat_anim_set2(40, 8, 5, 1);
            }
        } else {
            if (class_w[146].frame == 14) {
                rpg_bat_se_set2(1289, 0, 5);
                t = rpg_bat_unit1.flag;
                if ((t << 4) < 0) {
                    rpg_bat_sprite_pos_set(40, 72, 56);
                } else {
                    rpg_bat_sprite_pos_set(40, 77, 36);
                }
                rpg_bat_anim_set2(40, 8, 5, 1);
            }
        }
        if ((class_w[136].anim_flag & 1) == 0) {
            rpg_bat_anim_set2(40, 8, 1, 0);
        }
        if ((class_w[146].anim_flag & 1) == 0) {
            rpg_bat_row_disp();
            rpg_bat_state_next();
        }
        break;
    case 4:
        rpg_bat_move_dx = 261 - rpg_bat_tgt_x;
        rpg_bat_move_dy = 86 - rpg_bat_tgt_y;
        v1 = 1000;
        ret = rsin((rpg_bat_unit0.wait << 11) / 1000);
        aa1 = (s32)((u32)rpg_bat_move_dx * (u32)rpg_bat_unit0.wait) / v1 + rpg_bat_tgt_x;
        aa2 = (rpg_bat_tgt_y + (s32)((u32)rpg_bat_move_dy * (u32)rpg_bat_unit0.wait) / v1)
              - (ret >> 7);
        rpg_bat_sprite_pos_set(50, aa1, aa2);
        rpg_bat_unit0.wait = rpg_bat_unit0.wait + 20;
        rpg_bat_anim_set2(50, 12, 1, 1);
        if (rpg_bat_unit0.wait >= 1001) {
            rpg_bat_se_set2(1282, 1, 0);
            rpg_bat_state_next();
        }
        break;
    case 5:
        rpg_bat_anim_set2(50, 255, 1, 0);
        rpg_bat_sprite_pos_set(52, 0, 0);
        rpg_bat_anim_set(52, 2, 1, 1);
        rpg_bat_step_next();
        break;
    }
}
