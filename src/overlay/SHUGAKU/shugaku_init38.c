/* @CompileEnv: IDO52_FORM_SITE=const:2:70:1,const:2:73:1 */
/* @SymbolSize: 380 */
/* 2. `@CompileEnv: IDO52_FORM_SITE=const:2:70:1,const:2:73:1` (Hook 30). */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 24 --to 44 */
/* @PostProc: remap_stack_slots --from 26 --to 46 */
/* @PostProc: remap_stack_slots --from 28 --to 48 */
/* @PostProc: remap_stack_slots --from 30 --to 50 */
/* shugaku_init38 - SHUGAKU overlay, Address: 0x801355E0, Size: 380 bytes (95 insns) */
#include "types.h"

extern u8  vblank_flag;
extern s32 vb_ready;
extern s8  now_girl;
extern u8  screen_bri;
extern u8  fade_bri;
extern u8  shugaku_float_on;
extern s16 shugaku_float_cnt;
extern u8  shugaku_rpg_arg;
extern s32 rpg_bat_arg;
extern s32 shugaku_ss_serifuev;
extern s32 shugaku_ss_serifuev_kind;
extern s32 shugaku_sss_soundev;
extern s32 shugaku_serifuev_tbl[];
extern s32 shugaku_serifuev_kind_tbl[];
extern s32 shugaku_soundev_tbl[];

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
extern void ClearImage();
extern void date_name_set(s32 a0);
extern void bustup_ptr_init(void);
extern void hizuke_init();
extern void message_window_init(void);
extern void yazirushi_off(void);
extern void shugaku_bank_ptr_init2(void);
extern void sub_sub_step_up(void);

void shugaku_init38(void)
{
    s16 rects[4];

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

    rects[0] = 320;
    rects[1] = 416;
    rects[2] = 320;
    rects[3] = 88;
    ClearImage((s32 *)rects, 8, 0, 0);

    rects[0] = 704;
    rects[1] = 160;
    ClearImage((s32 *)rects, 8, 0, 0);

    vblank_flag = 0;
    vb_ready = 1;
    now_girl = 2;
    date_name_set(2);
    bustup_ptr_init();
    screen_bri = 128;
    fade_bri = 0;
    hizuke_init();
    message_window_init();
    yazirushi_off();
    shugaku_float_on = 0;
    shugaku_float_cnt = 0;
    shugaku_rpg_arg = (u8)rpg_bat_arg;
    rpg_bat_arg = 255;
    shugaku_bank_ptr_init2();
    shugaku_ss_serifuev = shugaku_serifuev_tbl[19];
    shugaku_ss_serifuev_kind = shugaku_serifuev_kind_tbl[19];
    shugaku_sss_soundev = shugaku_soundev_tbl[19];
    sub_sub_step_up();
}
