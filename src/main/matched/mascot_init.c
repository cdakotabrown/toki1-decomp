/* Function: mascot_init */
/* Address: 0x800677DC */
/* Size: 452 bytes (113 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 -Wo,-varref,1 */
#include "common.h"

extern s16 mascot_x;
extern s16 mascot_y;
extern s8 mascot_hi;
extern s8 mascot_cw;
extern s16 cursor_work_copy_tbl0;

extern s8 mascot_cw_prio;
extern s8 mascot_cw_mode;
extern u8 mascot_cw_disp;
extern s8 mascot_cw_flag4;
extern s8 mascot_cw_sel;
extern s32 mascot_cw_img;
extern s32 mascot_cw_anim_tbl;
extern s16 mascot_cw_aux14;
extern s16 mascot_cw_anim;
extern s16 mascot_cw_frame;
extern s32 mascot_cw_hdr;
extern s32 mascot_cw_attr;

extern s8 mascot2_cw_prio;
extern s8 mascot2_cw_mode;
extern u8 mascot2_cw_disp;
extern s8 mascot2_cw_flag4;
extern s8 mascot2_cw_sel;
extern s32 mascot2_cw_img;
extern s32 mascot2_cw_anim_tbl;
extern s16 mascot2_cw_aux14;
extern s16 mascot2_cw_anim;
extern s16 mascot2_cw_frame;
extern s32 mascot2_cw_hdr;
extern s32 mascot2_cw_attr;

extern s32 mascot_init_tbl0;
extern s32 mascot_init_tbl1;
extern s32 _sys_default_tpage_set_tbl0;
extern s16 mascot_init_tbl2;

extern s32 mascot_init_tbl0_w;
extern s32 mascot_init_tbl1_w;
extern s32 _sys_default_tpage_set_tbl0_w;
extern s16 mascot_init_tbl2_s;

extern void mascot_disp_switch(s32);
extern void class_clear_n(s32);

void mascot_init(s32 arg0) {
    if (arg0 != 0 && arg0 < 14) {
        goto setup;
    }
defaults:
    mascot_x = 120;
    mascot_y = -55;
    mascot_hi = 15;
    mascot_cw = 8;
    cursor_work_copy_tbl0 = 0;
    mascot_disp_switch(0);
    return;
setup:
    cursor_work_copy_tbl0 = arg0;
    class_clear_n(27);
    mascot_cw_prio = 1;
    mascot_cw_mode = 64;
    mascot_cw_attr = 0x40000000;
    mascot_cw_disp = 132;
    mascot_cw_img = (s32)&mascot_init_tbl0;
    mascot_cw_anim_tbl = (s32)&mascot_init_tbl1;
    mascot_cw_hdr = (s32)&_sys_default_tpage_set_tbl0;
    mascot_cw_aux14 = mascot_init_tbl2;
    mascot_cw_flag4 = 8;
    mascot_cw_sel = 1;
    mascot_cw_anim = 0;
    mascot_cw_frame = cursor_work_copy_tbl0 - 1;
    class_clear_n(28);
    mascot2_cw_prio = 1;
    mascot2_cw_mode = 64;
    mascot2_cw_attr = 0x40000000;
    mascot2_cw_disp = 132;
    mascot2_cw_img = (s32)&mascot_init_tbl0_w;
    mascot2_cw_anim_tbl = (s32)&mascot_init_tbl1_w;
    mascot2_cw_hdr = (s32)&_sys_default_tpage_set_tbl0_w;
    mascot2_cw_aux14 = mascot_init_tbl2_s;
    mascot2_cw_flag4 = 8;
    mascot2_cw_sel = 1;
    mascot2_cw_anim = 0;
    mascot2_cw_frame = 13;
}
