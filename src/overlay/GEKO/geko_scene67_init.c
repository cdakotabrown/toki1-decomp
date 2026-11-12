/* @PostProc: swap_instructions --offset1 0x130 --offset2 0x138 */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x13C */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x13C --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x14C */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x14C --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x15C --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x168 */
/* geko_scene67_init -- GEKO overlay, Address: 0x801400D4, Size: 396 bytes (99 insns) */
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
extern s16 para_tai;
extern s16 para_kon;
extern s16 girl10_tokimeki;
extern s16 girl10_yuukou;
extern u8 now_girl;
extern s8 cw3_disp;
extern s32 work_tarao;
extern s8 fade_bri;
extern s32 geko_scene67_basyo_str;
extern s32 geko_scene67_init_str0;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_scene67_init(void)
{
    now_girl = 10;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    basyo_init(&geko_scene67_basyo_str);
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
    geko_ss_serifuev = geko_serifuev_tbl[67];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[67];
    geko_sss_soundev = geko_soundev_tbl[67];
    strcpy(&ba_string, &geko_scene67_init_str0);
    if (work_tarao == 1) {
        girl10_tokimeki = (girl10_tokimeki + 1);
        girl10_yuukou = (girl10_yuukou + 2);
        para_tai = (para_tai - 5);
        para_kon = (para_kon + 5);
        param_clamp();
    }
    now_event_no = 67;
    sub_sub_step_up();
}
