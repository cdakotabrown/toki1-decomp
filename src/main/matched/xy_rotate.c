/* Function: xy_rotate */
/* Address: 0x80086424 */
/* Size: 204 bytes (51 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: remap_stack_slots --map '28:44' */
/* @PostProc: strip_trailing_instructions --nop-only */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 rsin(s32 arg);
extern s32 rcos(s32 arg);

void xy_rotate(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 sin_val, cos_val;

    sin_val = rsin(arg0);
    cos_val = rcos(arg0);

    *arg3 = (s32)((u32)cos_val * (u32)*arg1) / 4096 - (s32)((u32)*arg2 * (u32)sin_val) / 4096;
    *arg4 = (s32)((u32)sin_val * (u32)*arg1) / 4096 + (s32)((u32)*arg2 * (u32)cos_val) / 4096;
}
