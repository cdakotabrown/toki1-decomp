/* Address: 0x8004EED4 - 0x8004EEFC (40 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s16 k_sub_reset_point;

extern void k_reset(s16 arg0);

void k_sub_reset(void) {
    k_reset(k_sub_reset_point);
}
