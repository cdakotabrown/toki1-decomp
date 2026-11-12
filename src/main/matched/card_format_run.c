/* Debug Symbol: card_format_run (from o.bin, high match) */
/* Address: 0x800544F8 */
/* Size: 56 bytes (14 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 format(void *arg0);

extern u8 card_format_run_str0;

s32 card_format_run(void)
{
    if (format(&card_format_run_str0) == 1) {
        return 1;
    }
    return 0;
}
