/* Address: 0x80045410 */
/* Size: 3 instructions (trivial setter) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 trans_pointer;

void change_trans_pointer(s32 a0) {
    trans_pointer = a0;
}
