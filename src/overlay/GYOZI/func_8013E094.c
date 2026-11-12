/* Address: 0x8013E094, Size: 60 bytes */
/* @SymbolStart: 0x8013E050 */
/* @SymbolSize: 128 */
#include "types.h"

extern void func_8008D610(s32 arg0, s32 arg1, s32 arg2);

extern s32 D_8012E66C;
extern s32 *D_800D9234;
extern s32 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;
extern s32 D_800D9250;
extern s32 D_800D9254;
extern s32 D_80148628;
extern s32 D_8014862C;
extern s32 D_80148630;

void func_8013E094(void)
{
    D_800D9234 = &D_800D9250;
    D_800D9238 = &D_800D9254;
    D_800D923C = D_80148628;
    D_800D9240 = D_8014862C;
    D_800D9244 = D_80148630;
    func_8008D610(D_8012E66C, 1, 0);
}
