/* @SymbolStart: 0x80141998 */
/* @SymbolSize: 100 */
/* Address: 0x80141998 (absorbed cross-prologue head) .. 0x801419C0 */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 D_800B0A0A[];
extern u8 D_800B1746;
extern u16 D_80094714;

extern void func_80011DFC(void);

void func_801419C0(void) {
    if (*(s16 *)&D_800B0A0A[D_800B1746 * 52] < 80) {
        D_80094714 = D_80094714 + 2;
    }
    func_80011DFC();
}
