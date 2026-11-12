/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x12C */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x130 */
/* @PostProc: swap_instructions --offset1 0x12C --offset2 0x130 */
/* geko_girl6_init -- GEKO overlay, Address: 0x8013DC58, Size: 340 bytes (85 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void basyo_init(s32 *a0);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32 a0);
extern void shared_system_state_reset(void);
extern void strcpy(s32 *a0, s32 *a1);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern s32 ba_string;
extern s16 girl6_tokimeki;
extern s16 girl6_yuukou;
extern u8 now_girl;
extern s8 cw3_disp;
extern s8 fade_bri;
extern s32 geko_girl6_basyo_str;
extern s32 geko_girl6_init_str0;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl6_init(void)
{
    now_girl = 6;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    basyo_init(&geko_girl6_basyo_str);
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
    geko_ss_serifuev = geko_serifuev_tbl[49];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[49];
    geko_sss_soundev = geko_soundev_tbl[49];
    strcpy(&ba_string, &geko_girl6_init_str0);
    girl6_tokimeki = (girl6_tokimeki + 1);
    girl6_yuukou = (girl6_yuukou + 2);
    param_clamp();
    now_event_no = 40;
    sub_sub_step_up();
}
