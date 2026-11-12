/* func_801416BC -- GYOZI overlay, Address: 0x801416BC, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s16 D_80148620;

void func_801416BC(void)
{
    D_80148620 = 3;
    func_8004DE1C();
}
