/* @SymbolStart: 0x8013B884 */
/* @SymbolStart: 0x8013B884 */
/* @SymbolSize: 68 */
/* func_8013B888 -- GYOZI overlay, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_800BCE10(s32, s32);
extern void func_8004DE1C(void);

extern s16 D_801474A8;
extern s16 D_801474AC;
extern s32 D_800D92A0;
extern s32 D_80145C20;

void func_8013B888(void)
{
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10((s32)&D_800D92A0, (s32)&D_80145C20);
    func_8004DE1C();
}
