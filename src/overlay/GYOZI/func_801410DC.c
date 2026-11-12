/* func_801410DC -- GYOZI overlay, Address: 0x801410DC, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s8 D_8012B8C2;

void func_801410DC(void)
{
    D_8012B8C2 = 9;
    func_8004DE1C();
}
