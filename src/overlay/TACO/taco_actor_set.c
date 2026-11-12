/* @PostProc: patch_stack_complete --old 144 --new 152 */
/* @PostProc: remap_stack_slots --from 56 --to 76 --from 68 --to 84 --from 72 --to 88 --from 80 --to 92 --from 96 --to 104 --from 98 --to 106 --from 100 --to 108 --from 104 --to 112 --from 106 --to 114 --from 108 --to 116 --from 112 --to 120 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x2a0 */
/* @PostProc: patch_insn_reg_field 0x38:rs:t7:a2 0x38:rt:a2:t7 0x270:rs:t9:v1 0x270:rt:v1:t9 0x290:rt:t1:t2 0x2a0:rt:t2:t1 0x2c0:rs:t7:v1 0x2c0:rt:v1:t7 0x2e8:rd:v0:t2 0x2ec:rs:v0:t2 0x304:rt:t2:t1 0x308:rs:t2:t1 0x308:rt:t2:t1 0x30c:rt:t2:t1 0x330:rt:t1:t6 0x334:rs:t1:t6 0x334:rt:t1:t6 0x338:rt:t1:t6 0x35c:rt:t6:t4 0x360:rs:t6:t4 0x360:rt:t6:t4 0x364:rt:t6:t4 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x801497BC */
/* Size: 916 bytes (229 insns) */
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
extern void taco_band_chan_set(s32, s32);
extern void RotMatrixZ(s32, void *);
extern void RotMatrixX(s32, void *);
extern void RotMatrixY(s32, void *);

extern Mtx GsIDMATRIX;
extern Rec taco_coord[];
extern s16 taco_angle_tbl[][4];
extern u8 *taco_actor_rec;

extern Rec taco_coord_1_flg;
extern Rec taco_coord_2_flg;
extern Rec taco_coord_3_flg;

void taco_actor_set(s32 a0, s32 a1, s16 *a2, u8 *a3, u8 *arg4)
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
    s32 i;
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
            taco_actor_rec[2446] = 1;
        }
    }

    ac = taco_actor_rec + off;
    if (ac[133] != 0) {
        return;
    }

    taco_band_chan_set(a0, 9);

    if (r == &taco_coord_1_flg) {
        for (i = 4; i != 22; i += 3) {
            taco_band_chan_set(i, 9);
        }
    }
    if (r == &taco_coord_2_flg) {
        for (i = 5; i != 23; i += 3) {
            taco_band_chan_set(i, 9);
        }
    }
    if (r == &taco_coord_3_flg) {
        for (i = 6; i != 24; i += 3) {
            taco_band_chan_set(i, 9);
        }
    }
}
