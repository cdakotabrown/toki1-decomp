/* Address: 0x8004F014, Size: 40 bytes (10 instructions) */
/* @CompileFlags: -cckr */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 get_k_speed_tbl0;

void k_speed_set(a)
u8 a;
{
    get_k_speed_tbl0 = a;
    if (a >= 0x81) {
        get_k_speed_tbl0 = 0x7F;
    }
}
