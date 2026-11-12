/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x80152080 */
/* @RodataBase: 0x8015D864 */
/* @SymbolSize: 772 */
/* @PostProc: patch_stack_complete --old 56 --new 64 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x12c:rt:v1:v0 0x130:rs:v1:v0 0x15c:rt:v1:v0 0x160:rs:v1:v0 0x1b8:rt:t4:v1 0x1c0:rt:t4:v1 0x1d8:rt:v1:a1 0x1e8:rs:v1:a1 0x228:rt:v1:a1 0x238:rs:v1:a1 0x28c:rt:t5:t4 0x298:rt:t5:t4 */
/* monster_attack - RPG_BAT overlay, Address: 0x80152088, Size: 764 bytes (191 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 strcpy();
extern s32 rpg_bat_player_pose_set();
extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_rec_flag_check();
extern s32 rpg_bat_sprite_pos_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_damage_set();
extern s32 rpg_bat_msg_set();
extern s32 suuzi_str();
extern s32 rpg_bat_seq_step_next();
extern s32 rpg_bat_effect_clear();
extern s32 rpg_bat_seq_wait();
extern s32 rpg_bat_seq_step_gate();
extern s32 rpg_bat_mode_show();

extern char p_sei[];
extern char monster_attack_str0[];
extern char monster_attack_str1[];
extern char monster_attack_str2[];
extern char monster_attack_str3[];
extern char monster_attack_str4[];
extern char rpg_bat_msg_buf2[];
extern char rpg_bat_msg_buf2_1[];
extern char rpg_bat_msg_tbl2[];

extern s32 rpg_bat_mode;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_msg_ctl_mode;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void monster_attack(void)
{
    s32 v0;
    s32 v1;

    switch ((u32)rpg_bat_unit1.step) {
    case 0:
        rpg_bat_damage_set();
        v1 = rpg_bat_mode;
        switch (v1) {
        case 0x10000:
            strcpy((s32)rpg_bat_msg_buf2, (s32)monster_attack_str0);
            break;
        case 0x20000:
            strcpy((s32)rpg_bat_msg_buf2, (s32)monster_attack_str1);
            break;
        case 0x40000:
            strcpy((s32)rpg_bat_msg_buf2, (s32)monster_attack_str2);
            break;
        }
        suuzi_str((s32)rpg_bat_msg_buf2_1, (s32)p_sei, (s32)monster_attack_str3,
                      rpg_bat_dmg, (s32)monster_attack_str4);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl2, 2, 60);
        rpg_bat_seq_step_next();
        break;

    case 1:
        v0 = rpg_bat_unit1.cnt + 1;
        rpg_bat_msg_ctl_mode = 3;
        if ((v0 % 3) == 0) {
            class_w[155].r = 255;
        } else {
            class_w[155].r = 0;
        }
        rpg_bat_unit1.cnt = v0;
        if (v0 >= 16) {
            rpg_bat_seq_step_next();
        }
        break;

    case 2:
        if ((rpg_bat_unit0.flag & 0x100) == 0) {
            rpg_bat_anim_set(48, 5, 1, 1);
        }
        class_w[152].prio = 3;
        rpg_bat_mode_show();
        rpg_bat_seq_work = get_rand_mod(4) + 1;
        v0 = get_rand_mod(20);
        v1 = get_rand_mod(20);
        rpg_bat_sprite_pos_set(56, v0 - 5, v1 - 5);
        rpg_bat_seq_step_next();
        break;

    case 3:
        if (rpg_bat_rec_flag_check(56) != 0) {
            rpg_bat_mode_show();
            v0 = get_rand_mod(20);
            v1 = get_rand_mod(20);
            rpg_bat_sprite_pos_set(56, v0 - 5, v1 - 5);
            v1 = rpg_bat_seq_work - 1;
            rpg_bat_seq_work = v1;
            if (v1 == 0) {
                rpg_bat_seq_step_next();
            }
        }
        break;

    case 4:
        if (rpg_bat_rec_flag_check(56) != 0) {
            rpg_bat_anim_set(56, 255, 5, 0);
            rpg_bat_seq_step_next();
            rpg_bat_msg_ctl_mode = 1;
        }
        break;

    case 5:
        rpg_bat_seq_step_gate(1);
        break;

    case 6:
        rpg_bat_slot_adjust(0, 33, rpg_bat_dmg, 1);
        rpg_bat_seq_step_next();
        break;

    case 7:
        rpg_bat_seq_step_gate(0);
        break;

    case 8:
        rpg_bat_seq_wait(20);
        break;

    case 9:
        rpg_bat_player_pose_set();
        rpg_bat_effect_clear();
        break;
    }
}
