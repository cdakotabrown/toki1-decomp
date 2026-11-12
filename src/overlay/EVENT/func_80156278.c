/* @SymbolStart: 0x80156270 */
/* @SymbolSize: 84 */
/* Address: 0x80156278 */
/* Size: 84 bytes (21 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;

extern void func_8004CF30();
extern void func_80011DFC();

void func_80156278(void) {
    u8 t6_param_v = D_800B1746;

    if (t6_param_v == 9) {
        if (D_800EECBC == 2) {
            func_8004CF30();
            return;
        }
    }
    func_80011DFC();
}
