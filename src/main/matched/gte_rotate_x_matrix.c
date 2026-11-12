/* Function: gte_rotate_x_matrix (actually RotMatrixX) */
/* Address: 0x8009F8B0 */
/* Size: 184 bytes (46 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

extern s16 rcos(s32);
extern s32 rsin(s32);
extern void MulMatrix(MATRIX *, MATRIX *);

void gte_rotate_x_matrix(MATRIX *arg0, s32 arg1)
{
    s32 angle;
    s16 cos_val, sin_val;
    MATRIX mat;

    angle = arg1 / 360;

    cos_val = rcos(angle);
    sin_val = rsin(angle);

    if (arg1 != 0) {
        mat.m[0][0] = 4096;
        mat.m[0][1] = 0;
        mat.m[0][2] = 0;
        mat.m[1][0] = 0;
        mat.m[1][1] = cos_val;
        mat.m[1][2] = -sin_val;
        mat.m[2][0] = 0;
        mat.m[2][1] = sin_val;
        mat.m[2][2] = cos_val;
        mat.t[0] = 0;
        mat.t[1] = 0;
        mat.t[2] = 0;

        MulMatrix(arg0, &mat);
    }
}
