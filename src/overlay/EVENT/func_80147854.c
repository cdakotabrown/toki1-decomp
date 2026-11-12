/* @SymbolStart: 0x80147844 */
/* @SymbolSize: 56 */
/* Address: 0x80147844 (cross-prologue head; .sym draws the start at 0x80147854) */
/* Size: 56 bytes (14 instructions) including the absorbed head */
#include "types.h"

extern u16 D_80094714;
extern u8 D_800B1746;

extern void func_80011DFC();

void func_80147854(void) {
    D_80094714 = D_80094714 + D_800B1746;
    func_80011DFC();
}
