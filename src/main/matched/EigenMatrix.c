/* Address: 0x800A41C0 */
/* Size: 1140 bytes (285 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -g -fno-strength-reduce --mflo-nops-first --label-load-nop --loc-load-nop --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xfc */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0xfc */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0x100 */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x27c */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

typedef struct {
    s32 vx, vy, vz, pad;
} VECTOR;

extern void OuterProduct12(VECTOR *a, VECTOR *b, VECTOR *c);
extern void vector_normalize(VECTOR *v0, VECTOR *v1);

void EigenMatrix(MATRIX *m, MATRIX *t)
{
    MATRIX a;
    s32 c[9];
    VECTOR v1, v2, v3, n0, n1, n2;
    s32 i, j, k, l, idx, max;

    a.m[0][0] = m->m[0][0] - 4096;
    a.m[0][1] = m->m[0][1];
    a.m[0][2] = m->m[0][2];
    a.m[1][0] = m->m[1][0];
    a.m[1][1] = m->m[1][1] - 4096;
    a.m[1][2] = m->m[1][2];
    a.m[2][0] = m->m[2][0];
    a.m[2][1] = m->m[2][1];
    a.m[2][2] = m->m[2][2] - 4096;

    c[0] = a.m[1][1] * a.m[2][2] - a.m[1][2] * a.m[2][1];
    c[1] = a.m[1][0] * a.m[2][2] - a.m[0][2] * a.m[2][0];
    c[2] = a.m[1][0] * a.m[2][1] - a.m[1][1] * a.m[2][0];
    c[3] = a.m[0][1] * a.m[2][2] - a.m[0][2] * a.m[2][1];
    c[4] = a.m[0][0] * a.m[2][2] - a.m[0][2] * a.m[2][0];
    c[5] = a.m[0][0] * a.m[2][1] - a.m[0][1] * a.m[2][0];
    c[6] = a.m[0][1] * a.m[2][2] - a.m[0][2] * a.m[2][1];
    c[7] = a.m[0][0] * a.m[1][2] - a.m[0][2] * a.m[1][0];
    c[8] = a.m[0][0] * a.m[1][1] - a.m[0][1] * a.m[1][0];

    for (i = 0, k = 0; i < 3; i++, k += 3) {
        for (j = 0, l = k; j < 3; j++) {
            if (c[l + j] < 0) {
                c[l + j] = -c[l + j];
            }
        }
    }

    idx = 0;
    max = c[0];
    for (i = 0, k = 0; i < 3; i++, k += 3) {
        for (j = 0, l = k; j < 3; j++, l++) {
            if (max < c[l]) {
                idx = l;
                max = c[l];
            }
        }
    }

    switch (idx) {
    case 6:
    case 7:
    case 8:
        v1.vx = a.m[0][0];
        v1.vy = a.m[0][1];
        v1.vz = a.m[0][2];
        v2.vx = a.m[1][0];
        v2.vy = a.m[1][1];
        v2.vz = a.m[1][2];
        break;
    case 3:
    case 4:
    case 5:
        v1.vx = a.m[0][0];
        v1.vy = a.m[0][1];
        v1.vz = a.m[0][2];
        v2.vx = a.m[2][0];
        v2.vy = a.m[2][1];
        v2.vz = a.m[2][2];
        break;
    case 0:
    case 1:
    case 2:
        v1.vx = a.m[1][0];
        v1.vy = a.m[1][1];
        v1.vz = a.m[1][2];
        v2.vx = a.m[2][0];
        v2.vy = a.m[2][1];
        v2.vz = a.m[2][2];
        break;
    }

    OuterProduct12(&v1, &v2, &v3);
    vector_normalize(&v3, &n0);
    t->m[0][0] = n0.vx;
    t->m[1][0] = n0.vy;
    t->m[2][0] = n0.vz;
    vector_normalize(&v1, &n2);
    t->m[0][1] = n2.vx;
    t->m[1][1] = n2.vy;
    t->m[2][1] = n2.vz;
    OuterProduct12(&n0, &n2, &v3);
    vector_normalize(&v3, &n1);
    t->m[0][2] = n1.vx;
    t->m[1][2] = n1.vy;
    t->m[2][2] = n1.vz;
}
