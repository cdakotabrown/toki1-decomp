/* func_80141EE0 -- GYOZI overlay, Address: 0x80141EE0, Size: 76 bytes (19 insns) */
#include "types.h"

extern s32 func_8005B28C(void);
extern void func_8005B264(s32 a0);
extern void func_8005AEFC(s32 a0);
extern void func_8004DE1C(void);

extern s32 D_8012E68C;
extern s8 D_80148BEC;

void func_80141EE0(void)
{
    D_80148BEC = (s8)func_8005B28C();
    D_8012E68C = 114;
    func_8005B264(30);
    func_8005AEFC(2);
    func_8004DE1C();
}
