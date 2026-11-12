/* func_80138A48 -- GYOZI overlay, Address: 0x80138A48, Size: 96 bytes (24 insns) */
/* @PostProc: patch_insn_reg_field 0x30:rt:t6:v0 0x38:rs:t6:v0 0x40:rt:t7:t6 0x44:rs:t7:t6 0x44:rt:t7:t6 0x4c:rs:t7:t6 */
#include "types.h"

extern void func_8013A820(void);
extern void func_8008E408(s32 a0);
extern void func_80072FE8(void);

extern s32 D_800F6474;
extern s16 D_801474AC;

void func_80138A48(void)
{
    s32 frameCount;
    s32 isOne;

    func_8013A820();

    frameCount = D_800F6474;
    isOne = (u32)(frameCount ^ 1) < 1;
    D_800F6474 = frameCount + 1;

    if (isOne) {
        if (D_801474AC == 3) {
            func_8008E408(3);
            func_80072FE8();
        }
    }
}
