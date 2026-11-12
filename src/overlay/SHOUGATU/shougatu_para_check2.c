/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: patch_insn_reg_field 0x10:rt:t7:v0 0x14:rs:t7:v0 0x14:rt:t7:v0 0x18:rt:t6:t1 0x1c:rd:t8:t6 0x1c:rt:t7:v0 0x20:rd:t9:t7 0x20:rt:t8:t6 0x24:rd:t0:t8 0x24:rt:t9:t7 0x28:rd:t0:t8 0x28:rs:t0:t8 0x28:rt:t9:t7 0x2c:rd:t0:t8 0x2c:rt:t0:t8 0x30:rd:t0:t8 0x30:rs:t0:t8 0x30:rt:t9:t7 0x34:rs:t0:t8 0x34:rt:t1:v0 0x38:rs:t6:t1 0x38:rt:t6:t1 0x3c:rd:t2:t9 0x3c:rt:t1:v0 0x40:rd:t3:t0 0x40:rt:t2:t9 0x48:rs:t6:t1 0x48:rt:t3:t0 0x54:rt:v0:t2 0x58:rs:v0:t2 0x58:rt:v0:t2 0x5c:rt:v1:v0 0x60:rd:t4:t3 0x60:rt:v0:t2 0x64:rd:t5:t4 0x64:rt:t4:t3 0x68:rs:t5:t4 0x68:rt:t7:t5 0x6c:rs:t7:t5 0x6c:rt:t8:t5 0x70:rd:t9:t6 0x70:rt:t8:t5 0x74:rd:t9:t6 0x74:rs:t9:t6 0x74:rt:t8:t5 0x78:rs:t9:t6 0x78:rt:t0:t7 0x80:rs:v1:v0 0x80:rt:v1:v0 0x84:rt:t0:t7 0x88:rs:v1:v0 0x8c:rs:t5:t4 0x8c:rt:t6:t8 0x90:rd:t3:t9 0x90:rt:t6:t8 0x94:rs:t3:t9 0x94:rt:t4:t0 0x9c:rd:t5:t3 0x9c:rs:t2:t0 0x9c:rt:t4:t2 0xa4:rs:v1:v0 0xa4:rt:t5:t3 0xb0:rt:t7:t5 0xb4:rs:t7:t5 0xb4:rt:t7:t5 0xb8:rt:t9:t7 0xbc:rs:t9:t7 0xbc:rt:t9:t7 0xc4:rs:t7:t5 0xc4:rt:t8:t6 0xc8:rt:t8:t6 0xd0:rs:t9:t7 0xd0:rt:t0:t4 0xd4:rt:t0:t4 0xd8:rt:t1:t8 0xdc:rs:t1:t8 0xdc:rt:t1:t8 0xe4:rs:t1:t8 0xe4:rt:t6:t9 0xec:rt:t6:t9 */
/* @SymbolStart: 0x8013D9FC */
/* @SymbolSize: 304 */
/* Address: 0x8013DA30 */
#include "types.h"

extern u8 screen_w[];

extern void sub_sub_step_up();
extern void param_clamp();

extern s32 girl12_flag;
extern s16 para_un;
extern s32 girl14_flag;
extern s16 shougatu_menu_now_serifu;
extern volatile s16 para_kon;
extern volatile s16 para_str;

void shougatu_para_check2(void)
{
    s32 t0;
    s32 t1;
    s32 t4;
    s32 t7;
    u8 *p;

    t7 = ((u32)girl12_flag << 20) >> 29;
    t0 = (s16)(t7 * 25 + 150);
    t1 = para_un;
    if (t1 < t0) goto skip;

    t4 = ((u32)girl14_flag << 20) >> 29;
    shougatu_menu_now_serifu = ((t4 == 0) ^ 1) * 3 + 4;
    p = (u8 *)((u32)screen_w | 0);
    p[1241] = (u8)((((t4 + 1) << 1) & 0xE) | (p[1241] & 0xFFF1));
    sub_sub_step_up();
    return;

skip:
    shougatu_menu_now_serifu = shougatu_menu_now_serifu + 1;
    para_kon = para_kon + 10;
    para_str = para_str + 10;
    param_clamp();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
}
