/* shougatu_girl4_init - SHOUGATU overlay, Address: 0x8013AC5C, Size: 288 bytes (72 insns) */
/* @SymbolSize: 288 */
/* @PostProc: patch_insn_reg_field 0xa4:rt:t8:a0 0xb0:rt:t8:a0 0xbc:rt:t9:t8 0xc0:rs:t9:t8 0xc0:rt:t9:t8 0xc4:rt:t2:t1 0xc8:rs:t2:t1 0xc8:rt:t2:t1 0xd0:rt:t0:t9 0xd4:rs:t0:t9 0xd4:rt:t0:t9 0xd8:rt:t9:t8 0xe0:rt:t1:t0 0xe4:rd:t3:t2 0xe4:rt:t2:t1 0xe8:rs:t1:t0 0xe8:rt:t1:t0 0xec:rt:t4:t3 0xf0:rd:t3:t2 0xf0:rs:t3:t2 0xf0:rt:t2:t1 0xf4:rt:t0:t9 0xfc:rd:t3:t2 0xfc:rt:t3:t2 0x100:rs:t4:t3 0x100:rt:t4:t3 0x104:rd:v1:v0 0x104:rs:t3:t2 0x104:rt:t4:t3 0x108:rt:t1:t0 0x10c:rs:v1:v0 0x10c:rt:v0:t5 0x114:rs:v0:t5 0x11c:rs:v1:v0 */
#include "types.h"

extern u8  vblank_flag;
extern s32 vb_ready;
extern u8  screen_bri;
extern u8  fade_bri;
extern u8  bg_change_tbl2;
extern u8  now_girl;
extern s32 shougatu_date_serifu_tbl_7;
extern s32 shougatu_date_serifu_kind_tbl_7;
extern s32 shougatu_date_sound_tbl_7;
extern s32 shougatu_date_ss_serifu;
extern s32 shougatu_date_ss_serifu_kind;
extern s32 shougatu_date_sss_sound;
extern u8  screen_w[][56];

void prim_init(void);
void sub_sub_step_up(void);
void draw2d3d(s32 arg0, s32 arg1);
void back_clear_switch(s32 arg0);
void tpage_buf_clear(void);
void class_switch(s32 arg0);
void class_clear_all(s32 arg0);
void date_name_set(void);
void text_sys_init(void);
void yazirushi_off(void);
void shared_system_state_reset(void);
void bustup_ptr_init(void);
void hizuke_init(void);
void message_window_init(void);
void shougatu_ptab_init(void);

void shougatu_girl4_init(void)
{
    u8 *rec;
    u8 idx;
    u8 v;
    s32 a;
    s32 b;
    s32 c;

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
    now_girl = 4;
    date_name_set();
    shougatu_ptab_init();

    a = shougatu_date_serifu_tbl_7;
    idx = now_girl;
    b = shougatu_date_serifu_kind_tbl_7;
    shougatu_date_ss_serifu = a;
    c = shougatu_date_sound_tbl_7;
    shougatu_date_ss_serifu_kind = b;
    rec = screen_w[idx];
    shougatu_date_sss_sound = c;
    v = rec[456];
    v = v | 4;
    rec[456] = v;
    sub_sub_step_up();
}
