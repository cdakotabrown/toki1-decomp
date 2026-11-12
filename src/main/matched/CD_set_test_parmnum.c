/* Address: 0x8008B12C */
/* Size: 12 bytes (3 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 libcd_test_param_count;

void CD_set_test_parmnum(s32 val) {
    libcd_test_param_count = val;
}
