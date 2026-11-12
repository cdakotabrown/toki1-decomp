/* Address: 0x8013CDD8 (absorbed head)  Size: 88 bytes (22 instructions) */
/* @SymbolStart: 0x8013CDD8 */
/* @SymbolSize: 88 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
#include "types.h"

extern u16 D_80094714;
extern s32 D_800EECBC;
extern s32 D_80121D5C;

extern void func_80012D64();
extern void func_80011DFC();

void func_8013CDE8(void) {
    s32 val;
    u16 acc;

    acc = D_80094714;
    val = D_800EECBC;
    D_80094714 = (u16)(acc + val);
    func_80012D64(D_80121D5C, 17, 1, 2, val);
    func_80011DFC();
}
