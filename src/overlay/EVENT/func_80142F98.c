/* @SymbolStart: 0x80142F90 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8011FA14 */
/* Address: 0x80142F98 */
/* Size: 160 bytes (40 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_80107030();
extern void func_80107A28();
extern void func_80107BB0();
extern void func_80107CEC();
extern void func_80107E20();
extern void func_80015FE0();

void func_80142F98(void) {
    s32 hidden_t6_v = D_800EECB0;

    switch (hidden_t6_v - 1) {
        case 0: func_80107030(); break;
        case 1: func_80107A28(); break;
        case 2: func_80107BB0(); break;
        case 3: func_80107CEC(); break;
        case 4: func_80107E20(); break;
        default: func_80015FE0(); break;
    }
}
