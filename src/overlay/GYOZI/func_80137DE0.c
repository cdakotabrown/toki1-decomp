/* func_80137DE0 -- GYOZI overlay, Address: 0x80137DE0, Size: 108 bytes (27 insns) */
#include "types.h"

extern void func_80090960(s32, s32);
extern void func_8004DE1C(void);

extern s32 D_80147100;
extern s32 D_80147134;
extern s32 D_80147168;
extern s32 D_8014718C;
extern s32 D_80147190;
extern s32 D_80147194;
extern s16 D_80147198;
extern s16 D_8014719C;

void func_80137DE0(void)
{
    func_80090960(13, 38);
    D_8014718C = D_80147100;
    D_80147190 = D_80147134;
    D_80147194 = D_80147168;
    D_80147198 = 9;
    D_8014719C = 0;
    func_8004DE1C();
}
