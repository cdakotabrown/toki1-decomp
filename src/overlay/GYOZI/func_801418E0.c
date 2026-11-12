/* func_801418E0 -- GYOZI overlay, Address: 0x801418E0, Size: 56 bytes (14 insns) */
#include "types.h"

extern s32 func_8005B32C(void);
extern void func_8005B06C(s32 a0);
extern void func_8004DE1C(void);

void func_801418E0(void)
{
    if (func_8005B32C() != 0) {
        func_8005B06C(1);
        func_8004DE1C();
    }
}
