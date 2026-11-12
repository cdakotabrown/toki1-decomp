/* Address: 0x80143354 (absorbed head)  Size: 360 bytes (90 instructions) */
/* @SymbolStart: 0x80143354 */
/* @SymbolSize: 360 */
/* @PostProc: patch_stack_complete --old 104 --new 136 */
/* @PostProc: remap_stack_slots --map '28:48,32:52,36:56,40:64,72:96,92:116,96:120,100:124' */
#include "types.h"

typedef struct { s32 w[8]; } Rec32;

extern Rec32 GsIDMATRIX;
extern Rec32 coord_buf_0_coord;
extern s32 coord_buf;

extern s16 etc_roll_rot;
extern s16 etc_roll_tx;
extern s16 etc_roll_ty;
extern s16 etc_roll_tz;

extern void RotMatrix(void *a0, void *a1);
extern void ScaleMatrix(void *a0, void *a1);
extern void MulMatrix(void *a0, void *a1);

void etc_roll_matrix_set(void) {
    Rec32 buf96;
    Rec32 buf64;
    s32 col[3];

    buf96 = GsIDMATRIX;
    buf64 = GsIDMATRIX;
    RotMatrix(&etc_roll_rot, &buf96);

    col[0] = 4096;
    col[1] = 4096;
    col[2] = 4096;
    ScaleMatrix(&buf64, col);

    MulMatrix(&buf96, &buf64);

    buf96.w[5] = etc_roll_tx;
    buf96.w[6] = etc_roll_ty;
    buf96.w[7] = etc_roll_tz;

    coord_buf_0_coord = buf96;
    coord_buf = 0;
}
