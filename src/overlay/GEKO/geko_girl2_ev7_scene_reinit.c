/* @CompileEnv: IDO52_CONST_INREG_VALUES=2 */
/* @PostProc: patch_stack_complete --old 40 --new 56 */
/* @PostProc: remap_stack_slots --from 32 --to 44 --from 34 --to 46 --from 36 --to 48 --from 38 --to 50 */
/* geko_girl2_ev7_scene_reinit -- GEKO overlay, Address: 0x8013B310, Size: 396 bytes (99 insns) */
#include "types.h"

extern void draw2d3d(s32 a0, s32 a1);
extern void class_switch(s32 a0);
extern void prim_init(void);
extern void class_clear_all(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);
extern void back_clear_switch(s32 a0);
extern void tpage_buf_clear(void);
extern void text_sys_init(void);
extern void shared_system_state_reset(void);
extern void k_reset(s32 a0);
extern void ClearImage(void *a0, s32 a1, s32 a2, s32 a3);
extern void date_name_set(u8 a0);
extern void bustup_ptr_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void geko_ptr_tbl_init(void);
extern void sub_sub_step_up(void);

extern volatile u8 now_girl;
extern u8 screen_bri;
extern u8 vblank_flag;
extern s32 vb_ready;
extern s8 fade_bri;
extern s32 geko_ss_serifuev;
extern s32 geko_ss_serifuev_kind;
extern s32 geko_sss_soundev;
extern s32 geko_serifuev_tbl[];
extern s32 geko_serifuev_kind_tbl[];
extern s32 geko_soundev_tbl[];

void geko_girl2_ev7_scene_reinit(void)
{
    s16 buf[4];
    u8 two;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    set_flag_b3_high_bit(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    shared_system_state_reset();
    k_reset(1);

    buf[0] = 320;
    buf[1] = 256;
    buf[2] = 320;
    buf[3] = 248;
    ClearImage(buf, 8, 0, 0);

    buf[0] = 704;
    buf[1] = 0;
    ClearImage(buf, 8, 0, 0);

    vblank_flag = 0;
    vb_ready = 1;
    two = 2;
    now_girl = two;
    date_name_set(two);
    bustup_ptr_init();
    screen_bri = 128;
    fade_bri = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    geko_ptr_tbl_init();
    geko_ss_serifuev = geko_serifuev_tbl[20];
    geko_ss_serifuev_kind = geko_serifuev_kind_tbl[20];
    geko_sss_soundev = geko_soundev_tbl[20];
    sub_sub_step_up();
}
