/* @SymbolStart: 0x801562C4 */
/* @SymbolSize: 100 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=1 */
/* Address: 0x801562CC */
/* Size: 100 bytes (25 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;
extern u8 D_8012531C;

extern void func_8004CF30(void);
extern void func_80011DFC(void);

void func_801562CC(void) {
    u8 a0_v = D_800B1746;

    if (a0_v == 10) {
        if (D_800EECBC == 1) {
            if (D_8012531C == 1) {
                func_8004CF30();
                return;
            }
        }
    }
    func_80011DFC();
}
