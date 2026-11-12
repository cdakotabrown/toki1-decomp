/* @SymbolStart: 0x80143150 */
/* @SymbolSize: 136 */
/* Address: 0x80143158 */
/* Size: 136 bytes (34 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B0E43;

extern u8 D_800B0E42;
extern u16 D_80094714;
extern s16 D_80094718;

extern void func_80011DFC(void);

void func_80143158(void) {
    u8 a0_v = D_800B0E43;

    if (a0_v == 0 && D_800B0E42 == 0) {
        D_80094714 = D_80094714 + 4;
        D_80094718 = 0;
    } else if (D_800B0E42 != 0) {
        D_80094714 = D_80094714 + 2;
        D_80094718 = 0;
    }
    func_80011DFC();
}
