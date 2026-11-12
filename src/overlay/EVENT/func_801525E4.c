/* @SymbolStart: 0x801525DC */
/* @SymbolSize: 68 */
/* Address: 0x801525E4 */
/* Size: 68 bytes (17 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_8012531C;

extern s32 D_800EECBC;

extern void func_80118B4C(void);
extern void func_80011DFC(void);

void func_801525E4(void) {
    u8 t6_v = D_8012531C;

    if (t6_v == 5) {
        func_80118B4C();
        D_800EECBC = 0;
    } else {
        func_80011DFC();
    }
}
