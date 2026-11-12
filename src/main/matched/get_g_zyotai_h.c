/* Address: 0x80051F68 */
/* Size: 148 bytes (37 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_h_yuukou(s32 arg0);
extern s32 get_h_tokimeki(s32 arg0);

s32 get_g_zyotai_h(s32 arg0)
{
    u32 result;

    result = (u32)get_h_yuukou(arg0);
    if (result < 10) {
        return 4;
    }

    result = (u32)get_h_yuukou(arg0);
    if (result < 50) {
        return 3;
    }

    result = (u32)get_h_tokimeki(arg0);
    if (result < 50) {
        return 2;
    }

    result = (u32)get_h_tokimeki(arg0);
    if (result < 80) {
        return 1;
    }

    return 0;
}
