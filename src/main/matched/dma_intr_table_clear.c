/* Function: dma_intr_table_clear */
/* Address: 0x800B4730 */
/* Size: 40 bytes (10 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-earliest */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

void dma_intr_table_clear(u32 *ptr, s32 count)
{
    s32 i;

    for (i = count - 1; i != -1; i--) {
        *ptr++ = 0;
    }
}
