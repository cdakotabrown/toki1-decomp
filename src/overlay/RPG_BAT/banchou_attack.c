/* @RodataBase: 0x8015E2B0 */
/* @SymbolStart: 0x80158C4C */
/* @SymbolSize: 1056 */
/* @PostProc: scheduler_normalizer --branch-fp --const-fp */
/* @PostProc: patch_insn_reg_field 0x3a0:rt:t1:v0 0x3b4:rs:t1:v0 0x3b4:rt:t1:v0 0x3c4:rd:t2:t1 0x3c4:rs:t1:v0 0x3c8:rs:t2:t1 0x3c8:rt:t3:t2 0x3d0:rt:t3:t2 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern u8 banchou_attack_str0[];
extern u8 banchou_attack_str1[];
extern u8 rpg_bat_msg_buf2[];
extern u8 rpg_bat_msg_tbl2[];
extern u8 p_sei[];
extern u8 *rpg_bat_dmg;
extern s32 rpg_bat_seq_work;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern void rpg_bat_anim_set();
extern void rpg_bat_anim_set2();
extern void rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();
extern void rpg_bat_slot_adjust();
extern void rpg_bat_se_set();
extern void rpg_bat_damage_set();
extern void rpg_bat_msg_set();
extern void suuzi_str();
extern void rpg_bat_seq_step_next();
extern void rpg_bat_effect_clear();
extern void rpg_bat_seq_wait();
extern void rpg_bat_seq_step_gate();
extern void banchou_attack_call_talk();
extern void rpg_bat_turn_wait_substep();

void banchou_attack(void)
{
    s32 step;
    s32 v0;
    s32 v1;

    step = rpg_bat_unit1.step;
    switch ((u32)step) {
    case 0:
        if (get_rand_mod(10) == 0) {
            if (rpg_bat_unit0.flag == 0) {
                rpg_bat_seq_step_next();
            } else {
                rpg_bat_unit1.step = 2;
            }
        } else {
            rpg_bat_unit1.step = 2;
        }
        break;
    case 1:
        rpg_bat_turn_wait_substep();
        break;
    case 2:
        banchou_attack_call_talk();
        break;
    case 3:
        rpg_bat_seq_wait(60);
        break;
    case 4:
        v0 = rpg_bat_unit0.flag;
        if (v0 == 0) {
            rpg_bat_unit0.flag = v0 | 0x10000;
            rpg_bat_unit0.aux08 = 0;
        }
        (*(s32 *)&class_w[154].attribute) = 0x51000000;
        class_w[154].prio = 4;
        class_w[125].prio = 4;
        rpg_bat_se_set(1292, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 5:
        if ((rpg_bat_seq_work << 2) < 64) {
            rpg_bat_sprite_pos_set(58, (rpg_bat_seq_work << 2) + 96, 80);
            rpg_bat_anim_set2(58, 4, 1, 1);
        } else {
            rpg_bat_sprite_pos_set(58, (rpg_bat_seq_work << 2) + 96, 56);
            rpg_bat_anim_set2(58, 5, 1, 1);
        }
        if ((rpg_bat_seq_work % 4) < 2) {
            class_w[154].clut = 19;
        } else {
            class_w[154].clut = 18;
        }
        rpg_bat_seq_work += 1;
        if (((rpg_bat_seq_work << 2) + 96) >= 265) {
            rpg_bat_sprite_pos_set(58, 0, -8);
            rpg_bat_anim_set(58, 5, 1, 1);
            rpg_bat_se_set(1293, 1, 0);
            rpg_bat_seq_step_next();
        }
        break;
    case 6:
        if ((rpg_bat_seq_work % 4) < 2) {
            class_w[154].clut = 19;
        } else {
            class_w[154].clut = 18;
        }
        rpg_bat_seq_work += 1;
        v0 = (rpg_bat_seq_work << 2) + 96;
        if (v0 < 512) {
            if (rpg_bat_unit0.flag == 0x02000000) {
                rpg_bat_unit0.flag = 256;
            }
        }
        if (v0 >= 641) {
            if (rpg_bat_unit0.flag != 256) {
                rpg_bat_anim_set(48, 5, 1, 1);
            }
            rpg_bat_seq_step_next();
        }
        break;
    case 7:
        rpg_bat_anim_set2(58, 255, 1, 0);
        (*(s32 *)&class_w[154].attribute) = 0x41000000;
        class_w[154].clut = 18;
        rpg_bat_seq_step_next();
        break;
    case 8:
        rpg_bat_damage_set();
        suuzi_str(rpg_bat_msg_buf2, p_sei, banchou_attack_str0, rpg_bat_dmg,
                      banchou_attack_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_seq_step_next();
        break;
    case 9:
        rpg_bat_seq_step_gate(1);
        break;
    case 10:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 1);
        if (rpg_bat_unit0.hp == 0) {
            rpg_bat_unit0.flag = 4096;
            rpg_bat_unit1.step = 0;
            rpg_bat_unit1.flag = (rpg_bat_unit1.flag & 0xFFFF0000) | 5;
            rpg_bat_anim_set(48, 7, 1, 1);
        }
        rpg_bat_seq_step_next();
        break;
    case 11:
        rpg_bat_seq_step_gate(0);
        break;
    case 12:
        rpg_bat_seq_wait(20);
        break;
    case 13:
        rpg_bat_effect_clear();
        break;
    }
}
