/* @PostProc: patch_insn_reg_field 0x44:rt:v1:t7 0x48:rs:v1:t7 0x48:rt:t7:t8 0x4c:rs:v1:t7 0x4c:rt:t8:t9 0x50:rs:t8:t9 0x50:rt:t8:t9 0x54:rs:t7:t8 0x54:rt:t7:t8 0x58:rs:v1:t7 0x58:rt:t0:t1 0x5c:rs:t0:t1 0x5c:rt:t0:t1 0x60:rd:t9:t0 0x60:rs:t7:t8 0x60:rt:t8:t9 0x64:rd:t1:t2 0x64:rs:t9:t0 0x64:rt:t0:t1 0x6c:rt:t1:t2 0x70:rt:t2:t3 0x74:rs:t2:t3 0x74:rt:t2:t3 0x7c:rt:t2:t3 0x80:rt:t3:t4 0x84:rs:t3:t4 0x84:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x94:rt:t3:t4 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa4:rt:t4:t5 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xb8:rt:t5:t6 0xd0:rt:t6:t7 */
/* Address: 0x801542B8 */
/* Size: 212 bytes (53 instructions) */
#include "types.h"

extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;

extern u8 D_800B1746;
extern u8 D_800B1AF9;
extern s16 D_8012435C;
extern s32 D_80124360;
extern s32 D_80124268;
extern s32 D_801242D4;
extern s32 D_80124340;
extern s32 D_800EECBC;

extern s32 func_8002328C(s32);
extern void func_80049B20(s32, s32, s32);

void func_801542B8(void) {
    s32 ret;
    u32 once;
    s32 first;
    u32 r;

    ret = func_8002328C(D_800B1746);
    once = D_800B1AF9;
    first = (once < 1);
    D_800B1AF9 = once + 1;
    if (first) {
        r = ret & 0x7F;
        D_8012435C = (r >= 2) + (r >= 3) + (r >= 4);
    }
    D_8009471C = (s32)&D_8012435C;
    D_80094720 = (s32)&D_80124360;
    D_80094724 = D_80124268;
    D_80094728 = D_801242D4;
    D_8009472C = D_80124340;
    func_80049B20(D_800EECBC, 1, 0);
}
