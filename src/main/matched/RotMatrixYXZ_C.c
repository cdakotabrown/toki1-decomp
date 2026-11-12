/* Address: 0x800A39A0, Size: 492 bytes (123 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-delayed-branch --fill-epilogue --mflo-nops-first */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xC */
/* @PostProc: swap_instructions --offset1 0xC --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1C */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
/* directive.  Refuted: `@PostProc: maspsx --aspsx-version` (the mflo rule */
#include "types.h"

typedef struct {
    s16 vx, vy, vz, pad;
} SVECTOR;

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

extern s32 rcos(s32);
extern s32 rsin(s32);

MATRIX *RotMatrixYXZ_C(SVECTOR *r, MATRIX *m)
{
    s32 c0, c1, c2, s0, s1, s2;

    c0 = rcos(r->vx);
    c1 = rcos(r->vy);
    c2 = rcos(r->vz);
    s0 = rsin(r->vx);
    s1 = rsin(r->vy);
    s2 = rsin(r->vz);

    m->m[0][0] = ((c1 * c2) >> 12) + ((((s1 * s0) >> 12) * s2) >> 12);
    m->m[1][0] = (s2 * c0) >> 12;
    m->m[2][0] = -((s1 * c2) >> 12) + ((((c1 * s0) >> 12) * s2) >> 12);
    m->m[0][1] = -((c1 * s2) >> 12) + ((((s1 * s0) >> 12) * c2) >> 12);
    m->m[1][1] = (c2 * c0) >> 12;
    m->m[2][1] = ((s1 * s2) >> 12) + ((((c1 * s0) >> 12) * c2) >> 12);
    m->m[1][2] = -s0;
    m->m[0][2] = (s1 * c0) >> 12;
    m->m[2][2] = (c1 * c0) >> 12;
    return m;
}
