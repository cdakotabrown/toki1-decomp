/* func_8013B528 -- GYOZI overlay, Address: 0x8013B528, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s16 D_801474A8;

void func_8013B528(void)
{
    D_801474A8 = 3;
    func_8004DE1C();
}
