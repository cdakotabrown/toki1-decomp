/* func_8013BBE4 -- GYOZI overlay, Address: 0x8013BBE4, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_8013A820(void);

extern s32 D_8012E6B0;
extern s16 D_801474AC;

void func_8013BBE4(void)
{
    func_8013A820();
    if (D_801474AC == 2) {
        D_8012E6B0 = 1;
    } else {
        D_8012E6B0 = 0;
    }
}
