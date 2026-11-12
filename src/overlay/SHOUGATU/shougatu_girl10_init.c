/* Address: 0x8013B0EC, Size: 296 bytes (74 instructions) */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=10 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: patch_insn_reg_field 0x6c:rt:t7:t8 0x74:rs:t7:t8 0x74:rt:t8:t9 0x7c:rt:t8:t9 0x80:rt:t9:t0 0x88:rt:t9:t0 0xf4:rt:t0:t1 0xf8:rs:t0:t1 0xf8:rt:t0:t1 0x100:rt:t1:t2 0x104:rs:t1:t2 0x104:rt:t1:t2 0x108:rt:t0:t1 0x110:rt:t2:t3 0x114:rs:t2:t3 0x114:rt:t2:t3 0x118:rt:t1:t2 0x124:rt:t2:t3 */
#include "types.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void class_switch(s32);
extern void class_clear_all(s32);
extern void text_sys_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void sprintf(u8 *, u8 *, u8 *);
extern void shougatu_ptab_init(void);

extern u8  bg_change_tbl2;
extern u8  p_name2[];
extern u8  screen_bri;
extern u8  p_sei[];
extern u8  girl10_flag;
extern s8  girl_no;
extern s8  now_girl;
extern u8  vblank_flag;
extern s32 vb_ready;
extern u8  fade_bri;
extern u8  shougatu_sei_san_fmt[];
extern s32 shougatu_date_ss_serifu;
extern s32 shougatu_date_ss_serifu_kind;
extern s32 shougatu_date_sss_sound;
extern s32 shougatu_date_serifu_tbl[];
extern s32 shougatu_date_serifu_kind_tbl[];
extern s32 shougatu_date_sound_tbl[];

void shougatu_girl10_init(void)
{
    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();

    vblank_flag = 0;
    vb_ready = 1;
    girl10_flag = girl10_flag | 8;
    shared_system_state_reset();

    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();

    hizuke_init();
    message_window_init();
    yazirushi_off();

    girl_no = 10;
    now_girl = 10;
    date_name_set(10);

    sprintf(p_name2, shougatu_sei_san_fmt, p_sei);
    shougatu_ptab_init();

    shougatu_date_ss_serifu = shougatu_date_serifu_tbl[8];
    shougatu_date_ss_serifu_kind = shougatu_date_serifu_kind_tbl[8];
    shougatu_date_sss_sound = shougatu_date_sound_tbl[8];
    sub_sub_step_up();
}
