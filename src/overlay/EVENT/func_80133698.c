/* @SymbolStart: 0x80133690 */
/* @SymbolSize: 176 */
/* @RodataBase: 0x8011F5B8 */
/* Address: 0x80133698 */
/* Size: 176 bytes (44 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_800F7740(void);
extern void func_800F84E0(void);
extern void func_800F86C4(void);
extern void func_800F8B50(void);
extern void func_800F93FC(void);
extern void func_800F9558(void);
extern void func_80015FE0(void);

void func_80133698(void) {
    s32 selector_v = D_800EECB0;

    s32 index = selector_v - 1;

    switch (index) {
        case 0:
            func_800F7740();
            return;
        case 1:
            func_800F84E0();
            return;
        case 2:
            func_800F86C4();
            return;
        case 3:
            func_800F8B50();
            return;
        case 4:
            func_800F93FC();
            return;
        case 5:
            func_800F9558();
            return;
        default:
            func_80015FE0();
            return;
    }
}
