/* Address: 0x800530A8 */
/* Size: 124 bytes (31 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8  get_h_yuukou_str0[];
extern s16 girl0_yuukou[];

u8 get_h_yuukou(s32 a0) {
    s32 idx;
    s32 result;

    idx = a0 % 13;

    result = (s32)((u32)*(s16 *)((u8 *)girl0_yuukou + idx * 56) *
                   (u32)get_h_yuukou_str0[idx]) / 100;

    if (result >= 101) {
        return 100;
    }
    if (result < 0) {
        return 0;
    }
    return (u8)result;
}
