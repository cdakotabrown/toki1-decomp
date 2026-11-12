/* func_8013C24C -- GYOZI overlay, Address: 0x8013C24C, Size: 68 bytes (17 insns) */
#include "types.h"

extern void func_800504CC(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp10, s32 sp14);
extern void func_8004DE1C(void);

void func_8013C24C(void)
{
    func_800504CC(1, 0xB290, 0xB271, 0xBB4E, 0xBB06, 0xBAF5);
    func_8004DE1C();
}
