/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x11C --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* geko_girl2_ev1_scene_init -- GEKO overlay, Address: 0x8013B460, Size: 332 bytes (83 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void bustup_ptr_init(void);
extern void geko_ptr_tbl_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void date_name_set(s32 a0);
extern void shared_system_state_reset(void);
extern void strcpy(s32 *a0, s32 *a1);
extern s32 ba_string;
extern s32 geko_scene_init3_str0;
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern s16 girl2_tokimeki;
extern s16 girl2_yuukou;
extern u8 now_girl;
extern s8 cw3_disp;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern s8 geko_voice_latch_flag;
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl2_ev1_scene_init(void)
{
    now_girl = 2;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    cw3_disp = 0;
    (*(s8 *)&class_w[4].disp_flag) = 0;
    now_serifuev = 0;
    now_serifuev_in = 0;
    bustup_ptr_init();
    geko_ptr_tbl_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    date_name_set(now_girl);
    shared_system_state_reset();
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[14];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[14];
    geko_sss_soundev = geko_soundev_tbl[14];
    strcpy(&ba_string, &geko_scene_init3_str0);
    geko_voice_latch_flag = 0;
    girl2_tokimeki = (girl2_tokimeki + 1);
    girl2_yuukou = (girl2_yuukou + 2);
    param_clamp();
    now_event_no = 14;
    sub_sub_step_up();
}
