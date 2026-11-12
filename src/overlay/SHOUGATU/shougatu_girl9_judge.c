/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: patch_insn_reg_field 0x1c:rd:t7:t8 0x20:rs:t7:t8 0x28:rt:t8:t9 0x2c:rs:t8:t9 0x2c:rt:t8:t9 0x30:rt:t9:t0 0x34:rs:t8:t9 0x40:rs:t9:t0 0x40:rt:t9:t0 0x44:rt:t1:t2 0x48:rs:t9:t0 0x48:rt:t0:t1 0x4c:rs:t0:t1 0x5c:rt:t1:t2 0x60:rt:t2:t4 0x68:rs:t2:t4 0x68:rt:t3:t5 0x6c:rt:t3:t5 0x70:rt:t4:t6 0x74:rt:t4:t6 0x7c:rt:t5:t7 0x80:rt:t6:t8 0x84:rs:t6:t8 0x84:rt:t6:t8 0x88:rt:t5:t7 0x90:rt:t7:t9 0x94:rs:t7:t9 0x94:rt:t7:t9 0x98:rt:t6:t8 0xa0:rt:t8:t0 0xa4:rs:t8:t0 0xa4:rt:t8:t0 0xa8:rt:t7:t9 0xb0:rt:t8:t0 0xb8:rt:t9:t1 0xbc:rt:t9:t1 0xec:rt:t0:t2 0xf0:rs:t0:t2 0xf0:rt:t0:t2 0xf8:rs:t0:t2 0xf8:rt:t1:t3 0xfc:rt:t1:t3 */
/* @SymbolStart: 0x80140BA8 */
/* @SymbolSize: 256 */
/* Address: 0x80140BB0 */
#include "types.h"

extern u8 screen_w[];

extern void sub_sub_step_up();
extern void date_name_set();
extern void event_log_add();

extern s32 girl9_flag;
extern s16 girl9_tokimeki;
extern s32 frame_counter;
extern u8 now_sub_sub_step;

extern u8 shougatu_girl9_appear;

extern s16 girl9_yuukou;
extern s16 shougatu_menu_now_serifu;
extern s32 shougatu_menu_ss_serifu;
extern s32 shougatu_menu_ss_serifu_kind;
extern s32 shougatu_menu_sss_sound;
extern u8 now_girl;
extern s32 shougatu_menu_serifu_tbl[];
extern s32 shougatu_menu_serifu_kind_tbl[];
extern s32 shougatu_menu_sound_tbl[];

void shougatu_girl9_judge(void)
{
    s32 t6 = girl9_flag;
    u8 *v0;

    if ((t6 << 30) < 0) goto early;
    if (girl9_tokimeki < 20) goto early;
    if ((frame_counter & 1) == 0) goto early;
    shougatu_girl9_appear = 1;
    v0 = (u8 *)((u32)screen_w | 0);
    v0[960] |= 2;
    girl9_yuukou = 50;
    shougatu_menu_now_serifu = 5;
    shougatu_menu_ss_serifu = shougatu_menu_serifu_tbl[8];
    shougatu_menu_ss_serifu_kind = shougatu_menu_serifu_kind_tbl[8];
    shougatu_menu_sss_sound = shougatu_menu_sound_tbl[8];
    now_girl = 9;
    event_log_add(4, 1);
    date_name_set(now_girl);
    sub_sub_step_up();
    return;
early:
    now_sub_sub_step = now_sub_sub_step + 11;
}
