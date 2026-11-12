/* Function: _mk_xpndsp */
/* Address: 0x8009E330 */
/* Size: 708 bytes (177 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 (Sony LIBGS 2D_COM0 member) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: maspsx --aspsx-version=2.67 */
#include "types.h"

extern s16 GsORGOFSX;
extern s16 GsORGOFSY;

typedef struct {
    s16 u;
    s16 pad0;
    s16 v;
    s16 pad1;
    s32 pad2;
    s32 pad3;
} CoordIO;

typedef struct {
    s16 u;
    s16 v;
    s16 flag;
    s16 pad;
} CoordIn;

extern void ApplyMatrix(void *, CoordIn *, CoordIO *);

void _mk_xpndsp(u8 *arg0, u8 *arg1, void *arg2) {
    CoordIn coordIn;
    CoordIO coordOut;
    s32 flip_h0, flip_h1, flip_h2, flip_h3;
    s32 flip_v0, flip_v1, flip_v2, flip_v3;
    s32 pad1, pad2, pad3, pad4, pad5, pad6, pad7, pad8;
    CoordIO *s0ptr;
    s32 xoff, yoff;
    s32 v0;

    (void)&flip_h0; (void)&flip_h1; (void)&flip_h2; (void)&flip_h3;
    (void)&flip_v0; (void)&flip_v1; (void)&flip_v2; (void)&flip_v3;
    (void)&pad1; (void)&pad2; (void)&pad3; (void)&pad4;
    (void)&pad5; (void)&pad6; (void)&pad7; (void)&pad8;

    xoff = *(s16 *)(arg1 + 10) + GsORGOFSX;
    yoff = *(s16 *)(arg1 + 12) + GsORGOFSY;

    if (*(u16 *)(arg1 + 22) & 1) {
        v0 = arg1[9] + *(s16 *)(arg1 + 16) - 1;
        flip_v1 = v0;
        flip_v0 = v0;
        v0 = arg1[9];
        flip_v3 = v0;
    } else {
        v0 = arg1[9];
        flip_v1 = v0;
        flip_v0 = v0;
        v0 = arg1[9] + *(s16 *)(arg1 + 16) - 1;
        flip_v3 = v0;
    }
    flip_v2 = v0;

    if (*(u16 *)(arg1 + 22) & 2) {
        v0 = arg1[8] + *(s16 *)(arg1 + 14) - 1;
        flip_h2 = v0;
        flip_h0 = v0;
        v0 = arg1[8];
        flip_h3 = v0;
    } else {
        v0 = arg1[8];
        flip_h2 = v0;
        flip_h0 = v0;
        v0 = arg1[8] + *(s16 *)(arg1 + 14) - 1;
        flip_h3 = v0;
    }
    flip_h1 = v0;

    s0ptr = &coordOut;
    arg0[3] = arg1[0];
    arg0[0] = arg1[1];
    arg0[1] = arg1[2];
    arg0[2] = arg1[3];
    *(u16 *)(arg0 + 18) = *(u16 *)(arg1 + 4);
    *(u16 *)(arg0 + 10) = *(u16 *)(arg1 + 6);

    coordIn.u = *(u16 *)(arg1 + 18);
    coordIn.v = *(u16 *)(arg1 + 20);
    coordIn.flag = 0;
    ApplyMatrix(arg2, &coordIn, s0ptr);
    *(s16 *)(arg0 + 4) = xoff + coordOut.u;
    *(s16 *)(arg0 + 6) = yoff + coordOut.v;
    arg0[8] = flip_h0;
    arg0[9] = flip_v0;

    coordIn.u = *(u16 *)(arg1 + 18) + *(u16 *)(arg1 + 14);
    ApplyMatrix(arg2, &coordIn, s0ptr);
    *(s16 *)(arg0 + 12) = xoff + coordOut.u;
    *(s16 *)(arg0 + 14) = yoff + coordOut.v;
    arg0[16] = flip_h1;
    arg0[17] = flip_v1;

    coordIn.v = *(u16 *)(arg1 + 20) + *(u16 *)(arg1 + 16);
    ApplyMatrix(arg2, &coordIn, s0ptr);
    *(s16 *)(arg0 + 28) = xoff + coordOut.u;
    *(s16 *)(arg0 + 30) = yoff + coordOut.v;
    arg0[32] = flip_h3;
    arg0[33] = flip_v3;

    coordIn.u = *(u16 *)(arg1 + 18);
    ApplyMatrix(arg2, &coordIn, s0ptr);
    *(s16 *)(arg0 + 20) = xoff + coordOut.u;
    *(s16 *)(arg0 + 22) = yoff + coordOut.v;
    arg0[24] = flip_h2;
    arg0[25] = flip_v2;
}
