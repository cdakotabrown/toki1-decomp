/* @SymbolStart: 0x80155F40 */
/* @SymbolSize: 100 */
/* Address: 0x80155F48 */
/* Size: 100 bytes (25 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;
extern u8 D_8012531C;

extern void func_8004CF30();
extern void func_80011DFC();

void func_80155F48(void) {
    u8 t6_v = D_800B1746;

    s32 state;
    u8 branch;

    if (t6_v == 7) {
        state = D_800EECBC;
        if (state == 0) {
            branch = D_8012531C;
            if (branch == 1) {
                func_8004CF30();
                return;
            }
        }
    }
    func_80011DFC();
}
