/* Address: 0x8005393C */
/* Size: 240 bytes (60 instructions; the .sym window's 61st word is a trailing alignment nop -- */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 girl0_basyo;
extern u8 girl1_basyo;
extern u8 girl2_basyo;
extern u8 girl_blk[];

s32 basyo_girl_get(s32 a0)
{
    u8 *p;
    s32 i;

    if (a0 == ((u32)girl0_basyo >> 4)) {
        return 0;
    }
    if (a0 == ((u32)girl1_basyo >> 4)) {
        return 1;
    }
    if (a0 == ((u32)girl2_basyo >> 4)) {
        return 2;
    }
    i = 3;
    p = girl_blk;
    do {
        if (a0 == ((u32)p[458] >> 4)) {
            return i;
        }
        if (a0 == ((u32)p[514] >> 4)) {
            return i + 1;
        }
        if (a0 == ((u32)p[570] >> 4)) {
            return i + 2;
        }
        if (a0 == ((u32)p[626] >> 4)) {
            return i + 3;
        }
        i += 4;
        p += 224;
    } while (i != 11);
    return -1;
}
