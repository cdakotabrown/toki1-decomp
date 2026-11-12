/* @PostProc: patch_insn_reg_field 0x28:rt:t6:t7 0x2c:rs:t6:t7 0x2c:rt:t6:t7 0x30:rt:v1:t6 0x34:rs:v1:t6 0x34:rt:t8:t9 0x38:rd:t7:t8 0x38:rt:t6:t7 0x3c:rs:t8:t9 0x40:rs:v1:t6 0x44:rd:t7:t8 0x44:rs:t7:t8 0x44:rt:t6:t7 0x48:rs:t7:t8 0x4c:rt:t2:t1 0x50:rd:t3:t2 0x50:rt:t2:t1 0x54:rs:t3:t2 0x54:rt:t4:t3 0x5c:rt:t4:t3 0x60:rt:t5:t4 0x64:rs:t5:t4 0x64:rt:t5:t4 0x6c:rt:t5:t4 0x70:rt:t6:t5 0x74:rs:t6:t5 0x74:rt:t6:t5 0x7c:rt:t8:t6 0x80:rs:t8:t6 0x80:rt:t8:t6 0x84:rt:t6:t5 0x94:rt:t8:t6 0x9c:rt:t9:t8 0xa0:rs:t9:t8 0xa0:rt:t9:t8 0xc0:rt:t9:t8 */
/* Address: 0x80158588 */
/* Size: 196 bytes (49 instructions) */
#include "types.h"

extern u8 D_800B1746;
extern u8 D_800B0895;
extern s16 D_80124354;
extern s16 D_80124358;
extern s32 D_80124258;
extern s32 D_801242C4;
extern s32 D_80124330;
extern s32 D_8009471C;
extern s32 D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s32 D_800EECBC;

extern s32 func_8002328C();
extern void func_80049B20();

void func_80158588(void) {
    s32 v0;

    v0 = func_8002328C(D_800B1746);
    D_80124354 = D_800B0895 * 3 + (((u32)(v0 & 0x7F) < 2) ^ 1) +
                 (((u32)(v0 & 0x7F) < 3) ^ 1) - 273;
    D_8009471C = (s32)&D_80124354;
    D_80094720 = (s32)&D_80124358;
    D_80094724 = D_80124258;
    D_80094728 = D_801242C4;
    D_8009472C = D_80124330;
    func_80049B20(D_800EECBC, 1, 0);
}
