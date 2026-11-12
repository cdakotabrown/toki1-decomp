/* Address: 0x8008E55C */
/* Size: 192 bytes (48 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: scheduler_normalizer */
/* @PostProc: custom_script custom_func_8008E55C.py */
#include "types.h"

extern s32 SpuVmKeyOn(s32, s16, s16, u16, u16, u16);

s32 SpuVmSeKeyOn(s16 a0, s16 a1, u16 a2, s32 a3, u16 arg4, u16 arg5)
{
    s16 p0 = a0;
    s16 p1 = a1;
    u16 p2 = a2;
    u16 divisor;
    u16 result;

    if (arg4 == arg5) {
        result = 64;
        divisor = arg4;
    } else if (arg5 < arg4) {
        divisor = arg4;
        result = (u16)((u32)(arg5 << 6) / arg4);
    } else {
        divisor = arg5;
        result = (u16)(127 - (u32)(arg4 << 6) / arg5);
    }

    return SpuVmKeyOn(32, p0, p1, p2, divisor, result);
}
