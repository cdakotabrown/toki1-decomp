/* @SymbolStart: 0x80135DBC */
/* @SymbolSize: 60 */
/* Address: 0x80135DC4 (window starts at 0x80135DBC -- absorbed the */
/* Size: 60 bytes (15 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_800B1AF5;

extern void func_800F9DF8(void);
extern void func_80015FE0();

void func_80135DC4(void) {
    u8 sel = D_800B1AF5;

    if (sel == 0) {
        func_800F9DF8();
    } else {
        func_80015FE0();
    }
}
