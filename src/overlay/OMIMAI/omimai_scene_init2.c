/* @SymbolSize: 188 */
/* omimai_scene_init2 - OMIMAI overlay, Address: 0x80133730, Size: 188 bytes (47 insns) */
#include "types.h"

extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;
extern u8 bg_change_tbl2;

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void class_clear_all(s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void k_reset(s32);
extern void shared_system_state_reset(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void omimai_girl_serifu_init(void);
extern void sub_sub_step_up(void);

void omimai_scene_init2(void)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    k_reset(1);
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    hizuke_init();
    message_window_init();
    bustup_ptr_init();
    yazirushi_off();
    omimai_girl_serifu_init();
    sub_sub_step_up();
}
