/* @SymbolStart: 0x801454F0 */
/* @RodataBase: 0x8015C590 */
/* @SymbolSize: 1996 */
/* @PostProc: patch_insn_reg_field 0x174:rt:v0:v1 0x178:rs:v0:v1 0x178:rt:v0:v1 0x180:rs:v0:v1 0x180:rt:v0:v1 0x184:rt:v0:v1 0x188:rs:v0:v1 0x498:rt:v0:v1 0x49c:rs:v0:v1 0x49c:rt:v0:v1 0x4a4:rs:v0:v1 0x4a4:rt:v0:v1 0x4a8:rt:v0:v1 0x4ac:rs:v0:v1 */
/* Address: 0x801454F8 */
/* Size: 1988 bytes (497 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern void rpg_bat_pose_set(void);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_step_next(void);
extern void strcpy(char *, char *);
extern void rpg_bat_msg_set(char *, s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_timer_step(s32);
extern s32 get_CD_State(void);
extern void rpg_bat_girl_move_out(void);
extern void rpg_bat_girl_move_in(void);

extern s32 rpg_bat_girl_home_x;
extern s32 rpg_bat_girl_home_y;
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_msg_time;

extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_buf_1[];
extern char rpg_bat_msg_tbl[];
extern char rpg_bat_battle_step_str0[];
extern char rpg_bat_battle_step_str1[];
extern char rpg_bat_battle_step_str2[];
extern char rpg_bat_battle_step_str3[];
extern CLASS_W class_w[];
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_battle_step(void)
{
    s32 v0;

    if (rpg_bat_unit2.flag == 0x40000000) {
        switch ((u32)rpg_bat_unit0.step) {
        case 6:
            rpg_bat_pose_set();
            rpg_bat_sprite_pos_set(29, rpg_bat_girl_home_x + 96, rpg_bat_girl_home_y);
            rpg_bat_sprite_pos_set(50, rpg_bat_tgt_x + rpg_bat_girl_home_x + 96,
                          rpg_bat_tgt_y + rpg_bat_girl_home_y);
            rpg_bat_anim_set2(29, 0, 1, 1);
            rpg_bat_anim_set2(50, 7, 5, 1);
            class_w[146].frame = 1;
            class_w[125].prio = 4;
            class_w[146].prio = 3;
            rpg_bat_step_next();
            break;
        case 7:
            rpg_bat_sprite_pos_set(29, rpg_bat_girl_home_x - rpg_bat_unit0.wait + 96, rpg_bat_girl_home_y);
            rpg_bat_sprite_pos_set(50, rpg_bat_tgt_x + rpg_bat_girl_home_x - rpg_bat_unit0.wait + 96,
                          rpg_bat_tgt_y + rpg_bat_girl_home_y);
            rpg_bat_unit0.wait += 2;
            if (rpg_bat_unit0.wait < 97) {
                break;
            }
            rpg_bat_step_next();
            break;
        case 8:
            strcpy(rpg_bat_msg_buf, rpg_bat_battle_step_str0);
            strcpy(rpg_bat_msg_buf_1, rpg_bat_battle_step_str1);
            rpg_bat_msg_set(rpg_bat_msg_tbl, 2, 10);
            rpg_bat_koe_set(134, 0x1300008B, 0);
            rpg_bat_msg_ctl_mode = 3;
            rpg_bat_anim_set2(29, 8, 1, 1);
            rpg_bat_step_next();
            break;
        case 9:
            rpg_bat_timer_step(20);
            break;
        case 10:
            rpg_bat_anim_set2(50, 13, 1, 1);
            rpg_bat_step_next();
            break;
        case 11:
            rpg_bat_unit0.cnt += 1;
            if (rpg_bat_unit0.cnt >= 301) {
                rpg_bat_anim_set2(50, 13, 1, 0);
                rpg_bat_msg_ctl_mode = 1;
                rpg_bat_step_next();
            }
            v0 = get_CD_State();
            if (v0 == 4) {
                rpg_bat_seq_work[49] = 1;
            }
            if (rpg_bat_seq_work[49] != 1) {
                break;
            }
            v0 = get_CD_State();
            if (v0 != 2) {
                break;
            }
            rpg_bat_anim_set2(50, 13, 1, 0);
            rpg_bat_msg_ctl_mode = 1;
            rpg_bat_seq_work[49] = 0;
            rpg_bat_step_next();
            break;
        case 12:
            rpg_bat_koe_set(140, 0x14000099, 1);
            rpg_bat_msg_ctl_mode = 3;
            rpg_bat_step_next();
            break;
        case 13:
            rpg_bat_timer_step(5);
            break;
        case 14:
            rpg_bat_unit0.cnt += 1;
            if (rpg_bat_unit0.cnt >= 301) {
                rpg_bat_msg_ctl_mode = 1;
                rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
                rpg_bat_anim_set2(29, 0, 1, 1);
                rpg_bat_step_next();
            }
            v0 = get_CD_State();
            if (v0 == 4) {
                rpg_bat_seq_work[49] = 1;
            }
            if (rpg_bat_seq_work[49] != 1) {
                break;
            }
            v0 = get_CD_State();
            if (v0 != 2) {
                break;
            }
            rpg_bat_msg_ctl_mode = 1;
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
            rpg_bat_anim_set2(29, 0, 1, 1);
            rpg_bat_seq_work[49] = 0;
            rpg_bat_step_next();
            break;
        case 15:
            rpg_bat_timer_step(30);
            break;
        case 16:
            rpg_bat_sprite_pos_set(29, rpg_bat_girl_home_x + rpg_bat_unit0.wait, rpg_bat_girl_home_y);
            rpg_bat_sprite_pos_set(50, rpg_bat_tgt_x + rpg_bat_girl_home_x + rpg_bat_unit0.wait,
                          rpg_bat_tgt_y + rpg_bat_girl_home_y);
            rpg_bat_unit0.wait += 2;
            if (rpg_bat_unit0.wait < 97) {
                break;
            }
            rpg_bat_step_next();
            break;
        case 17:
            rpg_bat_unit0.step = 100;
            break;
        }
    } else {
        switch ((u32)rpg_bat_unit0.step) {
        case 6:
            rpg_bat_girl_move_out();
            break;
        case 7:
            strcpy(rpg_bat_msg_buf, rpg_bat_battle_step_str2);
            strcpy(rpg_bat_msg_buf_1, rpg_bat_battle_step_str3);
            rpg_bat_msg_set(rpg_bat_msg_tbl, 2, 10);
            rpg_bat_koe_set(134, 0x1300008B, 0);
            rpg_bat_msg_ctl_mode = 3;
            rpg_bat_anim_set2(29, 8, 1, 1);
            rpg_bat_step_next();
            break;
        case 8:
            rpg_bat_timer_step(20);
            break;
        case 9:
            rpg_bat_anim_set2(50, 13, 1, 1);
            rpg_bat_step_next();
            break;
        case 10:
            rpg_bat_unit0.cnt += 1;
            if (rpg_bat_unit0.cnt >= 301) {
                rpg_bat_anim_set2(50, 13, 1, 0);
                rpg_bat_msg_ctl_mode = 1;
                rpg_bat_step_next();
            }
            v0 = get_CD_State();
            if (v0 == 4) {
                rpg_bat_seq_work[49] = 1;
            }
            if (rpg_bat_seq_work[49] != 1) {
                break;
            }
            v0 = get_CD_State();
            if (v0 != 2) {
                break;
            }
            rpg_bat_anim_set2(50, 13, 1, 0);
            rpg_bat_msg_ctl_mode = 1;
            rpg_bat_seq_work[49] = 0;
            rpg_bat_step_next();
            break;
        case 11:
            rpg_bat_koe_set(140, 0x14000099, 1);
            rpg_bat_msg_ctl_mode = 3;
            rpg_bat_step_next();
            break;
        case 12:
            rpg_bat_timer_step(5);
            break;
        case 13:
            rpg_bat_unit0.cnt += 1;
            if (rpg_bat_unit0.cnt >= 301) {
                rpg_bat_msg_ctl_mode = 1;
                rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
                rpg_bat_anim_set2(29, 0, 1, 1);
                rpg_bat_step_next();
            }
            v0 = get_CD_State();
            if (v0 == 4) {
                rpg_bat_seq_work[49] = 1;
            }
            if (rpg_bat_seq_work[49] != 1) {
                break;
            }
            v0 = get_CD_State();
            if (v0 != 2) {
                break;
            }
            rpg_bat_msg_ctl_mode = 1;
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
            rpg_bat_anim_set2(29, 0, 1, 1);
            rpg_bat_seq_work[49] = 0;
            rpg_bat_step_next();
            break;
        case 14:
            rpg_bat_timer_step(30);
            break;
        case 15:
            rpg_bat_girl_move_in();
            break;
        case 16:
            rpg_bat_unit0.step = 100;
            break;
        }
    }
}
