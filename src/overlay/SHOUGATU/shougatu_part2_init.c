/* @SymbolSize: 288 */
/* @PostProc: patch_insn_reg_field 0xa4:rt:t8:a0 0xb0:rt:t8:a0 0xbc:rt:t9:t8 0xc0:rs:t9:t8 0xc0:rt:t9:t8 0xc8:rt:t0:t9 0xcc:rs:t0:t9 0xcc:rt:t0:t9 0xd0:rt:t9:t8 0xd8:rt:t1:t0 0xdc:rs:t1:t0 0xdc:rt:t1:t0 0xe0:rt:t2:t1 0xe4:rt:t0:t9 0xe8:rs:t2:t1 0xe8:rt:t2:t1 0xf0:rt:t1:t0 0xf8:rd:t3:t2 0xf8:rt:t2:t1 0xfc:rs:t3:t2 0x100:rt:t4:t3 0x10c:rt:t4:t3 */
/* Address: 0x801385C0 */
/* Size: 288 bytes (72 instructions) */
#include "types.h"

extern void prim_init();
extern void sub_sub_step_up();
extern void draw2d3d();
extern void back_clear_switch();
extern void tpage_buf_clear();
extern void class_switch();
extern void class_clear_all();
extern void text_sys_init();
extern void hizuke_init();
extern void message_window_init();
extern void bustup_ptr_init();
extern void date_name_set(void);
extern void yazirushi_off();
extern void shared_system_state_reset();
extern void shougatu_ptab_init();

extern u8  vblank_flag;
extern s32 vb_ready;
extern u8  screen_bri;
extern u8  fade_bri;
extern u8  bg_change_tbl2;
extern u8  now_girl;
extern u8  girl2_basyo;
extern s32 shougatu_date_ss_serifu;
extern s32 shougatu_date_ss_serifu_kind;
extern s32 shougatu_date_sss_sound;
extern u8  shougatu_girl2_basyo3;
extern s32 shougatu_date_serifu_tbl[];
extern s32 shougatu_date_serifu_kind_tbl[];
extern s32 shougatu_date_sound_tbl[];

void shougatu_part2_init(void)
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
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    now_girl = 2;
    date_name_set();
    shougatu_ptab_init();
    shougatu_date_ss_serifu = shougatu_date_serifu_tbl[1];
    shougatu_date_ss_serifu_kind = shougatu_date_serifu_kind_tbl[1];
    shougatu_date_sss_sound = shougatu_date_sound_tbl[1];
    {
        u32 t;
        t = girl2_basyo;
        if ((t >> 4) == 3) {
            shougatu_girl2_basyo3 = 1;
        } else {
            shougatu_girl2_basyo3 = 0;
        }
    }
    sub_sub_step_up();
}
