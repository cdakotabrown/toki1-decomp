/* @PostProc: patch_insn_reg_field 0x114:rt:t0:t1 */
/* @PostProc: patch_insn_reg_field 0x118:rs:t0:t1 */
/* @PostProc: patch_insn_reg_field 0x120:rt:t3:t4 */
/* @PostProc: patch_insn_reg_field 0x124:rt:t1:t2 */
/* @PostProc: patch_insn_reg_field 0x128:rs:t1:t2 */
/* @PostProc: patch_insn_reg_field 0x128:rt:t1:t2 */
/* @PostProc: patch_insn_reg_field 0x130:rs:t1:t2 */
/* @PostProc: patch_insn_reg_field 0x130:rt:t2:t3 */
/* @PostProc: patch_insn_reg_field 0x134:rt:t2:t3 */
/* @PostProc: patch_insn_reg_field 0x138:rs:t3:t4 */
/* @PostProc: patch_insn_reg_field 0x138:rt:t3:t4 */
/* @PostProc: patch_insn_reg_field 0x140:rs:t3:t4 */
/* @PostProc: patch_insn_reg_field 0x140:rt:t4:t5 */
/* @PostProc: patch_insn_reg_field 0x148:rt:t4:t5 */
/* @PostProc: patch_insn_reg_field 0x14c:rt:t5:t6 */
/* @PostProc: patch_insn_reg_field 0x158:rt:t5:t6 */
/* geko_scene68_init -- GEKO overlay, Address: 0x80140308, Size: 348 bytes */
/* geko_scene68_init -- GEKO overlay, Address: 0x80140308, Size: 348 bytes */
#include "types.h"
#include "tokimeki/class_w.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern s32 ba_string[];
extern s32 ba_string2[];
extern void draw2d3d(s32 a0, s32 a1);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void class_switch(s32 a0);
extern void class_clear_all(s32 a0);
extern void text_sys_init(void);
extern s32 get_g_zyotai_s(s32 a0);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void date_name_set(s32 a0);
extern void param_clamp(void);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void strcpy(s32 *a0, s32 *a1);
extern void geko_ptr_tbl_init(void);

extern s16 now_serifuev;
extern s16 now_serifuev_in;
extern u8 now_event_no;
extern s16 girl10_tokimeki;
extern s16 girl10_yuukou;
extern u8 now_girl;
extern u8 cw3_disp;
extern u8 fade_bri;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern s32 geko_scene68_init_str0;
extern CLASS_W class_w[];
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_scene68_init(void)
{
    s32 result;

    now_girl = 10;
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(ba_string, &geko_scene68_init_str0);
    cw3_disp = 0;
    class_w[4].disp_flag = 0;
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
    geko_ss_serifuev = geko_serifuev_tbl[68];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[68];
    geko_sss_soundev = geko_soundev_tbl[68];
    result = get_g_zyotai_s(10);
    if ((u32)(result & 0x7F) < 2) {
        girl10_tokimeki++;
    }
    girl10_yuukou += 2;
    param_clamp();
    now_event_no = 68;
    sub_sub_step_up();
}
