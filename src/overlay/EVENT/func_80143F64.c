/* Address: 0x80143F64 (true start 0x80143F5C: the `li t6,15; lui at,0x800b` */
/* Size: 52 bytes (13 instructions) */
/* @SymbolStart: 0x80143F5C */
/* @SymbolSize: 52 */
#include "types.h"

extern u8 D_800B1746;
extern s16 D_800EAFB6;

extern void func_80011DFC(void);

void func_80143F64(void) {
    D_800B1746 = 15;
    D_800EAFB6 = 4;
    func_80011DFC();
}
