/* @SymbolStart: 0x8014AA4C */
/* @RodataBase: 0x8014CBBC */
/* Address: 0x8014AA54 */
/* Size: 504 bytes (126 insns) */
/* @SymbolSize: 512 */
/* @PostProc: patch_insn_reg_field 0x4c:rt:v1:v0 0x50:rs:v1:v0 0x50:rt:v1:v0 0x58:rs:v1:v0 0x58:rt:v1:v0 0x5c:rt:v1:v0 0x60:rs:v1:v0 0x134:rt:v1:v0 0x138:rs:v1:v0 0x138:rt:v1:v0 0x140:rs:v1:v0 0x140:rt:v1:v0 0x144:rt:v1:v0 0x148:rs:v1:v0 0x178:rt:v1:v0 0x17c:rs:v1:v0 0x17c:rt:v1:v0 0x184:rs:v1:v0 0x184:rt:v1:v0 0x188:rt:v1:v0 0x18c:rs:v1:v0 0x1bc:rt:v1:v0 0x1c0:rs:v1:v0 0x1c0:rt:v1:v0 0x1c8:rs:v1:v0 0x1c8:rt:v1:v0 0x1cc:rt:v1:v0 0x1d0:rs:v1:v0 */
#include "types.h"

extern void name_ent_fade_box_show(void);
extern void class_clear_n(s32);
extern void name_ent_white_box_show(void);
extern void sub_step_up(void);
extern void dec_bg_show(s32);

extern s32 name_ent_step_cnt;
extern u8  step_mode;
extern u8  cal_cw_prio;
extern u8  cal_cw_mode;
extern u8  cal_cw_disp;
extern u8  cal_cw_sel;
extern u8  cal_cw_rate;
extern s16 cal_cw_anim;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8 step_mode;

void name_ent_prologue_flash_body(void) {
    s32 n;

    switch (step_mode) {
    case 0:
        name_ent_fade_box_show();
        n = name_ent_step_cnt + 4;
        name_ent_step_cnt = n;
        if (n >= 256) {
            class_clear_n(96);
            cal_cw_prio = 12;
            cal_cw_mode = 5;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 164;
            cal_cw_img = 0x80180204;
            cal_cw_anim_tbl = 0x80180220;
            cal_cw_hdr = 0x801801E4;
            cal_cw_anim = 0;
            cal_cw_rate = 2;
            cal_cw_sel = 128;
            cal_cw_x = -160;
            cal_cw_y = -120;
            step_mode++;
        }
        break;
    case 1:
        name_ent_white_box_show();
        name_ent_fade_box_show();
        n = name_ent_step_cnt - 4;
        name_ent_step_cnt = n;
        if (n < 3) {
            step_mode++;
        }
        break;
    case 2:
        name_ent_white_box_show();
        name_ent_fade_box_show();
        n = name_ent_step_cnt + 4;
        name_ent_step_cnt = n;
        if (n >= 256) {
            step_mode++;
            cal_cw_disp = 0;
        }
        break;
    case 3:
        name_ent_fade_box_show();
        n = name_ent_step_cnt - 4;
        name_ent_step_cnt = n;
        if (n < 3) {
            step_mode++;
        }
        break;
    case 4:
        sub_step_up();
        break;
    }
    dec_bg_show(1);
}
