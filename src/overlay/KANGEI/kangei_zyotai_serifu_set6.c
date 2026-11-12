/* @PostProc: patch_insn_reg_field 0x20:rt:v1:t6 0x24:rs:v1:t6 0x24:rt:t6:t7 0x28:rs:v1:t6 0x2c:rt:t0:t9 0x30:rs:t6:t7 0x34:rs:v1:t6 0x34:rt:t3:t2 0x38:rs:t3:t2 0x38:rt:t4:t2 0x40:rt:t0:t9 0x40:rd:t1:t0 0x44:rt:t1:t0 0x44:rd:t2:t1 0x48:rt:t4:t2 0x48:rd:t5:t3 0x4c:rs:t2:t1 0x4c:rt:t5:t3 0x4c:rd:t6:t4 0x50:rs:t6:t4 0x50:rt:t7:t5 0x5c:rt:t7:t5 */
/* kangei_zyotai_serifu_set6 - KANGEI overlay, Address: 0x801388EC, Size: 96 bytes (24 insns) */
/* @SymbolSize: 96 */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a0);
extern void sub_sub_step_up(void);

extern s16 kangei_now_s;

void kangei_zyotai_serifu_set6(void)
{
    register u32 key;
    register u32 t7, t9, t2;
    register u32 t8, t0, t3, t1, t4, t5;
    register s32 result;

    result = get_g_zyotai_s(0);
    key = (u32)(result & 0x7F);
    t7 = (u32)(key < 2u);
    t9 = (u32)(key < 3u);
    t9 ^= 1u;
    t7 ^= 1u;
    t2 = (u32)(key < 4u);
    t2 ^= 1u;
    t8 = t7 << 1;
    t0 = t9 << 1;
    t1 = t8 + t0;
    t3 = t2 << 1;
    t4 = t1 + t3;
    t5 = t4 + 6;
    kangei_now_s = (s16)t5;
    sub_sub_step_up();
}
