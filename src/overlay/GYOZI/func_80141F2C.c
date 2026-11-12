/* func_80141F2C -- GYOZI overlay, Address: 0x80141F2C, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_8005B264(s32 a0);
extern void func_8005AEFC(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_80148BEC;

void func_80141F2C(void)
{
    func_8005B264(D_80148BEC);
    func_8005AEFC(1);
    func_8004DE1C();
}
