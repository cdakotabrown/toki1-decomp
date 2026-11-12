/* Address: 0x80058098 */
/* Size: 12 bytes (3 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 bg_change_tbl2;

void set_tbg_bri(u8 val) {
    bg_change_tbl2 = val;
}
