/* Address: 0x8004B6A8 - 0x8004B6C8 (32 bytes) */
/* Region: GCC (OR encoding) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void _debug_string_show(s32 a0, s32 a1, s32 a2, s32 a3);

void debug_string_show(s32 a0, s32 a1, s32 a2) {
    _debug_string_show(a0, a1, a2, 15);
}
