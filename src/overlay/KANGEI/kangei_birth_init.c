/* kangei_birth_init - KANGEI overlay, Address: 0x80138C20, Size: 184 bytes (46 insns) */
#include "types.h"

extern void draw2d3d(s32, s32);
extern void class_switch(s32);
extern void prim_init(void);
extern void back_clear_switch(s32);
extern void class_clear_all(s32);
extern void message_window_init(void);
extern void hizuke_init(void);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void sub_sub_step_up(void);

extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 fade_bri;
extern u8 kangei_icon_flag;

void
kangei_birth_init(void)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    back_clear_switch(1);
    class_clear_all(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    text_sys_init();
    vblank_flag = 0;
    vb_ready = 1;
    fade_bri = 0;
    shared_system_state_reset();
    parameter_disp_switch(0);
    parameter_disp_switch(0);
    icon_disp_switch(0);
    bustup_ptr_init();
    yazirushi_off();
    kangei_icon_flag = 0;
    sub_sub_step_up();
}
