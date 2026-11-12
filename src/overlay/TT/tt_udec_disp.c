/* @SymbolSize: 284 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* Function: tt_udec_disp */
/* Overlay: TT */
#include "types.h"

extern s32 tt_str_disp();

void tt_udec_disp(s32 a0, s16 a1, u32 val, s32 a3)
{
    u8 buf[24];
    u8 *p;
    s32 v0;
    s32 v1;
    s32 ten;

    ten = 10;
    a3 = a3 & 0xFF;
    v1 = 1;
    p = buf;
    if (a3 == 0) {
        goto set_ten;
    }
    if ((u32)a3 < 11U) {
        goto cc4;
    }
set_ten:
    a3 = 10;
cc4:
    v0 = 1;
    while ((u32)v0 < (u32)a3) {
        v1 = v1 * ten;
        v0 = v0 + 1;
    }
    val = val % (u32)(v1 * ten);
    v0 = 0;
    if (a3 != 0) {
        do {
            v0 = v0 + 1;
            p = p + 1;
            p[-1] = (u8)(val / (u32)v1 + 48);
            val = val % (u32)v1;
            v1 = v1 / ten;
        } while ((u32)v0 < (u32)a3);
    }
    *p = 0;
    tt_str_disp((s16)a0, a1, buf);
}
