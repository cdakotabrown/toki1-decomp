/* @SymbolStart: 0x80155E2C */
/* @SymbolSize: 60 */
/* Address: 0x80155E34 */
/* Size: 60 bytes (15 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern u8 D_8012531C;

extern void func_8004A7F4(void);
extern void func_80118B4C(void);

void func_80155E34(void) {
    u8 t6_param_v = D_8012531C;

    if (t6_param_v == 0) {
        func_8004A7F4();
    } else {
        func_80118B4C();
    }
}
