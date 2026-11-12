/* @SymbolStart: 0x80156440 */
/* @SymbolSize: 100 */
/* Address: 0x80156448 */
/* Size: 100 bytes (25 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern s32 D_800EECBC;
extern u8 D_8012531C;
extern void func_8004CF30(void);
extern void func_80011DFC(void);

void func_80156448(void) {
    u8 a0_v = D_800B1746;

    if (a0_v == 10 && D_800EECBC == 0 && D_8012531C == 0) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}
