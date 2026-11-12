/* @SymbolSize: 88 */
/* func_801387C8 -- GYOZI overlay, Address: 0x801387C8, Size: 88 bytes (22 insns) */
#include "types.h"

extern void func_80051DD8(s32 a0, s32 a1, s32 a2);
extern void func_8013A140(void);
extern void func_8004DDD8(void);

extern s32 D_800F5A2D[];
extern u8 D_800F5AAC;
extern s8 D_801474B0;

void func_801387C8(void)
{
    func_80051DD8(16, 0x80197000, 0xA3B8);
    func_8013A140();
    D_801474B0 = *(u8*)&D_800F5A2D[D_800F5AAC];
    func_8004DDD8();
}
