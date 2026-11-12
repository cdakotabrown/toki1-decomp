/* Address: 0x80047890, Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 don_timer;

void don_init(void) {
    don_timer = 4;
}
