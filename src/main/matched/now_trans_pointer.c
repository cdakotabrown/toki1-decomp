/* Address: 0x80045400, Size: 16 bytes (4 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern s32 trans_pointer;

s32 now_trans_pointer(void) {
    return trans_pointer;
}
