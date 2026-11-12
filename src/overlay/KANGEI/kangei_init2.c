/* kangei_init2 - KANGEI overlay, Address: 0x80134F10, Size: 260 bytes (65 insns) */
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
extern void date_name_set(s32);
extern void kangei_ptr_tbl2_init(void);
extern void sub_sub_step_up(void);

extern u8  vblank_flag;
extern s32 vb_ready;
extern u8  fade_bri;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;
extern u8  girl_no;
extern s32 kangei_msg_text_intro;
extern s32 kangei_msg_kind_intro;
extern s32 kangei_msg_snd_intro;
extern s32 kangei_ss_s;
extern s32 kangei_ss_s_kind;
extern s32 kangei_sss_so;
extern u8  kangei_icon_flag;

void
kangei_init2(void)
{
    u8 sa;
    s32 v1, v2, v3;

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
    icon_disp_switch(0);
    kangei_now_s = 0;
    kangei_now_s_in = 0;
    bustup_ptr_init();
    yazirushi_off();
    sa = girl_no;
    date_name_set(sa);
    kangei_ptr_tbl2_init();
    v1 = kangei_msg_text_intro;
    v2 = kangei_msg_kind_intro;
    v3 = kangei_msg_snd_intro;
    kangei_ss_s = v1;
    kangei_ss_s_kind = v2;
    kangei_sss_so = v3;
    kangei_icon_flag = 0;
    sub_sub_step_up();
}
