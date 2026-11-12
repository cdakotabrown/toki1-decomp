/* Address: 0x80152E20 (cross-prologue head absorbed) .. 0x80152E4C */
/* Size: 96 bytes (24 instructions) */
/* @SymbolStart: 0x80152E20 */
/* @SymbolSize: 96 */
#include "types.h"

extern u8 D_800B0860[];
extern u8 D_800B1746;
extern u8 D_800B1AF6;

extern void func_80011DFC(void);

void func_80152E4C(void) {
    D_800B0860[D_800B1746 * 52 + 465] =
        D_800B0860[D_800B1746 * 52 + 465] + 1;
    func_80011DFC();
    D_800B1AF6 = D_800B1AF6 + 7;
}
