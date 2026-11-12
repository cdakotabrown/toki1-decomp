/* @SymbolStart: 0x80157158 */
/* @SymbolSize: 1256 */
/* @RodataBase: 0x8015DDEC */
/* @PostProc: patch_stack_complete --old 40 --new 64 */
/* @PostProc: remap_stack_slots --from 32 --to 52 */
/* @PostProc: swap_instructions --offset1 0x268 --offset2 0x270 */
/* @PostProc: swap_instructions --offset1 0x340 --offset2 0x348 */
/* @PostProc: swap_instructions --offset1 0x424 --offset2 0x42C */
/* @PostProc: swap_instructions --offset1 0x428 --offset2 0x42C */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x4c:rt:v0:v1 0x50:rs:v0:v1 0x50:rt:v0:v1 0x58:rs:v0:v1 0x60:rs:v0:v1 0x78:rt:v0:v1 0x7c:rs:v0:v1 0x7c:rt:v0:v1 0x84:rs:v0:v1 0x8c:rs:v0:v1 0xb4:rs:v0:v1 0xbc:rt:v0:v1 0x150:rt:a3:v1 0x158:rs:a3:v1 0x158:rt:a3:v1 0x168:rs:a3:v1 0x174:rs:a3:v1 0x174:rt:a3:v1 0x178:rt:a3:v1 0x18c:rt:a3:v1 0x190:rs:a3:v1 0x190:rt:a3:v1 0x198:rs:a3:v1 0x274:rt:v1:a1 0x284:rs:v1:a1 0x2ac:rt:v0:v1 0x2b8:rt:v0:v1 0x34c:rt:v1:a1 0x35c:rs:v1:a1 0x384:rt:v0:v1 0x38c:rs:v0:v1 0x38c:rt:v0:v1 0x398:rt:v0:v1 0x3a4:rs:v0:v1 0x414:rt:v0:v1 0x418:rs:v0:v1 0x418:rt:v0:v1 0x420:rs:v0:v1 0x424:rt:t4:v0 0x430:rs:t4:v0 0x430:rt:t4:v0 0x43c:rd:t5:t4 0x43c:rs:t4:v0 0x440:rs:t5:t4 0x440:rt:t6:t5 0x448:rt:t6:t5 0x490:rt:a3:v1 0x494:rs:a3:v1 0x494:rt:a3:v1 0x498:rt:t7:t6 0x4a0:rd:t8:t7 0x4a0:rs:t7:t6 0x4a0:rt:a3:v1 0x4a4:rt:t8:t7 0x4ac:rs:a3:v1 0x4ac:rt:v0:v1 0x4b0:rt:v0:v1 0x4b4:rs:v0:v1 */
/* utyuuzin_attack2 - RPG_BAT overlay, Address: 0x80157160, Size: 1248 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 p_sei[];
extern s32 utyuuzin_attack2_str0[];
extern s32 utyuuzin_attack2_str1[];
extern s32 utyuuzin_attack2_str2[];
extern s32 rpg_bat_msg_buf2[];
extern s32 rpg_bat_msg_tbl2[];
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_bg_wave_phase;
extern s32 rpg_bat_seq_work;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern void strcpy();
extern void rpg_bat_player_pose_set();
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
extern void rpg_bat_flag_seq();
extern void rpg_bat_seq_step_gate();

void utyuuzin_attack2(void)
{
    s32 sel;
    s32 tmp;
    s32 tmp2;

    sel = rpg_bat_unit1.step;
    if (sel < 80) {
        if (rpg_bat_unit0.flag == 0x02000000) {
            rpg_bat_unit0.flag = 256;
        }
        rpg_bat_player_pose_set();
        sel = rpg_bat_unit1.step;
    }
    if (sel >= 4 && sel < 81) {
        rpg_bat_se_set(1287, 8, 3);
        sel = rpg_bat_unit1.step;
    }
    switch (sel) {
    case 0:
        strcpy(rpg_bat_msg_buf2, utyuuzin_attack2_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_unit0.flag = rpg_bat_unit0.flag | 0x1000;
        rpg_bat_se_set(1299, 1, 0);
        rpg_bat_seq_step_next();
        return;
    case 1:
        rpg_bat_seq_step_gate(0);
        return;
    case 2:
        rpg_bat_unit0.flag = rpg_bat_unit0.flag & ~0x1000;
        tmp = rpg_bat_unit1.cnt;
        rpg_bat_bg_wave_phase = tmp + 64;
        rpg_bat_unit1.cnt = tmp + 4;
        rpg_bat_se_set(1286, 0, 2);
        if (rpg_bat_unit1.cnt >= 209) {
            rpg_bat_seq_step_next();
        }
        return;
    case 3:
        rpg_bat_seq_wait(20);
        return;
    case 4:
        rpg_bat_sprite_pos_set(40, 0, 0);
        (*(s32 *)&class_w[136].attribute) = 0x51000000;
        rpg_bat_anim_set(40, 3, 5, 1);
        rpg_bat_seq_step_next();
        return;
    case 5:
        rpg_bat_flag_seq(40);
        return;
    case 6:
        rpg_bat_anim_set(40, 4, 1, 1);
        rpg_bat_seq_work = get_rand_mod(4) + 3;
        if (rpg_bat_unit0.flag != 256) {
            rpg_bat_anim_set(48, 5, 1, 1);
        }
        tmp = get_rand_mod(16);
        tmp2 = get_rand_mod(16);
        rpg_bat_sprite_pos_set(41, tmp + 264, tmp2 + 80);
        rpg_bat_anim_set2(41, 5, 5, 1);
        rpg_bat_se_set(1288, 1, 0);
        rpg_bat_unit1.step = 80;
        return;
    case 80:
        if ((class_w[137].anim_flag & 1) == 0) {
            rpg_bat_se_set(1288, 1, 0);
            rpg_bat_seq_work = rpg_bat_seq_work - 1;
            if (rpg_bat_seq_work == 0) {
                (*(s32 *)&class_w[136].attribute) = 0x41000000;
                rpg_bat_anim_set2(40, 255, 1, 0);
                rpg_bat_anim_set2(41, 255, 1, 0);
                rpg_bat_seq_step_next();
                return;
            }
            tmp = get_rand_mod(16);
            tmp2 = get_rand_mod(16);
            rpg_bat_sprite_pos_set(41, tmp + 264, tmp2 + 80);
            rpg_bat_anim_set2(41, 5, 5, 1);
        }
        return;
    case 81:
        rpg_bat_damage_set();
        if (rpg_bat_dmg >= rpg_bat_unit0.hp) {
            rpg_bat_dmg = rpg_bat_unit0.hp - 1;
        }
        suuzi_str(rpg_bat_msg_buf2, p_sei, utyuuzin_attack2_str1, rpg_bat_dmg,
                      utyuuzin_attack2_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_seq_step_next();
        return;
    case 82:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 1);
        if (rpg_bat_unit0.hp == 1) {
            tmp = rpg_bat_unit1.flag;
            rpg_bat_unit0.flag = 4096;
            rpg_bat_unit1.flag = (tmp & 0xFFFF0000) | 3;
            rpg_bat_unit1.step = 0;
            rpg_bat_anim_set(48, 4, 1, 1);
            return;
        }
        rpg_bat_seq_step_next();
        return;
    case 83:
        rpg_bat_seq_step_gate(0);
        return;
    case 84:
        tmp = rpg_bat_unit1.cnt;
        rpg_bat_bg_wave_phase = 272 - tmp;
        tmp = tmp + 4;
        rpg_bat_unit1.cnt = tmp;
        if (tmp >= 209) {
            rpg_bat_seq_step_next();
        }
        return;
    case 85:
        rpg_bat_seq_wait(20);
        return;
    case 86:
        rpg_bat_effect_clear();
        return;
    }
}
