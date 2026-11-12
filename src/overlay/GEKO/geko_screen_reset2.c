/* geko_screen_reset2 -- GEKO overlay, Address: 0x8013AC30, Size: 136 bytes (34 insns), Byte-perfect */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void sub_sub_step_up(void);

extern s8 now_event_no;
extern s8 fade_bri;

void geko_screen_reset2(void)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    shared_system_state_reset();
    hizuke_init();
    message_window_init();
    fade_bri = 0;
    yazirushi_off();
    now_event_no = 7;
    sub_sub_step_up();
}
