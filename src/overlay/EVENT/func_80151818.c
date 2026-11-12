/* @SymbolStart: 0x80151810 */
/* @SymbolSize: 88 */
/* Address: 0x80151818 */
/* Size: 88 bytes (22 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_8012531C;

extern void func_800433D0();
extern void func_800469F4();
extern void func_80011DFC();

void func_80151818(void) {
    u8 hidden_t6_v = D_8012531C;

    if ((u32)hidden_t6_v < 2) {
        func_800433D0(512);
        func_800469F4(17773);
    } else {
        func_800433D0(513);
        func_800469F4(17782);
    }
    func_80011DFC();
}
