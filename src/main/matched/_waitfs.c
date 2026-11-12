/* Address: 0x800956F0 */
/* Size: 100 bytes (25 instructions) */
/* Region: Library (0x80086000+) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: rename_register_chain --chain v0:t8:t9 */
#include "types.h"

void _waitfs(void) {
    volatile s32 i;
    volatile s32 val;
    s32 tmp;

    val = 13;
    i = 0;

    while (i < 240) {
        tmp = val;
        val = (tmp << 2) - tmp;
        i = i + 1;
    }
}
