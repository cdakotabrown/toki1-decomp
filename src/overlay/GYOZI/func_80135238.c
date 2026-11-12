/* func_80135238 -- GYOZI overlay, Address: 0x80135238, Size: 68 bytes (17 insns) */
#include "types.h"

extern void func_800549E8(s32 a0);
extern void func_8004DE1C(void);

extern s8 D_801461A0;

void func_80135238(void)
{
    func_800549E8(96);
    func_800549E8(97);
    func_800549E8(98);
    func_800549E8(99);
    D_801461A0 = 0;
    func_8004DE1C();
}
