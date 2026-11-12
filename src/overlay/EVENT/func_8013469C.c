/* @SymbolStart: 0x8013468C */
/* @SymbolSize: 56 */
/* Address: 0x8013468C (absorbed cross-prologue head) .. 0x8013469C */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern u16 D_80094714;
extern s32 D_800EECBC;

extern void func_80011DFC(void);

void func_8013469C(void) {
    D_80094714 = D_80094714 + D_800EECBC;
    func_80011DFC();
}
