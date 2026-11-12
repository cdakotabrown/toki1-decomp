/* @SymbolStart: 0x80146330 */
/* @SymbolSize: 160 */
/* @RodataBase: 0x8011FB4C */
/* Address: 0x80146338 */
/* Size: 160 bytes (40 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_8010A3D0();
extern void func_8010A430();
extern void func_8010AD70();
extern void func_8010A490();
extern void func_8010B29C();
extern void func_80015FE0();

void func_80146338(void) {
    s32 hidden_t6_v = D_800EECB0;

    switch (hidden_t6_v - 1) {
        case 0: func_8010A3D0(); break;
        case 1: func_8010A430(); break;
        case 2: func_8010AD70(); break;
        case 3: func_8010A490(); break;
        case 4: func_8010B29C(); break;
        default: func_80015FE0(); break;
    }
}
