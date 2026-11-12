/* Address: 0x8004F03C, Size: 16 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 get_k_speed_tbl0;

u8 get_k_speed(void) {
    return get_k_speed_tbl0;
}
