/* Function: gte_rotate_z_matrix (gte_rotate_z_matrix) */
/* Address: 0x8009E130 */
/* Size: 180 bytes (45 instructions) */
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
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

extern s32 rcos(s32);
extern s32 rsin(s32);
extern void MulMatrix(MATRIX *dst, MATRIX *src);

void gte_rotate_z_matrix(MATRIX *dst, s32 angle_input) {
    s32 angle;
    s16 cos_val;
    s16 sin_val;
    MATRIX mat;

    angle = angle_input / 360;
    cos_val = rcos(angle);
    sin_val = rsin(angle);

    if (angle_input != 0) {
        mat.m[0][0] = cos_val;
        mat.m[0][2] = 0;
        mat.m[1][0] = sin_val;
        mat.m[1][1] = cos_val;
        mat.m[1][2] = 0;
        mat.m[2][0] = 0;
        mat.m[2][1] = 0;
        mat.m[0][1] = -sin_val;
        mat.t[0] = 0;
        mat.m[2][2] = 4096;
        mat.t[1] = 0;
        mat.t[2] = 0;

        MulMatrix(dst, &mat);
    }
}
