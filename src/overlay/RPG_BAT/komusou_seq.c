/* @RodataBase: 0x8015C968 */
/* @SymbolStart: 0x80146C30 */
/* @SymbolSize: 2984 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x854 --offset2 0x858 */
/* @PostProc: patch_insn_reg_field 0x1c4:rt:v0:a3 0x1c8:rt:v0:a3 0x2d4:rt:v0:a3 0x2d8:rt:v0:a3 0x2fc:rs:v0:a3 0x84c:rt:v0:v1 0x850:rt:v1:v0 0x854:rs:v1:v0 0x854:rt:v1:v0 0x858:rs:v0:v1 0x858:rt:v0:v1 0x85c:rs:v0:v1 0x860:rs:v0:v1 0x860:rt:v0:v1 0x868:rs:v0:v1 0x868:rt:v1:v0 0x86c:rs:v0:v1 */
/* Address: 0x80146C38 (true start 0x80146C30: the selector load is */
/* Size: 2976 bytes (744 insns) + 8 hoisted = 2984 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 sub_step_up(void);
extern void device_port_init(s32, s32);
extern s32 get_CD_State(void);
extern void k_disp_switch(s32, s32);
extern s32 rsin(s32);
extern void strcpy(char *, char *);
extern void strcat(char *, char *);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern s32 get_rand_mod(s32);
extern void rpg_bat_slot_adjust(s32, s32, s32, s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void komusou_volunteer_talk(void);
extern void rpg_bat_talk2a(void);
extern void komusou_leave_talk(void);
extern void rpg_bat_msg_set(char *, s32, s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_effect_clear(void);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_flag_seq(s32);
extern void rpg_bat_seq_step_gate(s32);
extern void rpg_bat_pick_list_set(void);

extern s32 rpg_bat_seq_msg_line_cnt;
extern s32 rpg_bat_girl;
extern s32 rpg_bat_komusou_sway_phase[];
extern s32 rpg_bat_komusou_sway_end;

extern char rpg_bat_msg_buf2[];
extern char rpg_bat_msg_tbl2[];
extern char rpg_bat_msg_buf4[];
extern char rpg_bat_msg4_line1[];
extern char rpg_bat_msg4_line2[];
extern char rpg_bat_msg4_line3[];
extern char rpg_bat_msg_tbl4[];
extern char komusou_seq_str0[];
extern char komusou_seq_str1[];
extern char komusou_seq_str2[];
extern char komusou_seq_str3[];
extern char komusou_seq_str4[];
extern char komusou_seq_str5[];
extern char komusou_seq_str6[];
extern char komusou_seq_str7[];
extern char komusou_seq_str8[];
extern char komusou_seq_str9[];
extern char p_sei[];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit3;

void komusou_seq(void)
{
    s32 *p;

    switch (rpg_bat_unit1.step) {
    case 0:
        strcpy(rpg_bat_msg_buf2, komusou_seq_str0);
        rpg_bat_sprite_pos_set(56, 0, 0);
        rpg_bat_sprite_pos_set(57, 0, 0);
        class_w[155].prio = 4;
        class_w[152].prio = 4;
        class_w[153].prio = 4;
        rpg_bat_anim_set(56, 3, 5, 1);
        rpg_bat_anim_set(57, 1, 5, 1);
        rpg_bat_se_set(1288, 1, 2);
        rpg_bat_seq_step_next();
        break;
    case 1:
        if ((class_w[152].anim_flag & 1) == 0) {
            rpg_bat_anim_set(56, 255, 1, 0);
        }
        rpg_bat_seq_wait(50);
        break;
    case 2:
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_anim_set(56, 255, 1, 0);
        rpg_bat_anim_set(57, 255, 1, 0);
        rpg_bat_sprite_pos_set(59, 0, 0);
        rpg_bat_anim_set(59, 2, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 3:
        rpg_bat_seq_step_gate(1);
        break;
    case 4:
        rpg_bat_unit1.cnt += 6;
        rpg_bat_se_set(1287, 0, 1);
        rpg_bat_sprite_pos_set(59, (s32)((u32)rpg_bat_unit1.cnt * 144U) / 100, 0);
        if (rpg_bat_unit1.cnt < 101) {
            break;
        }
        rpg_bat_sprite_pos_set(59, 144, 0);
        rpg_bat_seq_step_next();
        break;
    case 5:
        rpg_bat_anim_set(48, 10, 1, 1);
        rpg_bat_slot_adjust(0, 33, rpg_bat_unit0.hp - 3, 1);
        rpg_bat_unit0.flag = 0x1000;
        rpg_bat_se_set(1285, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 6:
        rpg_bat_seq_wait(30);
        break;
    case 7:
        rpg_bat_unit1.cnt += 6;
        rpg_bat_anim_set(59, 0, 1, 1);
        rpg_bat_anim_set(48, 4, 1, 1);
        rpg_bat_sprite_pos_set(59, 144 - (s32)((u32)rpg_bat_unit1.cnt * 144U) / 100, 0);
        if (rpg_bat_unit1.cnt < 101) {
            break;
        }
        rpg_bat_sprite_pos_set(59, 0, 0);
        rpg_bat_seq_step_next();
        break;
    case 8:
        rpg_bat_seq_wait(10);
        break;
    case 9:
        strcpy(rpg_bat_msg_buf2, p_sei);
        strcat(rpg_bat_msg_buf2, komusou_seq_str1);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 120);
        rpg_bat_seq_step_next();
        break;
    case 10:
        rpg_bat_seq_step_gate(0);
        break;
    case 11:
        komusou_volunteer_talk();
        break;
    case 12:
        strcpy(rpg_bat_msg_buf2, p_sei);
        strcat(rpg_bat_msg_buf2, komusou_seq_str2);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 60);
        rpg_bat_seq_step_next();
        break;
    case 13:
        rpg_bat_seq_step_gate(0);
        break;
    case 14:
        rpg_bat_unit1.cnt += 1;
        if (rpg_bat_unit1.cnt < 61) {
            rpg_bat_unit1.cnt = rpg_bat_unit1.cnt;
            if ((rpg_bat_unit1.cnt & 1) == 0) {
                rpg_bat_anim_set(48, 255, 1, 0);
                if (rpg_bat_girl != 14) {
                    rpg_bat_anim_set(29, 255, 1, 0);
                }
            } else {
                rpg_bat_anim_set(48, 255, 1, 1);
                if (rpg_bat_girl != 14) {
                    rpg_bat_anim_set(29, 255, 1, 1);
                }
            }
        }
        rpg_bat_sprite_pos_set(40, 0, 0);
        if (rpg_bat_unit1.cnt >= 61) {
            rpg_bat_unit1.cnt = rpg_bat_unit1.cnt;
            if ((rpg_bat_unit1.cnt & 1) == 0) {
                rpg_bat_anim_set(40, 0, 1, 1);
            } else {
                rpg_bat_anim_set(40, 0, 1, 0);
            }
        }
        if (rpg_bat_unit1.cnt < 120) {
            break;
        }
        rpg_bat_seq_step_next();
        break;
    case 15:
        rpg_bat_unit3.hp ^= 0x10000000;
        strcpy(rpg_bat_msg_buf2, komusou_seq_str3);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_seq_step_next();
        break;
    case 16:
        rpg_bat_seq_step_gate(1);
        break;
    case 17:
        rpg_bat_talk2a();
        break;
    case 18:
        strcpy(rpg_bat_msg_buf2, komusou_seq_str4);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_seq_step_next();
        break;
    case 19:
        rpg_bat_seq_step_gate(1);
        break;
    case 20:
        rpg_bat_anim_set(40, 1, 5, 1);
        rpg_bat_seq_step_next();
        break;
    case 21:
        rpg_bat_flag_seq(40);
        break;
    case 22:
        class_w[155].prio = 6;
        rpg_bat_anim_set(40, 2, 1, 1);
        rpg_bat_komusou_sway_phase[0] = get_rand_mod(1024);
        rpg_bat_komusou_sway_phase[1] = get_rand_mod(1024) + 1024;
        rpg_bat_komusou_sway_phase[2] = get_rand_mod(1024) + 2048;
        rpg_bat_anim_set(41, get_rand_mod(2) + 3, 1, 1);
        rpg_bat_anim_set(42, get_rand_mod(2) + 3, 1, 1);
        rpg_bat_anim_set(43, get_rand_mod(2) + 3, 1, 1);
        rpg_bat_sprite_pos_set(41, 16, (rsin(rpg_bat_komusou_sway_phase[0]) >> 8) + 32);
        rpg_bat_sprite_pos_set(42, 16, (rsin(rpg_bat_komusou_sway_phase[1]) >> 8) - 16);
        rpg_bat_sprite_pos_set(43, 16, (rsin(rpg_bat_komusou_sway_phase[2]) >> 8) - 64);
        rpg_bat_koe_set(0x0C00003A, 0x06000062, 9);
        rpg_bat_se_set(1289, 1, 3);
        rpg_bat_seq_step_next();
        break;
    case 23:
        rpg_bat_unit1.cnt += 1;
        rpg_bat_sprite_pos_set(41, 16 - rpg_bat_unit1.cnt / 2,
                      (rsin(rpg_bat_komusou_sway_phase[0]) >> 8) + 32);
        rpg_bat_sprite_pos_set(42, 16 - rpg_bat_unit1.cnt / 2,
                      (rsin(rpg_bat_komusou_sway_phase[1]) >> 8) - 16);
        rpg_bat_sprite_pos_set(43, 16 - rpg_bat_unit1.cnt / 2,
                      (rsin(rpg_bat_komusou_sway_phase[2]) >> 8) - 64);
        p = rpg_bat_komusou_sway_phase;
        do {
            *p += 40;
            p += 1;
        } while (p != &rpg_bat_komusou_sway_end);
        if (rpg_bat_unit1.cnt >= 545) {
            rpg_bat_anim_set(40, 0, 1, 1);
            rpg_bat_se_set(1287, 1, 4);
            rpg_bat_seq_step_next();
        }
        if (get_CD_State() != 2) {
            break;
        }
        if (rpg_bat_unit1.cnt < 545) {
            break;
        }
        rpg_bat_anim_set(40, 0, 1, 1);
        rpg_bat_se_set(1287, 1, 4);
        rpg_bat_seq_step_next();
        break;
    case 24:
        class_w[155].disp_flag = 129;
        rpg_bat_unit1.cnt += 6;
        rpg_bat_sprite_pos_set(59, -rpg_bat_unit1.cnt, 0);
        if (rpg_bat_unit1.cnt < 137) {
            break;
        }
        rpg_bat_seq_step_next();
        break;
    case 25:
        strcpy(rpg_bat_msg_buf2, komusou_seq_str5);
        k_disp_switch(rpg_bat_unit1.disp, 0);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 120);
        rpg_bat_seq_step_next();
        break;
    case 26:
        rpg_bat_seq_step_gate(1);
        break;
    case 27:
        komusou_leave_talk();
        break;
    case 28:
        rpg_bat_unit1.cnt += 1;
        if ((rpg_bat_unit1.cnt & 1) == 0) {
            rpg_bat_anim_set(40, 0, 1, 1);
        } else {
            rpg_bat_anim_set(40, 0, 1, 0);
        }
        if (rpg_bat_unit1.cnt < 91) {
            break;
        }
        rpg_bat_unit3.hp ^= 0x10000000;
        rpg_bat_anim_set(48, 0, 1, 1);
        if (rpg_bat_girl != 14) {
            rpg_bat_anim_set(29, 0, 1, 1);
        }
        rpg_bat_seq_step_next();
        break;
    case 29:
        rpg_bat_seq_wait(30);
        break;
    case 30:
        strcpy(rpg_bat_msg_buf4, komusou_seq_str6);
        strcpy(rpg_bat_msg4_line1, komusou_seq_str7);
        strcpy(rpg_bat_msg4_line2, komusou_seq_str8);
        strcpy(rpg_bat_msg4_line3, komusou_seq_str9);
        rpg_bat_pick_list_set();
        rpg_bat_seq_step_next();
        break;
    case 31:
        rpg_bat_seq_wait(60);
        break;
    case 32:
        device_port_init(1, 514);
        rpg_bat_seq_step_next();
        break;
    case 33:
        rpg_bat_msg_set(rpg_bat_msg_tbl4, rpg_bat_seq_msg_line_cnt, 90);
        rpg_bat_anim_set(48, 9, 1, 1);
        if (rpg_bat_girl != 14) {
            rpg_bat_anim_set(29, 4, 1, 1);
        }
        rpg_bat_seq_step_next();
        break;
    case 34:
        rpg_bat_seq_step_gate(0);
        break;
    case 35:
        sub_step_up();
        break;
    default:
        rpg_bat_effect_clear();
        break;
    }
}
