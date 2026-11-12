/* func_80133814 -- GYOZI overlay, Address: 0x80133814, Size: 40 bytes (10 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s16 D_80145EB4;

void func_80133814(void)
{
    D_80145EB4 = 39;
    func_8004DE1C();
}
