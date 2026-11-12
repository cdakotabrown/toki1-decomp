/* Function: _mk_normsp  (filed as func_8009E62C -- that symbol is the PROLOGUE; the real function */
/* Address: 0x8009E62C */
/* Size: 556 bytes (139 instructions) as recorded; TRUE window */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -fno-strength-reduce -fno-schedule-insns2 --delay-slot-load-nop --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @SymbolStart: 0x8009E5F4 */
/* @SymbolSize: 612 */
#include "types.h"

extern s16 GsORGOFSX;
extern s16 GsORGOFSY;

void _mk_normsp(u8 *out, u8 *info)
{
    s32 uv[16];
    s32 i;
    s32 *ptr;
    s32 x;
    s32 y;
    s32 t;
    u8 b;
    s32 m;

    y = *(s16 *)(info + 10) + *(s16 *)(info + 18) + GsORGOFSX;
    x = *(s16 *)(info + 12) + *(s16 *)(info + 20) + GsORGOFSY;

    if (*(u16 *)(info + 22) & 1) {
        uv[5] = info[9] + *(s16 *)(info + 16) - 1;
        uv[4] = uv[5];
        uv[6] = uv[7] = info[9] - 1;
    } else {
        uv[5] = info[9];
        uv[4] = uv[5];
        uv[6] = uv[7] = info[9] + *(s16 *)(info + 16);
    }

    if (*(u16 *)(info + 22) & 2) {
        uv[2] = info[8] + *(s16 *)(info + 14) - 1;
        uv[0] = uv[2];
        uv[1] = uv[3] = info[8] - 1;
    } else {
        uv[2] = info[8];
        uv[0] = uv[2];
        uv[1] = uv[3] = info[8] + *(s16 *)(info + 14);
    }

    i = 0;
    m = 255;
    ptr = uv;
    do {
        if (ptr[0] < 0) ptr[0] = 0;
        if (ptr[4] < 0) ptr[4] = 0;
        if (ptr[0] >= 256) ptr[0] = m;
        if (ptr[4] >= 256) ptr[4] = m;
        i++;
        ptr++;
    } while (i < 4);

    out[3] = info[0];
    out[0] = info[1];
    out[1] = info[2];
    out[2] = info[3];

    *(u16 *)(out + 18) = *(u16 *)(info + 4);
    t = *(u16 *)(info + 6);
    *(s16 *)(out + 4) = y;
    *(s16 *)(out + 6) = x;
    *(u16 *)(out + 10) = t;

    out[8] = (u8)uv[0];
    out[9] = (u8)uv[4];
    t = *(u16 *)(info + 14);
    *(s16 *)(out + 14) = x;
    *(s16 *)(out + 12) = y + t;

    out[16] = (u8)uv[1];
    b = uv[5];
    *(s16 *)(out + 20) = y;
    out[17] = b;
    *(s16 *)(out + 22) = x + *(u16 *)(info + 16);

    out[24] = (u8)uv[2];
    out[25] = (u8)uv[6];
    *(s16 *)(out + 28) = y + *(u16 *)(info + 14);
    *(s16 *)(out + 30) = x + *(u16 *)(info + 16);
    out[32] = (u8)uv[3];
    out[33] = (u8)uv[7];
}
