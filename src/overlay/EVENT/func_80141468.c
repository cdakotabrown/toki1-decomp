/* @SymbolStart: 0x80141460 */
/* @SymbolSize: 176 */
/* @RodataBase: 0x8011F99C */
/* Address: 0x80141468 */
/* Size: 176 bytes (44 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_80105510(void);
extern void func_80105570(void);
extern void func_80106834(void);
extern void func_80106908(void);
extern void func_80106A8C(void);
extern void func_80106B48(void);
extern void func_80015FE0(void);

void func_80141468(void) {
    s32 selector_v = D_800EECB0;

    s32 index = selector_v - 1;

    switch (index) {
        case 0:
            func_80105510();
            return;
        case 1:
            func_80105570();
            return;
        case 2:
            func_80106834();
            return;
        case 3:
            func_80106908();
            return;
        case 4:
            func_80106A8C();
            return;
        case 5:
            func_80106B48();
            return;
        default:
            func_80015FE0();
            return;
    }
}
