/* @SymbolStart: 0x80155D2C */
/* @SymbolSize: 116 */
/* Address: 0x80155D34 */
/* Size: 116 bytes (29 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;
extern u8 D_8012531C;
extern void func_80011DFC(void);

void func_80155D34(void) {
    u8 a0_v = D_800B1746;

    if (a0_v == 7 && D_800EECBC == 0 && D_8012531C == 0) {
        func_80011DFC();
        return;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}
