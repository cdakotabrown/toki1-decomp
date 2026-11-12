/* @PostProc: scheduler_normalizer --const-fp --store-fp --branch-fp */
/* geko_girl4_27_init -- GEKO overlay, Address: 0x8013F018, Size: 416 bytes (104 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void basyo_init(void *a0);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32 a0);
extern void shared_system_state_reset(void);
extern void strcpy(void *a0, void *a1);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern s16 para_tai;
extern s16 girl4_tokimeki;
extern s16 girl4_yuukou;
extern u8 ev_flag1b;
extern u8 ev_flag1c;
extern u8 now_girl;
extern s8 cw3_disp;
extern s8 fade_bri;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;

extern u8 ba_string[];
extern u8 geko_girl4_basyo_str[];
extern u8 geko_girl4_27_init_str0[];
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl4_27_init(void)
{
    s32 state;

    now_girl = 4;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    basyo_init(&geko_girl4_basyo_str);
    cw3_disp = 0;
    (*(s8 *)&class_w[4].disp_flag) = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    geko_ptr_tbl_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    fade_bri = 0;
    date_name_set(now_girl);
    shared_system_state_reset();
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[61];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[61];
    geko_sss_soundev = geko_soundev_tbl[61];
    strcpy(&ba_string, &geko_girl4_27_init_str0);

    state = ev_flag1b;
    if ((u32)state >= 3U) {
        ev_flag1b = 2;
    }
    if (ev_flag1c == 0 && ev_flag1b == 1) {
        girl4_tokimeki = girl4_tokimeki + 1;
    }
    girl4_yuukou = girl4_yuukou + 1;
    para_tai = para_tai + 10;
    param_clamp();
    now_event_no = 27;
    sub_sub_step_up();
}
