/* @SymbolStart: 0x80152CB4 */
/* @SymbolSize: 100 */
/* Address: 0x80152CBC */
/* Size: 100 bytes (25 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_80125304;

extern s16 D_80094730;
extern u8 D_80125308;

extern void func_80011DFC(void);

void func_80152CBC(void) {
    u8 t6_in_v = D_80125304;

    if (t6_in_v != 0) {
        D_80094730 = (s16)(D_80094730 + 16);
    }
    if (D_80125308 != 0) {
        D_80094730 = (s16)(D_80094730 + 16);
    }
    func_80011DFC();
}
