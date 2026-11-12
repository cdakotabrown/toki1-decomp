/* Address: 0x8014554C */
/* Size: 180 bytes (45 insns) */
#include "types.h"

extern void taco_grad_disp(s32 a0, s32 a1, s32 a2, s32 a3,
                          s32 s16, s32 s20, s32 s24);

void taco_grad_band_disp(s32 a0, s32 a1, s32 a2, s32 a3, s32 kind, s32 ctx)
{
    if (kind >= 0 && kind < 31) {
        taco_grad_disp(a0, a1, a2, a3, (kind & 0xFF) << 3, 0, ctx);
    } else if (kind >= 31 && kind < 35) {
        taco_grad_disp(a0, a1, a2, a3, 0, 2, ctx);
    } else if (kind >= 35 && kind < 97) {
        taco_grad_disp(a0, a1, a2, a3, ((kind - 35) & 0xFF) << 2, 1, ctx);
    }
}
