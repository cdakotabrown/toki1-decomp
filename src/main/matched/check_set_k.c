/* Address: 0x8004F108, Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 disp_string_state;

u8 check_set_k(void) {
    return disp_string_state;
}
