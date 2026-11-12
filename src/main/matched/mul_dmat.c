/* Address: 0x8009F214 */
/* Size: 320 bytes (80 instructions) */
/* Region: Library */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-epilogue --jal-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x4 0xc */
/* @PostProc: swap_instructions 0x8 0x10 */
/* @PostProc: swap_instructions 0x10 0x14 */
/* @PostProc: swap_instructions 0x14 0x18 */
/* @PostProc: swap_instructions 0x3c 0x40 */
/* @PostProc: swap_instructions 0x50 0x54 */
/* @PostProc: swap_instructions 0x100 0x108 */
/* @PostProc: swap_instructions 0x110 0x120 */
/* @PostProc: swap_instructions 0x114 0x118 */
/* @PostProc: swap_instructions 0x120 0x124 */
/* @PostProc: swap_instructions 0x128 0x12c */
/* @PostProc: swap_instructions 0x12c 0x130 */
/* @PostProc: swap_instructions 0x130 0x134 */
/* @PostProc: swap_instructions 0x134 0x13c */
#include "types.h"

typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fC;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 f1C;
} SndObj;

typedef struct {
    s16 flag;
    s16 result;
    s32 *dataPtr;
    SndObj data;
} SndCtrl;

extern void ApplyMatrixLV(SndObj *, s32 *, s32 *);
extern void gte_rotate_y_matrix(SndObj *, s32);
extern void gte_rotate_x_matrix(SndObj *, s32);
extern void gte_rotate_z_matrix(SndObj *, s32);
extern s32 gte_scale_matrix(SndObj *, s16, s16, s16);

s32 mul_dmat(SndObj *a0, SndCtrl *a1) {
    s32 coords[3];
    s32 result;

    ApplyMatrixLV(a0, a1->dataPtr + 5, coords);

    a0->f14 += coords[0];
    a0->f18 += coords[1];
    a0->f1C += coords[2];

    gte_rotate_y_matrix(a0, a1->dataPtr[1]);
    gte_rotate_x_matrix(a0, a1->dataPtr[0]);
    gte_rotate_z_matrix(a0, a1->dataPtr[2]);
    result = gte_scale_matrix(a0, ((s16 *)a1->dataPtr)[6], ((s16 *)a1->dataPtr)[7], ((s16 *)a1->dataPtr)[8]);

    if (a1->flag == 0) {
        a1->data = *a0;
        a1->result = (s16)result;
        a1->flag = 128;
    }

    return result;
}
