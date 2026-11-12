/* kangei_zyotai_serifu_set22 - KANGEI overlay, Address: 0x8013888C, Size: 96 bytes (24 insns) */
/* @SymbolSize: 96 */
/* @PostProc: patch_insn_reg_field 0x20:rt:v1:t6 0x24:rs:v1:t6 0x24:rt:t6:t7 0x28:rs:v1:t6 0x2c:rt:t0:t9 0x30:rs:t6:t7 0x34:rs:v1:t6 0x34:rt:t3:t2 0x38:rs:t3:t2 0x38:rt:t4:t2 0x40:rd:t1:t0 0x40:rt:t0:t9 0x44:rd:t2:t1 0x44:rt:t1:t0 0x48:rd:t5:t3 0x48:rt:t4:t2 0x4c:rd:t6:t4 0x4c:rs:t2:t1 0x4c:rt:t5:t3 0x50:rs:t6:t4 0x50:rt:t7:t5 0x5c:rt:t7:t5 */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void sub_sub_step_up(void);

extern s16 kangei_now_s;

void kangei_zyotai_serifu_set22(void)
{
    u32 key;
    u32 a, b, c;
    s32 val;

    key = (u32)(get_g_zyotai_s(0) & 0x7F);
    a = (key < 2U) ^ 1U;
    b = (key < 3U) ^ 1U;
    c = (key < 4U) ^ 1U;
    val = (s32)(a << 1) + (s32)(b << 1) + (s32)(c << 1) + 22;
    kangei_now_s = (s16)val;
    sub_sub_step_up();
}
