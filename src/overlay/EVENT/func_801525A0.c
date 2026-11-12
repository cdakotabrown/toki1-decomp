/* @SymbolStart: 0x80152598 */
/* Address: 0x801525A0 */
/* Size: 68 bytes (17 instructions) */
/* @SymbolSize: 68 */
#include "types.h"

extern s16 D_800B0956;

extern s16 D_80094730;

extern void func_80011DFC(void);

void func_801525A0(void) {
    s16 t6_v = D_800B0956;

    if (t6_v < 120) {
        D_80094730 = D_80094730 + 2;
    }
    func_80011DFC();
}
