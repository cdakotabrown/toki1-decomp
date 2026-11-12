/* Address: 0x800A463C */
/* Size: 212 bytes (53 instructions)   [TRUE extent: 0x800A4634, 220 bytes / */
/* Region: Library (0x80086000+) */
/* window is `@SymbolStart: 0x800A4634` + `@SymbolSize: 220`. */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -g --fill-epilogue --label-load-nop --loc-load-nop */
/* @SymbolStart: 0x800A4634 */
/* @SymbolSize: 220 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

s32 IsIdMatrix(s16 *face) {
    s32 buf[9];
    s32 *row;
    s32 *ptr;
    s32 sum;
    s32 i, j;

    buf[0] = face[0] - 0x1000;
    buf[1] = face[1];
    buf[2] = face[2];
    buf[3] = face[3];
    buf[4] = face[4] - 0x1000;
    buf[5] = face[5];
    buf[6] = face[6];
    buf[7] = face[7];
    buf[8] = face[8] - 0x1000;

    sum = 0;
    j = 0;
    row = buf;
    do {
        i = 0;
        ptr = row;
        do {
            i++;
            if (*ptr < 0) {
                *ptr = -*ptr;
            }
            sum += *ptr;
            ptr++;
        } while (i < 3);
        j++;
        row += 3;
    } while (j < 3);

    return sum < 20;
}
