/* @SymbolStart: 0x80141AC8 */
/* @SymbolSize: 56 */
/* Address: 0x80141AC8 (absorbed cross-prologue head) .. 0x80141AD8 */
/* Size: 56 bytes (14 instructions) */
#include "types.h"

extern u16 D_80094714;
extern s32 D_800EECBC;

extern void func_80011DFC(void);

void func_80141AD8(void) {
    D_80094714 = D_80094714 + D_800EECBC;
    func_80011DFC();
}
