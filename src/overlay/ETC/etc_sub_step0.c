/* @SymbolStart: 0x8013FF00 */
/* @SymbolSize: 44 */
/* Address: 0x8013FF00 (includes the 2-word cross-prologue head at 0x8013FF00) */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern u8 now_sub_sub_step;

extern void etc_init();

void etc_sub_step0(void)
{
    if (now_sub_sub_step != 0) {
        return;
    }
    etc_init();
}
