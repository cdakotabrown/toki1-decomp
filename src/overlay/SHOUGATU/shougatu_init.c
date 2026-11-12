/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
/* @PostProc: patch_insn_reg_field 0x10c:rt:a2:v1 0x114:rs:a2:v1 0x114:rt:a2:v1 0x128:rs:a2:v1 0x144:rt:v1:t2 0x14c:rd:t2:t3 0x14c:rt:v1:t2 0x150:rd:t3:t4 0x150:rt:t2:t3 0x154:rs:t3:t4 0x154:rt:t4:t5 0x158:rs:t4:t5 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x801326F8 */
/* Size: 364 bytes (91 instructions) */
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
extern void day_flag_set(s32);
extern void bustup_ptr_init(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void strcpy(void *, void *);
extern void shougatu_res_init(void);

extern u8 bg_change_tbl2;
extern u8 ba_string[];
extern u8 girl_appear[];
extern u8 screen_w[];
extern u8 screen_bri;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 fade_bri;
extern u8 shougatu_init_str0[];
extern s16 shougatu_meet_now_serifu;
extern s16 shougatu_meet_now_serifu_in;
extern u8 shougatu_serifu6_flag;
extern u8 shougatu_meet_girl;
extern u8 shougatu_alone_flag;
extern u8 shougatu_girl0_good;

void shougatu_init(void)
{
    s16 i;
    s32 v;
    u32 b;

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    strcpy(ba_string, shougatu_init_str0);
    vblank_flag = 0;
    vb_ready = 1;
    shared_system_state_reset();
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    day_flag_set(0);
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    shougatu_res_init();
    date_name_set(0);
    shougatu_meet_now_serifu = 0;
    shougatu_meet_now_serifu_in = 0;
    shougatu_serifu6_flag = 0;
    shougatu_meet_girl = 255;
    shougatu_alone_flag = 0;
    shougatu_girl0_good = 1;
    for (i = 0; i < 11; i++) {
        v = *(s32 *)&screen_w[i * 56 + 456];
        b = ((u32)v << 30) >> 31;
        girl_appear[i] = (u8)((b < 1U) ^ 1);
    }
    sub_sub_step_up();
}
