/* Address: 0x801481A8 (real entry: the 8-byte head `lbu a1, step_mode` */
/* Size: 1120 bytes (280 insns) */
/* @SymbolStart: 0x801481A8 */
/* @SymbolSize: 1120 */
/* @RodataBase: 0x8014CA38 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/class_w.h"

extern u8 step_mode;
extern u8 k_no;
extern u8 name_ent_serifu_no;
extern s32 name_ent_step_cnt;
extern u32 Bg;
extern s16 cw98_frame;
extern u8 cal_cw_prio;
extern u8 cal_cw_mode;
extern u8 cal_cw_disp;
extern u8 cal_cw_sel;
extern u32 cal_cw_attr;
extern u32 cal_cw_img;
extern u32 cal_cw_anim_tbl;
extern u32 cal_cw_hdr;
extern u16 cal_cw_frame;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern CLASS_W class_w[];

extern void name_ent_serifu_set();
extern void dec_bg_show();
extern void name_ent_fade_box_show();
extern s32 get_CD_State();
extern void name_ent_text_scroll();
extern void name_ent_prologue_serifu13_kuchi_ani();
extern s32 get_now_k();
extern void name_ent_pair_init();
extern void cd_read_start2();
extern s32 get_CD_State2();
extern void class_clear_f();
extern void memcpy();
extern void indexed_byte_toggle();
extern void name_ent_bg_set();
extern void sub_step_up();
extern s32 rand();
extern void name_ent_cw98_set9();

void name_ent_prologue_serifu13_body(void)
{
    s32 timer;

    switch (step_mode) {
    case 0:
        name_ent_serifu_no = 13;
        name_ent_serifu_set();
        name_ent_step_cnt = 0;
        step_mode++;
        dec_bg_show(1);
        break;
    case 1:
        timer = name_ent_step_cnt;
        if (timer < 255) {
            name_ent_step_cnt = timer + 1;
            name_ent_fade_box_show();
        }
        if (get_CD_State() & 4) {
            step_mode++;
        }
        dec_bg_show(1);
        break;
    case 2:
        timer = name_ent_step_cnt;
        if (timer < 255) {
            name_ent_step_cnt = timer + 1;
            name_ent_fade_box_show();
        } else {
            name_ent_fade_box_show();
        }
        name_ent_text_scroll();
        dec_bg_show(1);
        name_ent_prologue_serifu13_kuchi_ani();
        if (get_now_k() == k_no + 3) {
            step_mode++;
            Bg = 0x41000000;
            name_ent_pair_init();
            name_ent_fade_box_show();
        }
        break;
    case 3:
        if (name_ent_step_cnt >= 4) {
            name_ent_fade_box_show();
            name_ent_step_cnt = name_ent_step_cnt - 4;
        }
        name_ent_text_scroll();
        name_ent_prologue_serifu13_kuchi_ani();
        break;
    case 4:
        class_w[100].anim_flag = 5;
        class_w[99].anim_flag = 5;
        step_mode++;
        break;
    case 5:
        if (class_w[99].anim_flag == 0 && class_w[99].anim_flag == 0) {
            step_mode++;
            name_ent_step_cnt = 0;
        }
        break;
    case 6:
        if (name_ent_step_cnt++ >= 31) {
            step_mode++;
        }
        break;
    case 7:
        class_w[98].anim_flag = 1;
        cw98_frame = 3;
        class_w[98].tick = 0;
        name_ent_serifu_no = 14;
        name_ent_serifu_set();
        step_mode++;
        break;
    case 8:
        if (cw98_frame == 3) {
            class_w[98].anim_flag = 0;
        }
        name_ent_text_scroll();
        name_ent_prologue_serifu13_kuchi_ani();
        break;
    case 9:
        cd_read_start2(49, 0x801A4000, 0xAA85);
        step_mode++;
        break;
    case 10:
        if (get_CD_State2() & 1) {
            cd_read_start2(33, 0x801E8000, 0xAC1E);
            step_mode++;
        }
        break;
    case 11:
        if (get_CD_State2() & 1) {
            class_clear_f();
            memcpy(0x80180000, 0x801E8000, 0x10800);
            cal_cw_prio = 10;
            cal_cw_mode = 64;
            cal_cw_attr = 0x41000000;
            cal_cw_disp = 164;
            cal_cw_img = 0x80180150;
            cal_cw_anim_tbl = 0x80180174;
            cal_cw_hdr = 0x80180140;
            cal_cw_frame = 0;
            cal_cw_sel = 128;
            cal_cw_x = -160;
            cal_cw_y = -120;
            indexed_byte_toggle(1);
            name_ent_bg_set(0x801A4000, 15);
            sub_step_up();
        }
        break;
    }

    if (step_mode >= 6) {
        return;
    }
    if ((rand() & 0xff) != 255) {
        return;
    }
    if (class_w[98].anim_flag & 1) {
        return;
    }
    name_ent_cw98_set9();
}
