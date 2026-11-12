/* Address: 0x8004F0DC */
/* Size: 9 instructions */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s16 check_end_k_tbl0;
extern s16 check_end_k_tbl1;

s32 check_end_k(void) {
    s16 a;
    s16 b;

    a = check_end_k_tbl1;
    b = check_end_k_tbl0;

    if (a == b) {
        return 1;
    }
    return 0;
}
