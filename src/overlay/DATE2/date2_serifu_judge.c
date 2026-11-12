/* @SymbolStart: 0x80136BA4 */
/* @SymbolSize: 328 */
/* @PostProc: patch_insn_reg_field 0x80:rt:v0:t2 0x98:rs:v0:t2 0x98:rt:v0:t2 0x9c:rt:t3:t4 0xa0:rd:t2:t3 0xa0:rt:v0:t2 0xa4:rd:t2:t3 0xa4:rs:t2:t3 0xa4:rt:v0:t2 0xa8:rd:t2:t3 0xa8:rt:t2:t3 0xac:rd:t3:t4 0xac:rs:t3:t4 0xac:rt:t2:t3 0xb0:rs:t3:t4 0xb0:rt:t3:t4 0xb4:rt:t4:t5 0xb8:rs:t3:t4 0xc4:rs:t4:t5 0xc4:rt:t4:t5 0xc8:rt:t5:t6 0xcc:rs:t4:t5 0xd8:rs:t5:t6 0xd8:rt:t5:t6 0xe0:rs:t5:t6 0xe0:rt:t6:t7 0xe4:rt:t6:t7 0x108:rt:t9:t0 0x10c:rt:t7:t8 0x110:rs:t7:t8 0x110:rt:t7:t8 0x118:rs:t7:t8 0x118:rt:t8:t9 0x120:rt:t8:t9 0x124:rs:t9:t0 0x124:rt:t9:t0 0x12c:rs:t9:t0 0x12c:rt:t0:t1 0x134:rt:t0:t1 */
/* date2_serifu_judge -- DATE2 overlay, Address: 0x80136BAC, Size: 320 bytes (80 insns) */
#include "types.h"

extern u8 now_girl;
extern s16 girl0_yuukou;
extern s16 date2_now_s;
extern s16 girl0_syoushin;
extern s16 girl0_tokimeki;
extern u8 screen_w[][56];

s32 get_h_yuukou(u8 arg0);
s32 get_h_tokimeki(u8 arg0);
void sub_sub_step_up(void);

void date2_serifu_judge(void)
{
    u8 t2;
    s32 v0;
    u8 a0;

    if (girl0_yuukou < 10)
    {
        date2_now_s = (s16)(date2_now_s + 2);
        now_girl = 0;
        goto tail;
    }

    a0 = now_girl;
    v0 = get_h_yuukou(a0);
    if ((u32)v0 < 10U)
    {
        date2_now_s = (s16)(date2_now_s + 2);
        goto tail;
    }

    if (girl0_syoushin >= 51)
    {
        now_girl = 0;
        goto tail;
    }

    t2 = screen_w[0][3935];
    if (*(s16 *)(screen_w[t2] + 454) >= 51)
    {
        goto tail;
    }

    if (girl0_tokimeki >= 50)
    {
        date2_now_s = (s16)(date2_now_s + 1);
        now_girl = 0;
        goto tail;
    }

    v0 = get_h_tokimeki(now_girl);
    if ((u32)v0 >= 50U)
    {
        date2_now_s = (s16)(date2_now_s + 1);
        goto tail;
    }

    date2_now_s = (s16)(date2_now_s + 4);
    sub_sub_step_up();
    sub_sub_step_up();
tail:
    sub_sub_step_up();
}
