/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:2:deny */
/* @PostProc: patch_insn_reg_field 0xa8:rt:t8:t9 0xb0:rs:t8:t9 0xb0:rt:t9:t0 0xb4:rt:t9:t0 0xd0:rt:t0:t1 0xd4:rs:t0:t1 0xd4:rt:t0:t1 0xdc:rt:t1:t2 0xe0:rs:t1:t2 0xe0:rt:t1:t2 0xe4:rt:t0:t1 0xec:rt:t2:t3 0xf0:rs:t2:t3 0xf0:rt:t2:t3 0xf4:rt:t1:t2 0x100:rt:t2:t3 */
/* shougatu_girl5_init - SHOUGATU overlay, Address: 0x8013A3F8, Size: 260 bytes (65 insns) */
#include "types.h"

extern void draw2d3d();
extern void class_switch();
extern void prim_init();
extern void class_clear_all();
extern void back_clear_switch();
extern void tpage_buf_clear();
extern void text_sys_init();
extern void shared_system_state_reset();
extern void bustup_ptr_init();
extern void hizuke_init();
extern void message_window_init();
extern void yazirushi_off();
extern void date_name_set();
extern void shougatu_ptab_init();
extern void sub_sub_step_up();

extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 screen_bri;
extern u8 fade_bri;
extern u8 bg_change_tbl2;
extern u8 girl5_flag;
extern u8 now_girl;
extern s32 shougatu_date_ss_serifu;
extern s32 shougatu_date_ss_serifu_kind;
extern s32 shougatu_date_sss_sound;
extern s32 shougatu_date_serifu_tbl[];
extern s32 shougatu_date_serifu_kind_tbl[];
extern s32 shougatu_date_sound_tbl[];

void shougatu_girl5_init(void)
{
    s32 x;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    girl5_flag = girl5_flag | 4;
    x = 5;
    now_girl = x;
    date_name_set(x);
    shougatu_ptab_init();
    shougatu_date_ss_serifu = shougatu_date_serifu_tbl[5];
    shougatu_date_ss_serifu_kind = shougatu_date_serifu_kind_tbl[5];
    shougatu_date_sss_sound = shougatu_date_sound_tbl[5];
    sub_sub_step_up();
}
