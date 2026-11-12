/* @SymbolStart: 0x801443E0 */
/* @SymbolSize: 192 */
/* @RodataBase: 0x8011FAA8 */
/* Address: 0x801443E8 */
/* Size: 192 bytes (48 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_801084A0(void);
extern void func_80108500(void);
extern void func_80109E14(void);
extern void func_80109EE8(void);
extern void func_8010A008(void);
extern void func_8010A0E4(void);
extern void func_80108560(void);
extern void func_80015FE0(void);

void func_801443E8(void) {
    s32 selector_v = D_800EECB0;

    s32 index = selector_v - 1;

    switch (index) {
        case 0:
            func_801084A0();
            return;
        case 1:
            func_80108500();
            return;
        case 2:
            func_80109E14();
            return;
        case 3:
            func_80109EE8();
            return;
        case 4:
            func_8010A008();
            return;
        case 5:
            func_8010A0E4();
            return;
        case 6:
            func_80108560();
            return;
        default:
            func_80015FE0();
            return;
    }
}
