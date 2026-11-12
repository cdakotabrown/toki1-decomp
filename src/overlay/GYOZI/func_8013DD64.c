/* @SymbolStart: 0x8013DD20 */
/* @SymbolSize: 124 */
#include "types.h"

extern void func_8008D610(s32 arg0, s32 arg1, s32 arg2);

extern u8 D_80148620[];
extern u8 D_80148624[];
extern s32 D_80148614;
extern s32 D_80148618;
extern s32 D_8014861C;

extern u8 *D_800D9234;
extern u8 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;

extern s32 D_8012E66C;

void func_8013DD64(void)
{
    D_800D9234 = D_80148620;
    D_800D9238 = D_80148624;
    D_800D923C = D_80148614;
    D_800D9240 = D_80148618;
    D_800D9244 = D_8014861C;
    func_8008D610(D_8012E66C, 1, 0);
}
