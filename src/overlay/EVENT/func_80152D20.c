/* @SymbolStart: 0x80152D18 */
/* @SymbolSize: 168 */
/* Address: 0x80152D20 */
/* Size: 168 bytes (42 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern s32 D_800EECC8;
extern u8 D_80125310;
extern u8 D_800B1AF6;
extern u8 D_800B1746;
extern u8 D_800B0A31;
extern s16 D_8012437C;

extern void func_80011DFC();

void func_80152D20(void) {
    s32 arg0_v = D_800EECBC;

    if (arg0_v == 1) {
        D_800EECBC = D_800EECC8;
        D_80125310 = 0;
        func_80011DFC();
        D_800B1AF6 = D_800B1AF6 + 12;
    } else {
        if ((&D_800B0A31)[D_800B1746 * 52] != 0) {
            D_8012437C = D_8012437C + 1;
        }
        func_80011DFC();
    }
}
