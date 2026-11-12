/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x11C */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x11C --offset2 0x124 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x124 */
/* geko_scene_init -- GEKO overlay, Address: 0x8013D6D4, Size: 328 bytes (82 insns) */
#include "types.h"

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
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern s8 now_event_no;
extern s32 ba_string;
extern s16 girl5_yuukou;
extern s16 girl5_syoushin;
extern u8 now_girl;
extern s32 work_tarao;
extern s8 fade_bri;
extern s32 geko_scene_init_str0;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_scene_init(void)
{
    now_girl = 5;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
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
    geko_ss_serifuev = geko_serifuev_tbl[42];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[42];
    geko_sss_soundev = geko_soundev_tbl[42];
    strcpy(&ba_string, &geko_scene_init_str0);
    if (work_tarao == 1) {
        girl5_yuukou = (girl5_yuukou + 1);
        girl5_syoushin = (girl5_syoushin - 1);
        param_clamp();
    }
    now_event_no = 33;
    sub_sub_step_up();
}
