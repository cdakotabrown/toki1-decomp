/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 40 --to 56 --from 44 --to 60 --from 52 --to 68 --from 56 --to 72 --from 64 --to 76 */
/* @PostProc: swap_instructions --offset1 0x234 --offset2 0x244 */
/* @PostProc: swap_instructions --offset1 0x2f8 --offset2 0x2fc */
/* @PostProc: patch_insn_reg_field 0x34:rs:t8:a2 0x34:rt:a2:t8 0x214:rs:t6:v1 0x214:rt:v1:t6 0x234:rt:t7:t5 0x244:rt:t5:t7 0x264:rs:t1:v1 0x264:rt:v1:t1 0x280:rd:v0:t4 0x284:rs:v0:t4 0x294:rt:t4:t3 0x29c:rs:t4:t3 0x2a0:rs:t4:t3 0x2b8:rt:t3:t5 0x2bc:rs:t3:t5 0x2bc:rt:t3:t5 0x2c0:rt:t3:t5 0x2c4:rt:t5:t7 0x2f0:rs:t5:t7 0x2f0:rt:t5:t7 0x2f4:rt:t5:t7 */
/* Address: 0x8014A450 */
/* Size: 796 bytes (199 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

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
extern void taco_node_state_set(s32, s32);
extern void RotMatrixZ(s32, void *);
extern void RotMatrixX(s32, void *);
extern void RotMatrixY(s32, void *);

extern Mtx GsIDMATRIX;
extern Rec taco_coord[];
extern s16 taco_angle_tbl[][4];
extern u8 *taco_actor_rec;

extern Rec taco_coord_5_flg;
extern Rec taco_coord_11_flg;
extern LIBGS_DOBJ2 taco_obj[];


void taco_actor_set2(s32 a0, s32 a1, s16 *a2, u8 *a3, u8 *arg4, s32 arg5)
{
    Mtx m;
    SV in;
    SV out;
    Rec *r;
    s16 *pv;
    s32 k;
    s32 idx;
    s32 off;
    s32 dx;
    s32 dy;
    u8 *ac;

    k = a1 * 2;
    idx = a1 + 16;
    pv = a2 + k;
    dx = pv[0];
    dy = pv[1];
    taco_actor_rec[a1 * 136 + 2308] = a3[a1];

    m = GsIDMATRIX;
    RotMatrixX(((s32)taco_angle_tbl[0][0] << 12) / 360, &m);
    RotMatrixZ(((s32)taco_angle_tbl[0][2] << 12) / 360, &m);
    RotMatrixY(((s32)taco_angle_tbl[0][1] << 12) / 360, &m);

    r = &taco_coord[a0];
    in.vx = (s16)r->mat.t[0];
    in.vy = (s16)r->mat.t[1];
    in.vz = (s16)r->mat.t[2];
    ApplyMatrixSV(&m, &in, &out);

    off = idx * 136;
    *(s16 *)(taco_actor_rec + off + 116) = taco_coord[0].mat.t[0] + out.vx + dx;
    *(s16 *)(taco_actor_rec + off + 118) = taco_coord[0].mat.t[1] + out.vy + dy;
    *(s16 *)(taco_actor_rec + off + 120) = taco_coord[0].mat.t[2] + out.vz;

    ac = taco_actor_rec + off;
    if (ac[133] < 21U) {
        if (*(s16 *)(ac + 130) < *(s16 *)(ac + 122)) {
            ac[133] = arg4[k];
            ac = taco_actor_rec + off;
            *(s16 *)(ac + 130) = *(s16 *)(ac + 130) + 1;
        }
    }

    ac = taco_actor_rec + off;
    if (ac[133] != 0) {
        return;
    }

    if (a0 != 25) {
        taco_node_state_set(a0, arg5);
    }

    if (r == &taco_coord_5_flg) {
        (*(s32 *)&taco_obj[1].attribute) = 0x40000000;
        (*(s32 *)&taco_obj[2].attribute) = 0x40000000;
        (*(s32 *)&taco_obj[3].attribute) = 0x40000000;
        (*(s32 *)&taco_obj[4].attribute) = 0x40000000;
    } else if (r == &taco_coord_11_flg) {
        (*(s32 *)&taco_obj[7].attribute) = 0x40000000;
        (*(s32 *)&taco_obj[8].attribute) = 0x40000000;
        (*(s32 *)&taco_obj[9].attribute) = 0x40000000;
        (*(s32 *)&taco_obj[10].attribute) = 0x40000000;
    }
}
