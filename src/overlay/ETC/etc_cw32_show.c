/* @SymbolStart: 0x8014980C */
/* @SymbolSize: 48 */
/* Address: 0x80149814 (cross-prologue head at 0x8014980C absorbed) */
/* Size: 48 bytes (12 instructions) */
#include "types.h"

extern void parameter_show();
extern u8 cw32_disp;

void etc_cw32_show(void)
{
    s32 t6 = cw32_disp;

    if ((t6 & 0x0080) == 0) {
        return;
    }
    parameter_show();
}
