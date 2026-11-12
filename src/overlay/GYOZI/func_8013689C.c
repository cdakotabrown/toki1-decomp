/* @SymbolStart: 0x80136858 */
/* @SymbolSize: 124 */
#include "types.h"

extern void func_8008D610(s32 arg0, s32 arg1, s32 arg2);

extern u8 D_80147198[];
extern u8 D_8014719C[];
extern s32 D_8014718C;
extern s32 D_80147190;
extern s32 D_80147194;

extern u8 *D_800D9234;
extern u8 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;

extern s32 D_8012E66C;

void func_8013689C(void)
{
    D_800D9234 = D_80147198;
    D_800D9238 = D_8014719C;
    D_800D923C = D_8014718C;
    D_800D9240 = D_80147190;
    D_800D9244 = D_80147194;
    func_8008D610(D_8012E66C, 1, 0);
}
