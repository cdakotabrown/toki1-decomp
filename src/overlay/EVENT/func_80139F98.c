/* @SymbolStart: 0x80139F90 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8011F758 */
/* Address: 0x80139F98 */
/* Size: 160 bytes (40 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_800FE030();
extern void func_800FEA38();
extern void func_800FF61C();
extern void func_800FF770();
extern void func_800FF82C();
extern void func_80015FE0();

void func_80139F98(void) {
    s32 hidden_t6_v = D_800EECB0;

    switch (hidden_t6_v - 1) {
        case 0: func_800FE030(); break;
        case 1: func_800FEA38(); break;
        case 2: func_800FF61C(); break;
        case 3: func_800FF770(); break;
        case 4: func_800FF82C(); break;
        default: func_80015FE0(); break;
    }
}
