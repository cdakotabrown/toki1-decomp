/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x28:rt:a0:t1 0x2c:rs:a0:t1 0x2c:rt:a0:t1 0x30:rt:a3:a2 0x34:rs:a0:t1 0x34:rt:a3:a2 0x38:rt:a1:t6 0x3c:rs:a1:t6 0x3c:rt:t6:t7 0x40:rs:a1:t6 0x40:rt:t7:t8 0x44:rs:t7:t8 0x44:rt:t7:t8 0x48:rs:t6:t7 0x48:rt:t6:t7 0x4c:rd:t8:t9 0x4c:rs:t6:t7 0x4c:rt:t7:t8 0x50:rd:t9:t0 0x50:rt:t8:t9 0x58:rt:a2:a1 0x5c:rt:t9:t0 0x60:rs:a2:a1 0x60:rt:a2:a1 0x64:rd:t0:t2 0x70:rs:a2:a1 0x70:rt:t0:t2 0x74:rt:t1:t3 0x78:rt:a0:t5 0x7c:rs:t1:t3 0x7c:rt:t2:t4 0x80:rt:t2:t4 0x84:rs:a0:t5 0x84:rt:a0:t5 0x8c:rs:a0:t5 0x8c:rt:a3:a2 0x90:rd:t3:t6 0x94:rs:a2:a1 0x94:rt:t3:t6 0x98:rt:t4:t7 0xa0:rs:t4:t7 0xa0:rt:t5:t8 0xa8:rt:t5:t8 */
/* date_basyo0_zyotai_step - DATE overlay, Address: 0x8013A900, Size: 180 bytes (45 insns) */
#include "types.h"

extern s32 get_g_zyotai_s(s32 idx);
extern void param_clamp();
extern void sub_sub_step_up();
extern volatile u8 now_girl;
extern u8 screen_w[];
extern s16 now_serifu;
extern s16 now_serifu_in;

void date_basyo0_zyotai_step(void)
{
    s32 v0;
    u32 n;
    u8 idx;

    v0 = get_g_zyotai_s(now_girl);
    n = v0 & 0x7F;
    now_serifu = (s16)(((n >= 2) + (n >= 3)) << 1);
    now_serifu_in = 0;
    idx = now_girl;
    *(s16 *)&screen_w[idx * 56 + 446] =
        *(s16 *)&screen_w[idx * 56 + 446] + 1;
    idx = now_girl;
    *(s16 *)&screen_w[idx * 56 + 450] =
        *(s16 *)&screen_w[idx * 56 + 450] + 1;
    param_clamp();
    sub_sub_step_up();
}
