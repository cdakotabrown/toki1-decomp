/* @SymbolStart: 0x80135B7C */
/* @SymbolSize: 112 */
/* func_80135B90 - GYOZI overlay, Address: 0x80135B90, Size: 92 bytes (23 insns) */
#include "types.h"

extern void func_801372B0(void);
extern void func_8004DE1C(void);

extern s32 D_8012E66C;
extern u8 D_80146364;
extern s16 D_800D9248;
extern s16 D_800D924C;
extern s32 D_800D9258;
extern s32 D_800D925C;
extern s32 D_800D9260;
extern s32 D_80146224;
extern s32 D_80146258;
extern s32 D_8014628C;

void func_80135B90(void)
{
    s32 a;
    s32 b;
    s32 c;

    D_80146364 = D_8012E66C;
    D_800D9248 = 0;
    D_800D924C = 0;
    func_801372B0();
    a = D_80146224;
    b = D_80146258;
    c = D_8014628C;
    D_800D9258 = a;
    D_800D925C = b;
    D_800D9260 = c;
    func_8004DE1C();
}
