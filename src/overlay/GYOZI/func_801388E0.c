/* Address: 0x801388E0, Size: 64 bytes */
/* @SymbolStart: 0x8013889C */
/* @SymbolSize: 132 */
/* @PostProc: pad_trailing_nop --count 1 */
#include "types.h"

extern void func_8008D610(s32 arg0, s32 arg1, s32 arg2);

extern s32 D_8012E66C;
extern s32 *D_800D9234;
extern s32 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;
extern s32 D_8014749C;
extern s32 D_801474A0;
extern s32 D_801474A4;
extern s32 D_801474A8;
extern s32 D_801474AC;

void func_801388E0(void)
{
    D_800D9234 = &D_801474A8;
    D_800D9238 = &D_801474AC;
    D_800D923C = D_8014749C;
    D_800D9240 = D_801474A0;
    D_800D9244 = D_801474A4;
    func_8008D610(D_8012E66C, 1, 1);
}
