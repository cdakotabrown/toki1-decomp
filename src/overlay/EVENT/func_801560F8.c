/* @SymbolStart: 0x801560F0 */
/* @SymbolSize: 84 */
/* Address: 0x801560F8 */
/* Size: 84 bytes (21 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern u8 D_800B0BC6;
extern u8 D_800B1AF6;

extern void func_80011DFC();

void func_801560F8(void) {
    u8 t6_param_v = D_800B1746;

    if (t6_param_v == 8) {
        if (D_800B0BC6 != 0) {
            D_800B1AF6 = D_800B1AF6 + 25;
        }
    }
    func_80011DFC();
}
