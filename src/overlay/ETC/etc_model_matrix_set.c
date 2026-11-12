/* @SymbolStart: 0x8013F9FC */
/* @SymbolSize: 396 */
/* @PostProc: patch_stack_complete --old 112 --new 144 */
/* @PostProc: remap_stack_slots --from 28 --to 48 --from 32 --to 52 --from 36 --to 56 --from 40 --to 64 --from 72 --to 96 --from 92 --to 116 --from 96 --to 120 --from 100 --to 124 --from 104 --to 128 --from 106 --to 130 --from 108 --to 132 */
/* Address: 0x8013FA04 */
/* Size: 388 bytes (97 instructions) */
#include "types.h"

extern s32 etc_model_roty;

typedef struct {
    s16 m[3][3];
    s32 t[3];
} Mtx;

extern void RotMatrix(s16 *, Mtx *);
extern void MulMatrix(Mtx *, Mtx *);
extern void ScaleMatrix(Mtx *, s32 *);

extern s32 coord_buf;
extern Mtx coord_buf_0_coord;
extern Mtx GsIDMATRIX;
extern s32 etc_model_pos[];

void etc_model_matrix_set(void)
{
    s16 v[3];
    Mtx m1;
    Mtx m2;
    s32 scale[3];
    s32 ang;

    ang = etc_model_roty;
    v[2] = 0;
    v[0] = 0;
    v[1] = ang;
    if (ang >= 3072) {
        v[1] = 3072;
    }

    m1 = GsIDMATRIX;
    m2 = GsIDMATRIX;
    RotMatrix(v, &m1);

    scale[0] = 4096;
    scale[1] = 4096;
    scale[2] = 4096;
    ScaleMatrix(&m2, scale);
    MulMatrix(&m1, &m2);

    m1.t[0] = etc_model_pos[0];
    m1.t[1] = etc_model_pos[1];
    m1.t[2] = etc_model_pos[2];

    coord_buf_0_coord = m1;
    coord_buf = 0;
}
