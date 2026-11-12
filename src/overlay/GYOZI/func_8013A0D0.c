/* func_8013A0D0 -- GYOZI overlay, Address: 0x8013A0D0, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_8013A820(void);

extern s32 D_8012E6B0;
extern s16 D_801474AC;

void func_8013A0D0(void)
{
    func_8013A820();
    if (D_801474AC == 3) {
        D_8012E6B0 = 1;
    } else {
        D_8012E6B0 = 0;
    }
}
