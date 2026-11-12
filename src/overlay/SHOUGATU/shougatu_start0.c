/* @PostProc: patch_insn_reg_field 0x3c:rt:v0:t9 0x40:rs:v0:t9 0x40:rt:v0:t9 0x44:rt:v1:v0 0x48:rs:v0:t9 0x48:rt:t9:t0 0x4c:rs:t9:t0 0x50:rs:v1:v0 0x50:rt:v1:v0 0x54:rs:v0:t9 0x54:rt:t0:t1 0x5c:rt:t0:t1 0x60:rt:t1:t2 0x64:rs:t1:t2 0x64:rt:t1:t2 0x6c:rd:t2:t3 0x6c:rt:t1:t2 0x70:rd:t3:t4 0x70:rs:v1:v0 0x70:rt:t2:t3 0x78:rs:t3:t4 0x94:rt:t4:t5 0x98:rt:t4:t5 0x9c:rd:t5:t6 0xa0:rd:t6:t7 0xa0:rt:t5:t6 0xa8:rs:t6:t7 0xac:rt:t7:t8 0xb0:rs:t7:t8 0xb4:rt:t8:t9 0xbc:rt:t8:t9 0x134:rt:t9:t1 0x138:rt:t9:t1 0x140:rt:t0:t2 0x144:rt:t0:t2 0x198:rt:v1:v0 0x19c:rs:v1:v0 0x19c:rt:v1:v0 0x1a0:rs:v1:v0 0x1a0:rt:v0:t3 0x1a4:rt:t6:t0 0x1a8:rd:t1:t4 0x1a8:rt:v0:t3 0x1ac:rd:t2:t5 0x1ac:rt:t1:t4 0x1b0:rs:t2:t5 0x1bc:rs:v1:v0 0x1bc:rt:t3:t6 0x1c4:rs:t3:t6 0x1c4:rt:t4:t7 0x1c8:rs:t4:t7 0x1c8:rt:t5:t8 0x1cc:rs:v1:v0 0x1cc:rt:t5:t8 0x1d0:rs:t6:t0 0x1d0:rt:t6:t0 0x1d8:rd:t7:t9 0x1d8:rt:t6:t0 0x1dc:rd:t8:t1 0x1dc:rt:t7:t9 0x1e0:rs:t8:t1 0x200:rt:t9:t2 0x20c:rt:t9:t2 0x218:rt:t9:t2 0x21c:rt:t0:t3 0x220:rs:t0:t3 0x220:rt:t0:t3 0x228:rt:t1:t4 0x22c:rs:t1:t4 0x22c:rt:t1:t4 0x230:rt:t0:t3 0x238:rt:t2:t5 0x23c:rs:t2:t5 0x23c:rt:t2:t5 0x240:rt:t1:t4 0x24c:rt:t2:t5 */
/* @SymbolStart: 0x8013DB7C */
/* @SymbolSize: 592 */
/* Address: 0x8013DB84, 584 bytes (146 insns) + an 8-byte cross-prologue head */
#include "types.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void tpage_buf_clear(void);
extern void class_switch(s32);
extern void class_clear_all(s32);
extern void text_sys_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void yazirushi_off(void);
extern void shared_system_state_reset(void);
extern void strcpy(u8 *, u8 *);
extern void shougatu_ptrs2_set(void);

extern u8 bg_change_tbl2;
extern u8 p_name[];
extern u8 g_name[];
extern u8 ba_string[];
extern u8 screen_w[];
extern u8 screen_bri;
extern u8 p_sei[];
extern s32 p_birthday;
extern s32 girl0_birth;
extern s32 girl12_flag;
extern volatile u8 schedule_cursor;
extern volatile u8 day_flag;
extern u8 now_girl;
extern u8 vblank_flag;
extern s32 vb_ready;
extern u8 fade_bri;
extern u8 shougatu_start0_str0[];
extern u8 shougatu_start0_str1[];
extern u8 shougatu_start0_str2[];
extern s32 shougatu_menu_ss_serifu;
extern s32 shougatu_menu_ss_serifu_kind;
extern s32 shougatu_menu_sss_sound;
extern s16 shougatu_menu_now_serifu;
extern s16 shougatu_menu_now_serifu_in;
extern s32 shougatu_menu_serifu_tbl[];
extern s32 shougatu_menu_serifu_kind_tbl[];
extern s32 shougatu_menu_sound_tbl[];

s32 shougatu_start0(void)
{
    s32 x;
    s32 y;
    u32 f;
    u32 g;
    u8 *p;
    u8 *q;
    s32 w;
    u32 b;

    x = girl0_birth;
    if ((x & 0xF) == 12) {
        if (((u32)(x << 23) >> 27) == 24) {
            f = day_flag;
            p = screen_w;
            if ((f & 0x20) == 0) {
                day_flag = f | 0x20;
                p[1693 + (schedule_cursor << 2)] = 0;
                goto_step(87);
                return 0;
            }
        }
    }

    y = p_birthday;
    g = day_flag;
    if ((y & 0xF) == 12) {
        if (((u32)(y << 23) >> 27) == 24) {
            if ((g & 0x40) == 0) {
                day_flag = g | 0x40;
                goto_step(86);
                return 0;
            }
        }
    }

    draw2d3d(1, 0);
    class_switch(1);
    prim_init();
    class_clear_all(0);
    back_clear_switch(1);
    tpage_buf_clear();
    text_sys_init();
    shared_system_state_reset();
    strcpy(ba_string, shougatu_start0_str0);
    vblank_flag = 0;
    vb_ready = 1;
    screen_bri = 128;
    fade_bri = 0;
    bg_change_tbl2 = 0;
    shougatu_menu_now_serifu = 0;
    shougatu_menu_now_serifu_in = 0;
    bustup_ptr_init();
    hizuke_init();
    message_window_init();
    yazirushi_off();
    strcpy(p_name, p_sei);

    q = screen_w;
    b = q[1129];
    w = *(s32 *)(q + 1128);
    if (((u32)(w << 20) >> 29) >= 3) {
        q[1129] = (q[1129] & 0xFFF1) | 0x0004;
    }
    if (((u32)(girl12_flag << 20) >> 29) == 0) {
        strcpy(g_name, shougatu_start0_str1);
    } else {
        strcpy(g_name, shougatu_start0_str2);
    }
    now_girl = 12;
    shougatu_ptrs2_set();
    shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[11];
    shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[11];
    shougatu_menu_sss_sound = shougatu_menu_sound_tbl[11];
    sub_sub_step_up();
}
