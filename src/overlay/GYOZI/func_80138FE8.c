/* func_80138FE8 -- GYOZI overlay, Address: 0x80138FE8, Size: 84 bytes (21 insns) */
#include "types.h"

extern void func_8013A820(void);
extern void func_80086AB0(s32);

extern s32 D_800F6474;
extern s16 D_801474AC;

void func_80138FE8(void)
{
    s32 temp;
    s32 cond;

    func_8013A820();
    if (D_801474AC == 2) {
        temp = D_800F6474;
        cond = (u32)temp < 1u;
        D_800F6474 = temp + 1;
        if (cond != 0) {
            func_80086AB0(1281);
        }
    }
}
