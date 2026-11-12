/* @SymbolStart: 0x8013AA5C */
/* @SymbolSize: 72 */
/* func_8013AA64 -- GYOZI overlay, Address: 0x8013AA64, Size: 64 bytes (16 insns) */
#include "types.h"

extern void func_800BCE10(s32, s32);
extern void func_8004DE1C(void);

extern s16 D_801474A8;
extern s16 D_801474AC;
extern s32 D_800D92A0;
extern s32 D_80145BC8;

void func_8013AA64(void)
{
    D_801474A8 = 6;
    D_801474AC = 0;
    func_800BCE10((s32)&D_800D92A0, (s32)&D_80145BC8);
    func_8004DE1C();
}
