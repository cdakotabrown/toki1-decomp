/* @SymbolStart: 0x8013BA10 */
/* @SymbolSize: 60 */
/* Address: 0x8013BA18 */
/* Size: 60 bytes (15 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern void func_8004A734(void);
extern void func_8004A764(void);

void func_8013BA18(void) {
    s32 hidden_t6_v = D_800EECBC;

    if (hidden_t6_v == 0) {
        func_8004A734();
    } else {
        func_8004A764();
    }
}
