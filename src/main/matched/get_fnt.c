/* Address: 0x8004DA18 */
/* Size: 1036 bytes (259 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* @CompileFlags: -cckr -Wc,-pic0 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern void memcpy();

get_fnt(dest, src, srcOff, divisor, count, flags)
u8 *dest;
u8 *src;
s32 srcOff;
s32 divisor;
s32 count;
u8 flags;
{
    s32 pa0;
    s32 pa1;
    s32 pa2;
    s32 pa3;
    s32 pa4;
    u8 localBuf[256];
    s32 pb0;
    s32 pb1;
    s32 i;
    s32 j;
    s32 acc;
    s32 rowSize;
    s32 step;
    s32 srcBase;
    u8 *srcPtr;

    j = 0;
    if (count > 0) {
        acc = 0;
        step = 128 / count;
        rowSize = count / divisor;
        srcPtr = src + (srcOff % step) * count / divisor;
        srcBase = (srcOff / step) * count;
        do {
            memcpy(dest + acc / divisor,
                          (u8 *)((u32)srcPtr + ((srcBase + j) << 7) / divisor), rowSize);
            j++;
            acc += count;
        } while (j != count);
    }
    if (flags & 1) {
        memcpy(localBuf, dest, 256);
        for (i = 0; i < count; i++) {
            memcpy(dest + (count - 1 - i) * count, localBuf + i * count, count);
        }
    }
    if (flags & 2) {
        memcpy(localBuf, dest, 256);
        for (i = 0; i < count; i++) {
            dest[i * count + 4] = localBuf[count * i + 3];
            dest[i * count + 5] = localBuf[count * i + 2];
            dest[i * count + 6] = localBuf[count * i + 1];
            dest[i * count + 7] = localBuf[count * i + 0];
            dest[i * count + 0] = localBuf[count * i + 7];
            dest[i * count + 1] = localBuf[count * i + 6];
            dest[i * count + 2] = localBuf[count * i + 5];
            dest[i * count + 3] = localBuf[count * i + 4];
        }
    }
}
