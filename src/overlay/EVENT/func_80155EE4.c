/* @SymbolStart: 0x80155EDC */
/* @SymbolSize: 100 */
/* Address: 0x80155EE4 */
/* Size: 100 bytes (25 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern u8 D_8012531C;

extern void func_80011DFC(void);

void func_80155EE4(void) {
    u8 t6_v = D_800B1746;

    u8 branch;

    if (t6_v == 7) {

        branch = D_8012531C;
        if (branch == 0) {

            func_80011DFC();
            func_80011DFC();
            func_80011DFC();
            return;
        }
    }

    func_80011DFC();
}
