/* @SymbolStart: 0x801565EC */
/* @SymbolSize: 64 */
/* Address: 0x801565F4 */
/* Size: 64 bytes (16 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1746;

extern void func_8004A734(void);
extern void func_8004A764(void);

void func_801565F4(void) {
    u8 hidden_t6_v = D_800B1746;

    if (hidden_t6_v == 9) {
        func_8004A734();
    } else {
        func_8004A764();
    }
}
