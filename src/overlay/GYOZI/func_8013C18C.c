/* @SymbolStart: 0x8013C148 */
/* @SymbolSize: 124 */
#include "types.h"

extern void func_8008D610(s32 arg0, s32 arg1, s32 arg2);

extern u8 D_80148108[];
extern u8 D_8014810C[];
extern s32 D_801480FC;
extern s32 D_80148100;
extern s32 D_80148104;

extern u8 *D_800D9234;
extern u8 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;

extern s32 D_8012E66C;

void func_8013C18C(void)
{
    D_800D9234 = D_80148108;
    D_800D9238 = D_8014810C;
    D_800D923C = D_801480FC;
    D_800D9240 = D_80148100;
    D_800D9244 = D_80148104;
    func_8008D610(D_8012E66C, 1, 0);
}
