/* @SymbolStart: 0x80145F68 */
/* @SymbolSize: 44 */
/* Address: 0x80145F70 */
/* Size: 36 bytes (9 instructions) */
#include "types.h"

extern void etc_bukatu_result();

extern u8 now_sub_sub_step;

void bukatu_result_main(void)
{
    s32 t6 = now_sub_sub_step;

    if (t6 != 0) {
        return;
    }
    etc_bukatu_result();
}
