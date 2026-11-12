/* @SymbolStart: 0x8013FFF0 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8011F938 */
/* Address: 0x8013FFF8 */
/* Size: 160 bytes (40 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_80104090();
extern void func_801049B4();
extern void func_80104B50();
extern void func_80104E64();
extern void func_80104F80();
extern void func_80015FE0();

void func_8013FFF8(void) {
    s32 hidden_t6_v = D_800EECB0;

    switch (hidden_t6_v - 1) {
        case 0: func_80104090(); break;
        case 1: func_801049B4(); break;
        case 2: func_80104B50(); break;
        case 3: func_80104E64(); break;
        case 4: func_80104F80(); break;
        default: func_80015FE0(); break;
    }
}
