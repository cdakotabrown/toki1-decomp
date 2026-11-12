/* @SymbolStart: 0x80141A60 */
/* @SymbolSize: 104 */
/* Address: 0x80141A60 (absorbed cross-prologue head) .. 0x80141A88 */
/* Size: 104 bytes (26 instructions) */
#include "types.h"

extern u8 D_800B0A0A[];
extern u8 D_800B1746;

extern void func_80011DFC(void);

void func_80141A88(void) {
    if (*(s16 *)&D_800B0A0A[D_800B1746 * 52] >= 80) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}
