/* @SymbolStart: 0x8013E7E0 */
/* @SymbolSize: 176 */
/* @RodataBase: 0x8011F8DC */
/* Address: 0x8013E7E8 */
/* Size: 176 bytes (44 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_80102890(void);
extern void func_801028F0(void);
extern void func_801034E8(void);
extern void func_801035BC(void);
extern void func_801039E8(void);
extern void func_80103ABC(void);
extern void func_80015FE0(void);

void func_8013E7E8(void) {
    s32 t6_v = D_800EECB0;

    switch (t6_v) {
        case 1: func_80102890(); break;
        case 2: func_801028F0(); break;
        case 3: func_801034E8(); break;
        case 4: func_801035BC(); break;
        case 5: func_801039E8(); break;
        case 6: func_80103ABC(); break;
        default: func_80015FE0(); break;
    }
}
