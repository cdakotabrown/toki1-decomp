/* Function: debug_number_show */
/* Address: 0x8004B198 */
/* Size: 44 bytes (11 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern void _debug_number_show(s32 a0, s32 a1, s32 a2, s32 a3);
extern u8 _debug_number_show_str0;

void debug_number_show(s32 a0, s32 a1, s32 a2) {
    u8 flag = _debug_number_show_str0;

    if (flag != 0) {
        _debug_number_show(a0, a1, a2, 15);
    }
}
