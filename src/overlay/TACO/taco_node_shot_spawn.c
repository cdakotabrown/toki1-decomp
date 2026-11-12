/* @PostProc: patch_stack_complete --old 96 --new 104 */
/* @PostProc: remap_stack_slots --from 28 --to 44 --from 40 --to 48 --from 42 --to 50 --from 44 --to 52 --from 48 --to 56 --from 50 --to 58 --from 52 --to 60 --from 56 --to 64 --from 58 --to 66 --from 60 --to 68 --from 64 --to 72 */
/* Address: 0x80147C70 */
/* Size: 488 bytes (122 insns) */
#include "types.h"

typedef struct {
    s16 m[3][3];
    s32 t[3];
} Mtx;

typedef struct {
    s32 flag;
    Mtx mat;
    u8  rest[44];
} Rec;

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SV;

extern void ApplyMatrixSV(Mtx *, SV *, SV *);
extern void taco_shot_spawn(SV, s32, s32);
extern void RotMatrixZ(s32, void *);
extern void RotMatrixX(s32, void *);
extern void RotMatrixY(s32, void *);

extern Mtx GsIDMATRIX;
extern Rec taco_coord[];
extern s16 taco_angle_tbl[][4];

void taco_node_shot_spawn(s32 a0, s32 a1, s16 a2, s16 a3, s16 arg4, s32 arg5,
                   s32 arg6)
{
    Mtx m;
    SV in;
    SV out;
    SV sv;
    s16 *v0;
    Rec *r;

    m = GsIDMATRIX;

    v0 = taco_angle_tbl[a0];
    RotMatrixX(((s32)v0[0] << 12) / 360, &m);
    RotMatrixZ(((s32)v0[2] << 12) / 360, &m);
    RotMatrixY(((s32)v0[1] << 12) / 360, &m);

    r = &taco_coord[a1];
    in.vx = (s16)r->mat.t[0];
    in.vy = (s16)r->mat.t[1];
    in.vz = (s16)r->mat.t[2];
    ApplyMatrixSV(&m, &in, &out);

    r = &taco_coord[a0];
    sv.vx = r->mat.t[0] + out.vx + a2;
    sv.vy = r->mat.t[1] + out.vy + a3;
    sv.vz = r->mat.t[2] + out.vz + arg4;
    taco_shot_spawn(sv, arg5, arg6);
}
