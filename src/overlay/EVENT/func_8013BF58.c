/* @SymbolStart: 0x8013BF50 */
/* @SymbolSize: 192 */
/* @RodataBase: 0x8011F7E8 */
/* Address: 0x8013BF58 */
/* Size: 192 bytes (48 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECB0;

extern void func_80100010(void);
extern void func_80100CEC(void);
extern void func_80100E30(void);
extern void func_80100F04(void);
extern void func_80100FC0(void);
extern void func_80101104(void);
extern void func_801001F4(void);
extern void func_80015FE0(void);

void func_8013BF58(void) {
    s32 selector_v = D_800EECB0;

    s32 index = selector_v - 1;

    switch (index) {
        case 0:
            func_80100010();
            return;
        case 1:
            func_80100CEC();
            return;
        case 2:
            func_80100E30();
            return;
        case 3:
            func_80100F04();
            return;
        case 4:
            func_80100FC0();
            return;
        case 5:
            func_80101104();
            return;
        case 6:
            func_801001F4();
            return;
        case 7:
        default:
            func_80015FE0();
            return;
    }
}
