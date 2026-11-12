/* @SymbolStart: 0x8013D510 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8011F860 */
/* Address: 0x8013D518 */
/* Size: 160 bytes (40 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_801015B0();
extern void func_80101F24();
extern void func_80102118();
extern void func_801021FC();
extern void func_801022C0();
extern void func_80015FE0();

void func_8013D518(void) {
    s32 hidden_t6_v = D_800EECB0;

    switch (hidden_t6_v - 1) {
        case 0: func_801015B0(); break;
        case 1: func_80101F24(); break;
        case 2: func_80102118(); break;
        case 3: func_801021FC(); break;
        case 4: func_801022C0(); break;
        default: func_80015FE0(); break;
    }
}
