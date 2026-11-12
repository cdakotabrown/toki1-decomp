/* func_801391D0 -- GYOZI overlay, Address: 0x801391D0, Size: 88 bytes (22 insns) */
#include "types.h"

extern void func_8013A820(void);
extern void func_80086AB0(s32);

extern s32 D_800F6474;
extern s16 D_801474AC;

void func_801391D0(void)
{
    s32 temp;
    s32 cond;

    func_8013A820();
    temp = D_800F6474;
    cond = (u32)temp < 1u;
    D_800F6474 = temp + 1;
    if (cond != 0) {
        if (D_801474AC == 3) {
            func_80086AB0(1280);
        }
    }
}
