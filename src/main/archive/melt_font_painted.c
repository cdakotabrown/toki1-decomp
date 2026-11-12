/* Address: 0x8004F91C */
/* Size: 432 bytes (108 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_func_8004F91C.py */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=32 IDO52_LDA_NOINREG_CLASSES=1 */
#include "types.h"

extern u8 melt_font_tbl0[];

void func_8004F91C(s32 arg0, s32 *arg1) {
    u8 bits[144];
    u8 *src;
    u8 *dst;
    u8 *end;
    s32 shift;
    s32 i;

    src = &melt_font_tbl0[((arg0 << 3) + arg0) << 1];
    dst = bits;
    end = bits + 144;

    do {
        dst += 8;
        dst[-8] = ((u32)*src & 0x80) >> 7;
        dst[-7] = ((u32)*src & 0x40) >> 6;
        dst[-6] = ((u32)*src & 0x20) >> 5;
        dst[-5] = ((u32)*src & 0x10) >> 4;
        dst[-4] = ((u32)*src & 0x08) >> 3;
        dst[-3] = ((u32)*src & 0x04) >> 2;
        dst[-2] = ((u32)*src & 0x02) >> 1;
        dst[-1] = *src & 0x01;
        src++;
    } while ((u32)dst < (u32)end);

    i = 0;
    dst = bits;

    do {
        *arg1 = 0;
        shift = 0;
        do {
            *arg1 += *dst << shift;
            shift += 4;
            dst++;
        } while (shift < 32);
        arg1++;

        *arg1 = 0;
        shift = 0;
        do {
            *arg1 += *dst << shift;
            shift += 4;
            dst++;
        } while (shift < 16);
        arg1++;

        *arg1 = 0;
        shift = 0;
        do {
            *arg1 += *dst << shift;
            shift += 4;
            dst++;
        } while (shift < 32);

        arg1[1] = 0;
        arg1[1] = *dst;
        arg1[1] = (dst[1] << 4) + arg1[1];
        arg1[1] = (dst[2] << 8) + arg1[1];
        arg1[1] = (dst[3] << 12) + arg1[1];
        i++;
        dst += 4;
        arg1 += 2;
    } while (i != 6);
}
