/* func_80136C78 -- GYOZI overlay, Address: 0x80136C78, Size: 36 bytes (9 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s8 D_800F62CF;

void func_80136C78(void)
{
    D_800F62CF = 0;
    func_8004DE1C();
}
