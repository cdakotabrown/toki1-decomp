/* @SymbolStart: 0x80136664 */
/* @SymbolSize: 60 */
/* Address: 0x8013666C */
/* Size: 60 bytes (15 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1AF5;

extern void func_800FA6A0(void);
extern void func_80015FE0();

void func_8013666C(void) {
    u8 hidden_t6_v = D_800B1AF5;

    if (hidden_t6_v == 0) {
        func_800FA6A0();
    } else {
        func_80015FE0();
    }
}
