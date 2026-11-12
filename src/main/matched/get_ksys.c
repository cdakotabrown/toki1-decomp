/* Address: 0x8004EDC4, Size: 12 bytes (3 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 disp_string_state[];

void *get_ksys(void) {
    return (void *)disp_string_state;
}
