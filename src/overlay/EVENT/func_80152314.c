/* @SymbolStart: 0x8015230C */
/* @SymbolSize: 84 */
/* Address: 0x80152314 */
/* Size: 84 bytes (21 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_80125300;

extern s16 D_80094730;
extern s32 D_800EECBC;

extern void func_80011DFC(void);

void func_80152314(void) {
    u8 a0_v = D_80125300;

    if (a0_v == 0) {
        D_80094730 = D_80094730 + 1;
        func_80011DFC();
        func_80011DFC();
        D_800EECBC = 0;
    }
    func_80011DFC();
}
