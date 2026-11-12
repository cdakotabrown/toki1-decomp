/* @SymbolStart: 0x80137858 */
/* @SymbolSize: 200 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t6:t8 0x1c:rs:t6:t8 0x1c:rt:t6:t8 0x20:rd:t7:t6 0x28:rd:t8:t7 0x28:rt:t7:t6 0x2c:rs:t6:t7 0x34:rt:t9:t0 0x38:rs:t9:t0 0x38:rt:t9:t0 0x3c:rt:t0:t9 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x6C */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6C */
/* @PostProc: swap_instructions --offset1 0x6C --offset2 0x70 */
/* func_8013786C -- GYOZI overlay, Address: 0x80137858 (cross-prologue head; */
/* limits itself with @SymbolSize: 48). */
/* Size: 200 bytes (49 insns + 1 word of inter-function alignment padding). */
#include "types.h"

extern u32 func_8005EBE0(s32 a0);
extern void func_8004DEAC(s32 a0);
extern void func_80138858(void);
extern void func_8004DE1C(void);

extern u32 D_800F5490;
extern u8 D_800F53DF;
extern u8 D_800F53E0;
extern u8 D_800F5AB1;
extern u8 D_800F5AB2;
extern u8 D_800F62CF;
extern u8 D_800F6479;
extern u8 D_800F647A;

s32 func_8013786C(void)
{
    if (((D_800F5490 << 23) >> 27) == D_800F53E0) {
        if ((D_800F5490 & 0xF) == D_800F53DF) {
            if (func_8005EBE0(D_800F62CF) >= 70) {
                D_800F5AB1 = D_800F6479;
                D_800F5AB2 = D_800F647A + 1;
                func_8004DEAC(3);
                return 0;
            }
        }
    }

    if (func_8005EBE0(D_800F62CF) >= 70) {
        func_80138858();
    } else {
        func_8004DE1C();
    }
}
