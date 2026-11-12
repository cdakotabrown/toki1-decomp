/* @PostProc: swap_instructions --offset1 0x10C --offset2 0x114 */
/* @PostProc: swap_instructions --offset1 0x110 --offset2 0x118 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x11C */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x11C */
/* geko_girl1_ev1_scene_init -- GEKO overlay, Address: 0x8013D1C8, Size: 320 bytes (80 insns) */
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
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32 a0);
extern void shared_system_state_reset(void);
extern void geko_ptr_tbl_init(void);
extern void strcpy(s32 *a0, s32 *a1);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern s32 ba_string;
extern s16 girl1_tokimeki;
extern s16 girl1_yuukou;
extern u8 now_girl;
extern s8 cw3_disp;
extern s32 geko_scene08_basyo_str;
extern s32 geko_scene_init08_str0;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl1_ev1_scene_init(void)
{
    now_girl = 1;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    basyo_init(&geko_scene08_basyo_str);
    cw3_disp = 0;
    (*(s8 *)&class_w[4].disp_flag) = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    date_name_set(1);
    shared_system_state_reset();
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[37];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[37];
    geko_sss_soundev = geko_soundev_tbl[37];
    strcpy(&ba_string, &geko_scene_init08_str0);
    girl1_tokimeki = (girl1_tokimeki + 1);
    girl1_yuukou = (girl1_yuukou + 2);
    param_clamp();
    now_event_no = 8;
    sub_sub_step_up();
}
