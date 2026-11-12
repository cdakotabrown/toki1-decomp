/* Function: GetTPage */
/* Address: 0x800A29A0 */
/* Size: 212 bytes (53 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: remap_stack_slots --map 20:36 */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=2 */
#include "common.h"

extern s32 GetGraphType(void);

u16 GetTPage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 v;
    s32 result;

    result = GetGraphType();
    if (result == 1) {
        goto case1;
    }

    result = GetGraphType();
    if (result != 2) {
        goto case_else;
    }

case1:
    v = ((arg0 & 3) << 9) |
        ((arg1 & 3) << 7) |
        ((arg3 & 0x300) >> 3) |
        ((arg2 & 0x3FF) >> 6);
    goto done;

case_else:
    v = ((arg0 & 3) << 7) |
        ((arg1 & 3) << 5) |
        ((arg3 & 0x100) >> 4) |
        ((arg2 & 0x3FF) >> 6) |
        ((arg3 & 0x200) << 2);

done:
    return (u16)v;
}
