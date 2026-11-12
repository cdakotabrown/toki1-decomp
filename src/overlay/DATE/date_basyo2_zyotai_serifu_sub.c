/* @PostProc: patch_insn_reg_field 0x28:rt:v1:t6 0x2c:rt:t6:t7 0x30:rs:t6:t7 0x30:rt:t6:t7 0x34:rs:v1:t6 0x34:rt:t7:t8 0x38:rs:t7:t8 0x3c:rs:v1:t6 0x40:rt:t1:t0 0x44:rs:t6:t7 0x48:rd:t2:t1 0x48:rt:t1:t0 0x4c:rs:t2:t1 0x4c:rt:t3:t2 0x58:rt:t3:t2 */
/* date_basyo2_zyotai_serifu_sub - DATE overlay, Address: 0x80139280, Size: 92 bytes (23 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern s32 get_g_zyotai_s(s32, s32, s32, s32);
extern u8 now_girl;
extern s16 now_serifu;

void date_basyo2_zyotai_serifu_sub(s32 arg0, s32 a1, s32 a2, s32 a3)
{
    s32 v0;
    s32 t6;
    s32 t8;
    s32 t0;
    s32 t2;

    if (arg0) {
    }
    v0 = get_g_zyotai_s((s32)now_girl, a1, a2, a3);
    t6 = v0 & 0x7F;
    t8 = ((u32)t6 < 2u) ^ 1;
    t0 = ((u32)t6 < 3u) ^ 1;
    t2 = now_serifu - t8 - t0 + 2;
    now_serifu = (s16)t2;
    sub_sub_step_up();
}
