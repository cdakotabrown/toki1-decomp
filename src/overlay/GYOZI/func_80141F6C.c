/* @SymbolStart: 0x80141F64 */
/* @SymbolSize: 72 */
/* func_80141F6C -- GYOZI overlay, Address: 0x80141F64, Size: 72 bytes (18 insns) */
#include "types.h"

extern void func_8004F860(s32, s32, s32, s32, s32);
extern void func_8004DE1C(void);

extern s32 D_80148B90;
extern s32 D_800F65C0;

void func_80141F6C(void)
{
    s32 temp = D_80148B90;

    func_8004F860(640, 0, 64, 128, temp);
    D_800F65C0 = 1;
    func_8004DE1C();
}
