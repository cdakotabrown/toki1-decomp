/* func_80132CC0 -- GYOZI overlay, Address: 0x80132CC0, Size: 52 bytes (13 insns) */
#include "types.h"

extern void func_800BCE10(s32, s32);
extern void func_8004DE1C(void);

extern s32 D_800D92E0;
extern s32 D_801458A0;

void func_80132CC0(void)
{
    func_800BCE10((s32)&D_800D92E0, (s32)&D_801458A0);
    func_8004DE1C();
}
