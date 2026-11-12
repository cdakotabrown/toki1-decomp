/* @RodataBase: 0x8015B8FC */
/* @SymbolStart: 0x80138F6C */
/* @SymbolSize: 1268 */
/* @PostProc: patch_insn_reg_field 0x44:rt:v0:v1 0x48:rs:v0:v1 0x48:rt:v0:v1 0x50:rs:v0:v1 0x58:rs:v0:v1 0x64:rs:v0:v1 0x170:rt:v0:v1 0x174:rs:v0:v1 0x174:rt:v0:v1 0x188:rs:v0:v1 0x1ac:rt:v0:v1 0x1b0:rs:v0:v1 0x1b0:rt:v0:v1 0x1b8:rs:v0:v1 0x1d4:rt:v0:v1 0x1d8:rs:v0:v1 0x1d8:rt:v0:v1 0x1ec:rs:v0:v1 */
/* rpg_bat_intro_step - RPG_BAT overlay, Address: 0x80138F74, Size: 1260 bytes (315 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern void strcpy(char *, char *);
extern void strcat(char *, char *);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern s32 rpg_bat_rec_flag_check(s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern s32 get_rand_mod(s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_msg_set(char *, s32, s32);
extern void suuzi_str(char *, char *, char *, s32, char *);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_effect_clear(void);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_seq_step_gate(s32);
extern void rpg_bat_flag_seq(s32);

extern s32 rpg_bat_target_no;
extern s32 rpg_bat_dmg;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_seq_work;

extern s32 date_name_set_tbl0[];

extern char rpg_bat_msg_buf2[];
extern char rpg_bat_msg_buf2_1[];
extern char rpg_bat_msg_buf2_2[];
extern char rpg_bat_msg_buf2_3[];
extern char rpg_bat_msg_tbl2[];
extern char p_sei[];
extern char rpg_bat_intro_step_str0[];
extern char rpg_bat_intro_step_str1[];
extern char rpg_bat_intro_step_str2[];
extern char rpg_bat_intro_step_str3[];
extern char rpg_bat_intro_step_str4[];
extern char rpg_bat_intro_step_str5[];
extern char rpg_bat_intro_step_str6[];
extern char rpg_bat_intro_step_str7[];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

void rpg_bat_intro_step(void)
{
    s32 frame;
    s32 flag;
    s32 t1;
    s32 t2;

    flag = rpg_bat_unit1.step;
    switch (flag) {
    case 0:
        switch (rpg_bat_target_no) {
        case 0:
            strcpy(rpg_bat_msg_buf2, rpg_bat_intro_step_str0);
            break;
        case 1:
            strcpy(rpg_bat_msg_buf2, rpg_bat_intro_step_str1);
            break;
        case 2:
            strcpy(rpg_bat_msg_buf2, rpg_bat_intro_step_str2);
            break;
        default:
            break;
        }
        strcpy(rpg_bat_msg_buf2_1, rpg_bat_intro_step_str3);
        strcpy(rpg_bat_msg_buf2_2, p_sei);
        strcat(rpg_bat_msg_buf2_2, rpg_bat_intro_step_str4);
        strcat(rpg_bat_msg_buf2_2, (char *)date_name_set_tbl0[rpg_bat_girl]);
        strcat(rpg_bat_msg_buf2_2, rpg_bat_intro_step_str5);
        suuzi_str(rpg_bat_msg_buf2_3, p_sei, rpg_bat_intro_step_str6, rpg_bat_dmg,
                      rpg_bat_intro_step_str7);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 4, 60);
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 1, 5, 1);
        rpg_bat_se_set(1285, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_flag_seq(rpg_bat_target_no + 59);
        break;
    case 2:
        rpg_bat_seq_step_gate(1);
        break;
    case 3:
        rpg_bat_anim_set2(rpg_bat_target_no + 59, 0, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 4:
        rpg_bat_unit1.cnt = rpg_bat_unit1.cnt + 1;
        frame = rpg_bat_unit1.cnt;
        rpg_bat_sprite_pos_set(48, (frame * -24) / 4, (frame * 48) / 4);
        if (rpg_bat_unit1.cnt == 4) {
            rpg_bat_seq_step_next();
        }
        break;
    case 5:
        rpg_bat_anim_set(48, 5, 1, 1);
        class_w[152].prio = 3;
        rpg_bat_anim_set(56, 12, 5, 1);
        rpg_bat_seq_work = get_rand_mod(4) + 1;
        t1 = get_rand_mod(20);
        t2 = get_rand_mod(20);
        rpg_bat_sprite_pos_set(56, t1 - 29, t2 + 43);
        rpg_bat_se_set(1286, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 6:
        if (rpg_bat_rec_flag_check(56) == 0) {
            return;
        }
        rpg_bat_se_set(1286, 1, 0);
        t1 = get_rand_mod(20);
        t2 = get_rand_mod(20);
        rpg_bat_sprite_pos_set(56, t1 - 29, t2 + 43);
        rpg_bat_anim_set(56, 12, 5, 1);
        rpg_bat_seq_work -= 1;
        if (rpg_bat_seq_work != 0) {
            return;
        }
        rpg_bat_seq_step_next();
        break;
    case 7:
        if (rpg_bat_rec_flag_check(56) == 0) {
            return;
        }
        rpg_bat_anim_set(56, 255, 5, 0);
        rpg_bat_seq_step_next();
        break;
    case 8:
        rpg_bat_seq_step_gate(3);
        break;
    case 9:
        rpg_bat_slot_adjust(0, 32, rpg_bat_dmg, 1);
        if (rpg_bat_unit0.hp == 0) {
            rpg_bat_effect_clear();
            rpg_bat_unit1.flag = 0x00100007;
            return;
        }
        rpg_bat_seq_step_next();
        break;
    case 10:
        rpg_bat_seq_step_gate(0);
        break;
    case 11:
        rpg_bat_unit1.cnt = rpg_bat_unit1.cnt + 1;
        frame = rpg_bat_unit1.cnt;
        frame = 4 - frame;
        rpg_bat_sprite_pos_set(48, (frame * -24) / 4, (frame * 48) / 4);
        if (rpg_bat_unit1.cnt == 4) {
            rpg_bat_seq_step_next();
        }
        break;
    case 12:
        rpg_bat_seq_wait(20);
        break;
    case 13:
        rpg_bat_unit0.flag &= ~0x1000;
        rpg_bat_effect_clear();
        break;
    default:
        break;
    }
}
