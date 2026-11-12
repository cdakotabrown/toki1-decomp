/* @SymbolStart: 0x8013A9D0 */
/* @SymbolSize: 68 */
/* func_8013A9D4 -- GYOZI overlay, Size: 68 bytes (17 insns) */
#include "types.h"

extern void func_800BCE10(s32, s32);
extern void func_8004DE1C(void);

extern s16 D_801474A8;
extern s16 D_801474AC;
extern s32 D_800D92A0;
extern s32 D_80145BB8;

void func_8013A9D4(void)
{
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10((s32)&D_800D92A0, (s32)&D_80145BB8);
    func_8004DE1C();
}
