/* @SymbolStart: 0x80137F40 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8011F6B8 */
/* Address: 0x80137F48 */
/* Size: 160 bytes (40 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_800FBFE0();
extern void func_800FCD0C();
extern void func_800FCDD8();
extern void func_800FD00C();
extern void func_800FD1C0();
extern void func_80015FE0();

void func_80137F48(void) {
    s32 hidden_t6_v = D_800EECB0;

    switch (hidden_t6_v - 1) {
        case 0: func_800FBFE0(); break;
        case 1: func_800FCD0C(); break;
        case 2: func_800FCDD8(); break;
        case 3: func_800FD00C(); break;
        case 4: func_800FD1C0(); break;
        default: func_80015FE0(); break;
    }
}
