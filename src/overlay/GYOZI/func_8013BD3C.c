/* @SymbolStart: 0x8013BD38 */
/* @SymbolStart: 0x8013BD38 */
/* @SymbolSize: 68 */
/* func_8013BD3C -- GYOZI overlay, Size: 60 bytes (15 insns) */
#include "types.h"

extern void func_800BCE10(s32, s32);
extern void func_8004DE1C(void);

extern s16 D_801474A8;
extern s16 D_801474AC;
extern s32 D_800D92A0;
extern s32 D_80145C40;

void func_8013BD3C(void)
{
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10((s32)&D_800D92A0, (s32)&D_80145C40);
    func_8004DE1C();
}
