/* Likely Symbol: get_g_zyotai_s (from o.bin, offset +108 bytes from 0x80051E1C) */
/* Address: 0x80051E88 */
/* Size: 224 bytes (56 instructions) */
/* Region: GCC (0x80041000 - 0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_h_yuukou(s32 arg0);
extern s32 get_h_tokimeki(s32 arg0);

s32 get_g_zyotai_s(s32 arg0)
{
    u32 val1;
    u32 val2;

    val1 = (u32)get_h_yuukou(arg0);
    if (val1 < 10U) {
        return 4;
    }

    val1 = (u32)get_h_yuukou(arg0);
    if (val1 < 30U) {
        return 3;
    }

    val1 = (u32)get_h_yuukou(arg0);
    if (val1 < 50U) {
        val2 = (u32)get_h_tokimeki(arg0);
        if (val2 < 50U) {
            return 3;
        }
        return 1;
    }

    val1 = (u32)get_h_yuukou(arg0);
    if (val1 < 80U) {
        val2 = (u32)get_h_tokimeki(arg0);
        if (val2 < 50U) {
            return 2;
        }
        return 1;
    }

    val2 = (u32)get_h_tokimeki(arg0);
    if (val2 < 50U) {
        return 0x82;
    }
    return 0x80;
}
