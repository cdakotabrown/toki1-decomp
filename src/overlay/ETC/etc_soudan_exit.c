/* @PostProc: pad_trailing_nop --count 3 */
/* Address: 0x8014C224 */
/* Size: 44 bytes (11 instructions) */
#include "types.h"

extern void return_schedule();

void etc_soudan_exit(void)
{
    return_schedule(1);
}
