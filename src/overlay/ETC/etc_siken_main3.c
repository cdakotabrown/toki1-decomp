/* @SymbolStart: 0x8014AADC */
/* @SymbolSize: 44 */
/* Address: 0x8014AAE4 */
/* Size: 36 bytes (9 instructions) */
#include "types.h"

extern void etc_sound_main0();

extern u8 now_sub_sub_step;

void etc_siken_main3(void)
{
    s32 t6 = now_sub_sub_step;

    if (t6 != 0) {
        return;
    }
    etc_sound_main0();
}
