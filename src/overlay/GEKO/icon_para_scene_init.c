/* @SymbolStart: 0x80134770 */
/* @SymbolSize: 360 */
/* @PostProc: patch_insn_reg_field 0x14:rt:v1:v0 0x1c:rs:v1:v0 0x3c:rs:v1:v0 0x3c:rt:t7:t8 0x44:rt:t7:t8 0xb0:rt:t8:t9 0xbc:rt:t8:t9 0xcc:rt:t9:t0 0xd0:rt:t9:t0 0xd8:rt:t0:t1 0xdc:rt:t0:t1 0x120:rd:t1:t2 0x124:rt:t1:t2 */
/* icon_para_scene_init -- GEKO overlay, Address: 0x80134778, Size: 352 bytes (88 insns) */
#include "types.h"

extern void day_schedule_return(void);
extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void strcpy(void *a0, void *a1);
extern void k_reset(s32 a0);
extern void shared_system_state_reset(void);
extern void bustup_ptr_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void icon_init(void);
extern void icon_can_use_set(s32 a0, s32 a1);
extern void icon_cursor_move(s32 a0);
extern void parameter_show_init(void);
extern void yazirushi_off(void);
extern void sub_sub_step_up(void);

extern s8 bg_change_tbl2;
extern u8 screen_bri;
extern u8 day_flag;
extern s8 vblank_flag;
extern s32 vb_ready;
extern s32 dec_bg_on;
extern s32 pick_girl;
extern s8 fade_bri;
extern s16 geko_now_serifu_icon;
extern s16 geko_now_serifu_icon_in;

extern u8 ba_string[];
extern s32 icon_para_scene_init_str0;

s32 icon_para_scene_init(s32 dead0, s32 dead1)
{
    s32 sel;
    s16 i;
    u8 saved_val;

    sel = day_flag;
    if (dead0) {} if (dead1) {}
    if ((sel & 4) != 0) {
        day_schedule_return();
        return 0;
    }

    day_flag = (u8)(sel | 4);
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(&ba_string, &icon_para_scene_init_str0);
    k_reset(1);
    vblank_flag = 0;
    vb_ready = 1;
    saved_val = pick_girl;
    shared_system_state_reset();
    pick_girl = saved_val;
    dec_bg_on = 1;
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    icon_init();

    i = 0;
    do {
        icon_can_use_set(i, 0);
        i = i + 1;
    } while (i < 15);

    icon_cursor_move(1);
    parameter_show_init();
    parameter_show_init();
    yazirushi_off();
    geko_now_serifu_icon = 0;
    geko_now_serifu_icon_in = 0;
    sub_sub_step_up();

}
