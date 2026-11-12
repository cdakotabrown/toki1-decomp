/* @SymbolStart: 0x80156FBC */
/* @SymbolSize: 64 */
/* Address: 0x80156FC4 */
/* Size: 64 bytes (16 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B0F2F;

extern void func_8004A2C4(void);
extern void func_8004A4D8(void);

void func_80156FC4(void) {
    u8 hidden_t6_v = D_800B0F2F;

    if (hidden_t6_v != 255) {
        func_8004A2C4();
    } else {
        func_8004A4D8();
    }
}
