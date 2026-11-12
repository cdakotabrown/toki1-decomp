/* func_8013FA94 -- GYOZI overlay, Address: 0x8013FA94, Size: 56 bytes (14 insns) */
#include "types.h"

extern void func_8004C6A0(s32 a0, s32 a1);
extern void func_80050D60(s32 a0, s32 a1);
extern void func_80081D30(s32 a0);

void func_8013FA94(void)
{
    func_8004C6A0(0, 64);
    func_80050D60(0, 0);
    func_80081D30(1);
}
