/* @SymbolStart: 0x801419FC */
/* @SymbolSize: 100 */
/* Address: 0x801419FC (absorbed cross-prologue head) .. 0x80141A24 */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 D_800B0A0A[];
extern u8 D_800B1746;
extern u8 D_800B1AF6;

extern void func_80011DFC(void);

void func_80141A24(void) {
    if (*(s16 *)&D_800B0A0A[D_800B1746 * 52] < 80) {
        D_800B1AF6 = D_800B1AF6 + 14;
    }
    func_80011DFC();
}
