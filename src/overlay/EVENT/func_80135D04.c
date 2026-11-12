/* @SymbolStart: 0x80135CFC */
/* @SymbolSize: 192 */
/* @RodataBase: 0x8011F608 */
/* Address: 0x80135D04 */
/* Size: 192 bytes (48 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_800F9DBC(void);
extern void func_800FA584(void);
extern void func_800FB67C(void);
extern void func_800FB728(void);
extern void func_800FB7E8(void);
extern void func_800FB920(void);
extern void func_800FA664(void);
extern void func_80015FE0(void);

void func_80135D04(void) {
    s32 selector_v = D_800EECB0;

    s32 index = selector_v - 1;

    switch (index) {
        case 0:
            func_800F9DBC();
            return;
        case 1:
            func_800FA584();
            return;
        case 2:
            func_800FB67C();
            return;
        case 3:
            func_800FB728();
            return;
        case 4:
            func_800FB7E8();
            return;
        case 5:
            func_800FB920();
            return;
        case 6:
            func_800FA664();
            return;
        case 7:
        default:
            func_80015FE0();
            return;
    }
}
