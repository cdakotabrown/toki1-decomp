/* Address: 0x800A3EB8, Size: 404 bytes (101 instructions) */
/* Region: Library (Psy-Q GCC 2.6.0, NOT IDO) */
/* @Compiler: gcc260 */
/* @CompileFlags: --fill-epilogue --mflo-nops-first --no-delay-slots --mflo-hoist-skip=5,7 --load-base-nop */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

extern s32 rcos(s32);
extern s32 rsin(s32);

MATRIX *RotMatrixZ_C(s32 r, MATRIX *m)
{
    s16 tmp[3][3];
    s32 pad;
    s32 c;
    s32 s;

    c = rcos((s16)r);
    s = rsin((s16)r);

    tmp[0][0] = (c * m->m[0][0] - s * m->m[1][0]) >> 12;
    tmp[0][1] = (c * m->m[0][1] - s * m->m[1][1]) >> 12;
    tmp[0][2] = (c * m->m[0][2] - s * m->m[1][2]) >> 12;
    m->m[1][0] = (s * m->m[0][0] + c * m->m[1][0]) >> 12;
    m->m[1][1] = (s * m->m[0][1] + c * m->m[1][1]) >> 12;
    m->m[1][2] = (s * m->m[0][2] + c * m->m[1][2]) >> 12;
    m->m[0][0] = tmp[0][0];
    m->m[0][1] = tmp[0][1];
    m->m[0][2] = tmp[0][2];
    return m;
}
