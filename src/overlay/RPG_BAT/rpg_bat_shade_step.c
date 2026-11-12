/* @SymbolStart: 0x80159214 */
/* @SymbolSize: 1036 */
/* @RodataBase: 0x8015E300 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x394 --offset2 0x3b4 */
/* @PostProc: swap_instructions --offset1 0x39c --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x3a0 --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x3a4 --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x3a8 --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x3b0 --offset2 0x3b4 */
/* @PostProc: patch_insn_reg_field 0x8c:rt:v0:t1 0x94:rs:v0:t1 0xfc:rt:v0:t1 0x104:rs:v0:t1 0x120:rd:v1:v0 0x124:rs:v1:v0 0x128:rt:v1:v0 0x12c:rs:v1:v0 0x138:rt:v1:v0 0x13c:rt:v1:v0 0x144:rt:v1:v0 0x148:rt:v0:t1 0x14c:rs:v0:t1 0x158:rs:v0:t1 0x15c:rd:v1:v0 0x15c:rt:v0:t1 0x160:rs:v0:t1 0x164:rd:v1:v0 0x168:rt:v1:v0 0x16c:rd:a0:v1 0x16c:rs:t3:v0 0x16c:rt:v1:t3 0x170:rt:a0:v1 0x178:rt:v0:t1 0x180:rs:t4:v1 0x180:rt:a0:t4 0x18c:rt:v0:t1 0x22c:rt:v0:v1 0x234:rs:v0:v1 0x23c:rt:v0:v1 0x244:rt:v0:v1 0x24c:rs:v0:v1 0x284:rs:t4:v0 0x284:rt:v0:t4 0x28c:rs:t5:v1 0x28c:rt:v1:t5 0x38c:rt:at:v0 0x398:rs:t2:v0 0x398:rt:t2:v0 0x3a8:rd:t3:t2 0x3a8:rs:t2:v0 0x3ac:rs:t3:t2 0x3ac:rt:t4:t3 0x3b0:rt:t2:at 0x3b4:rt:t4:t3 */
/* rpg_bat_shade_step - RPG_BAT overlay, Address: 0x8015921C, Size: 1028 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 p_sei[];
extern s32 rpg_bat_shade_step_str0[];
extern s32 rpg_bat_shade_step_str1[];
extern s32 rpg_bat_msg_buf2[];
extern s32 rpg_bat_msg_tbl2[];
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_unit0;
extern s32 rpg_bat_unit0_flag;
extern s32 rpg_bat_unit1_flag;
extern s32 rpg_bat_unit1_step;
extern s32 rpg_bat_unit1_cnt;
extern CLASS_W class_w[];

extern void dtd_on_tpage();
extern void _sprite_set_box_shade();
extern void rpg_bat_anim_set();
extern void rpg_bat_sprite_pos_set();
extern void rpg_bat_slot_adjust();
extern void rpg_bat_se_set();
extern void rpg_bat_damage_set();
extern void rpg_bat_msg_set();
extern void suuzi_str();
extern void rpg_bat_seq_step_next();
extern void rpg_bat_effect_clear();
extern void rpg_bat_seq_wait();
extern void rpg_bat_seq_step_gate();
extern void rpg_bat_effect_pick();

void rpg_bat_shade_step(void)
{
    s32 n;
    s32 v;
    s32 c;

    switch ((u32)rpg_bat_unit1_step) {
    case 0:
        rpg_bat_effect_pick();
        return;
    case 1:
        rpg_bat_anim_set(59, 9, 5, 1);
        rpg_bat_se_set(1290, 1, 0);
        rpg_bat_seq_step_next();
        return;
    case 2:
        n = rpg_bat_unit1_cnt;
        if (n == 110) {
            rpg_bat_sprite_pos_set(58, 0, 0);
            class_w[154].scale_x = 256;
            class_w[154].scale_y = 256;
            class_w[154].prio = 4;
            rpg_bat_anim_set(58, 11, 1, 1);
            rpg_bat_se_set(1291, 1, 0);
            rpg_bat_seq_step_next();
            n = rpg_bat_unit1_cnt;
        }
        rpg_bat_unit1_cnt = n + 1;
        return;
    case 3:
        n = rpg_bat_unit1_cnt;
        v = (n << 13) / 256;
        class_w[154].scale_x = v + 256;
        class_w[154].scale_y = v + 256;
        n = n + 3;
        if (n < 256) {
            v = n / 2;
            c = v + (v << 8);
            c = c + (c << 8);
            rpg_bat_unit1_cnt = n;
        } else {
            rpg_bat_unit1_cnt = n;
            c = 0x007FFE7F;
        }
        _sprite_set_box_shade(-160, -120, 320, 168, 2, c, 129);
        dtd_on_tpage(0, 0, 2, 1, 0);
        if (rpg_bat_unit1_cnt >= 501) {
            class_w[154].scale_x = 4096;
            class_w[154].scale_y = 4096;
            class_w[154].prio = 5;
            rpg_bat_anim_set(58, 255, 1, 0);
            rpg_bat_anim_set(59, 8, 1, 1);
            n = rpg_bat_unit0_flag;
            if (n == 0x02000000) {
                n = 256;
            }
            rpg_bat_unit0_flag = n;
            if (n != 256) {
                rpg_bat_anim_set(48, 5, 1, 1);
            }
            rpg_bat_seq_step_next();
        }
        return;
    case 4:
        n = rpg_bat_unit1_cnt;
        v = 127 - n;
        c = v + (v << 8);
        c = c + (c << 8);
        _sprite_set_box_shade(-160, -120, 320, 168, 2, c, 129);
        dtd_on_tpage(0, 0, 2, 1, 0);
        rpg_bat_unit1_cnt = rpg_bat_unit1_cnt + 7;
        if (rpg_bat_unit1_cnt >= 128) {
            rpg_bat_seq_step_next();
        }
        return;
    case 5:
        rpg_bat_damage_set();
        suuzi_str(rpg_bat_msg_buf2, p_sei, rpg_bat_shade_step_str0, rpg_bat_dmg,
                      rpg_bat_shade_step_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_seq_step_next();
        return;
    case 6:
        rpg_bat_seq_step_gate(1);
        return;
    case 7:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 1);
        if (rpg_bat_unit0 == 0) {
            rpg_bat_unit1_step = 0;
            rpg_bat_unit1_flag = (rpg_bat_unit1_flag & 0xFFFF0000) | 5;
            rpg_bat_unit0_flag = 4096;
            rpg_bat_anim_set(48, 7, 1, 1);
        }
        rpg_bat_seq_step_next();
        return;
    case 8:
        rpg_bat_seq_step_gate(0);
        return;
    case 9:
        rpg_bat_seq_wait(20);
        return;
    case 10:
        rpg_bat_effect_clear();
        return;
    }
}
