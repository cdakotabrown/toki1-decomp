/* @SymbolSize: 84 */
/* func_80133DE8 -- GYOZI overlay, Address: 0x80133DE8, Size: 92 bytes (23 insns) */
#include "types.h"

extern void func_801343A4(void);
extern void func_80086AB0(s32 arg);

extern s32 D_800F6474;
extern s16 D_80145EB8;

void func_80133DE8(void)
{
    s32 v1;
    s32 v0;

    func_801343A4();
    if (D_80145EB8 != 3) return;
    v1 = D_800F6474;
    v0 = (u32)v1 < 1U;
    D_800F6474 = v1 + 1;
    if (v0 == 0) return;
    func_80086AB0(1280);
}
