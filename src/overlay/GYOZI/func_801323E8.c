/* @SymbolStart: 0x801323A4 */
/* @SymbolSize: 124 */
#include "types.h"

extern void func_8008D610(s32 arg0, s32 arg1, s32 arg2);

extern u8 D_80145EB4[];
extern u8 D_80145EB8[];
extern s32 D_80145EA8;
extern s32 D_80145EAC;
extern s32 D_80145EB0;

extern u8 *D_800D9234;
extern u8 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;

extern s32 D_8012E66C;

void func_801323E8(void)
{
    D_800D9234 = D_80145EB4;
    D_800D9238 = D_80145EB8;
    D_800D923C = D_80145EA8;
    D_800D9240 = D_80145EAC;
    D_800D9244 = D_80145EB0;
    func_8008D610(D_8012E66C, 1, 0);
}
