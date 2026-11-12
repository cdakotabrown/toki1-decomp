/* geko_screen_reset -- GEKO overlay, Address: 0x80135CB4, Size: 192 bytes (48 insns), Byte-perfect */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void k_reset(s32 a0);
extern void shared_system_state_reset(void);
extern void yazirushi_off(void);
extern void bustup_ptr_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void sub_sub_step_up(void);

extern s8 bg_change_tbl2;
extern u8 screen_bri;
extern s8 vblank_flag;
extern s32 vb_ready;
extern s8 fade_bri;
extern s8 geko_dir_scroll_flag;

void geko_screen_reset(void)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    k_reset(1);
    shared_system_state_reset();
    yazirushi_off();
    vblank_flag = 0;
    vb_ready = 1;
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    geko_dir_scroll_flag = 1;
    sub_sub_step_up();
}
