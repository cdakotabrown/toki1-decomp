/* @SymbolSize: 16 */
/* Address: 0x801430b4 */
/* Size: 16 bytes (4 insns) */
#include "types.h"

extern u8 tt_read_prev_cnt;

u8 get_tt_read_prev(void) {
    return tt_read_prev_cnt;
}
