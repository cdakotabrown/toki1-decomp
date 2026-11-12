/* Address: 0x80063A88 */
/* Size: 72 bytes (18 instructions; the last is the jr ra delay-slot nop) */
/* Region: Game */
/* @SymbolSize: 72 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 screen_w[];

void parameter_change(s32 slot, s32 delta) {
    u8 *p;

    p = &screen_w[slot * 4];
    *(s32 *)&p[252] += delta;
    if (*(s32 *)&p[252] < 0) {
        *(s32 *)&p[252] = 0;
    }
    if (*(s16 *)&p[254] >= 1000) {
        *(s16 *)&p[254] = 999;
    }
}
