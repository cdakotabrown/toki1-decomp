/* @SymbolStart: 0x80157640 */
/* @SymbolSize: 540 */
/* @RodataBase: 0x8015DE24 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a3:v0 0x48:rs:a3:v0 0x48:rt:a3:v0 0x54:rt:a3:v0 0x68:rs:a3:v0 0x68:rt:a3:v0 0x6c:rt:a3:v0 0x94:rt:a3:v0 0x98:rs:a3:v0 0x98:rt:a3:v0 0xa0:rs:a3:v0 0x1c0:rt:a3:v0 0x1c4:rs:a3:v0 0x1c4:rt:a3:v0 0x1d8:rs:a3:v0 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_seq_wait(s32);
extern void strcpy(void *, void *);
extern void strcat(void *, void *);
extern void rpg_bat_msg_set(void *, s32, s32);
extern void rpg_bat_seq_step_gate(s32);
extern void rpg_bat_se_set(s32, s32, s32);
extern void sub_step_up(void);

extern s32 rpg_bat_bg_wave_phase;
extern s32 rpg_bat_result;
extern volatile s16 cw125_x;
extern volatile s16 cw144_x;
extern u8  rpg_bat_msg_buf2[];
extern u8  rpg_bat_msg_tbl2[];
extern u8  p_sei[];
extern u8  rpg_bat_escape_seq_str0[];
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void rpg_bat_escape_seq(void)
{
    s32 v0;

    switch (rpg_bat_unit1.step) {
    case 0:
        v0 = rpg_bat_unit1.cnt;
        rpg_bat_bg_wave_phase = 272 - v0;
        rpg_bat_unit2.flag = 0;
        rpg_bat_unit1.cnt = v0 + 4;
        rpg_bat_sprite_pos_set(29, 0, 0);
        rpg_bat_anim_set(29, 24, 1, 1);
        if (rpg_bat_unit1.cnt >= 209)
            rpg_bat_seq_step_next();
        break;
    case 1:
        rpg_bat_unit2.flag = 0;
        rpg_bat_sprite_pos_set(29, 0, 0);
        rpg_bat_anim_set(29, 24, 1, 1);
        rpg_bat_seq_step_next();
        break;
    case 2:
        rpg_bat_seq_wait(30);
        break;
    case 3:
        strcpy(rpg_bat_msg_buf2, p_sei);
        strcat(rpg_bat_msg_buf2, rpg_bat_escape_seq_str0);
        rpg_bat_msg_set(rpg_bat_msg_tbl2, 1, 90);
        rpg_bat_seq_step_next();
        break;
    case 4:
        rpg_bat_seq_step_gate(0);
        break;
    case 5:
        class_w[125].disp_flag = 129;
        rpg_bat_anim_set(48, 2, 1, 1);
        rpg_bat_se_set(1283, 1, 0);
        rpg_bat_seq_step_next();
        break;
    case 6:
        cw125_x += 4;
        cw144_x += 4;
        v0 = rpg_bat_unit1.cnt + 1;
        rpg_bat_unit1.cnt = v0;
        if (v0 >= 33)
            rpg_bat_seq_step_next();
        break;
    case 7:
        rpg_bat_seq_wait(60);
        break;
    case 8:
        rpg_bat_result = 1;
        sub_step_up();
        break;
    }
}
