/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: remap_stack_slots --from 36 --to 60 --from 48 --to 64 --from 52 --to 68 --from 80 --to 76 --from 82 --to 78 --from 84 --to 80 --from 88 --to 84 --from 90 --to 86 --from 92 --to 88 --from 96 --to 92 */
/* @PostProc: patch_insn_reg_field 0x14:rt:t6:t7 0x18:rs:t6:t7 0x18:rt:t6:t7 0x28:rs:t6:t7 0x34:rd:t7:t6 0x38:rs:v0:t1 0x38:rt:t1:v0 0x40:rt:t7:t6 0x44:rs:t9:a2 0x44:rt:a2:t9 0x58:rt:t7:t6 0x74:rs:t6:t7 0xa0:rd:t7:t6 0xa4:rs:t7:t6 0xc8:rt:t6:t7 0xd0:rt:t6:t7 0xfc:rs:t3:a2 0xfc:rt:a2:t3 0x100:rt:t7:t6 0x108:rt:t7:t6 0x10c:rt:t6:t7 0x11c:rt:t6:t7 0x140:rt:t7:t6 0x144:rs:t7:t6 0x144:rt:t7:t6 0x14c:rs:t7:t6 0x14c:rt:t6:t7 0x154:rs:t6:t7 0x15c:rs:t7:t6 0x160:rs:t7:t6 0x258:rt:t6:t7 0x270:rt:t6:t7 0x27c:rt:t7:t6 0x280:rs:t7:t6 0x280:rt:t7:t6 0x298:rs:t7:t6 0x29c:rd:t6:t7 0x2a4:rs:t6:t7 0x2b0:rt:t7:t6 0x2c0:rt:t7:t6 0x2d4:rt:t6:t7 0x2d8:rs:t6:t7 0x2d8:rt:t6:t7 0x2e8:rd:t7:t6 0x2e8:rs:t6:t7 0x2ec:rt:t7:t6 0x2fc:rs:t3:v1 0x2fc:rt:v1:t3 0x31c:rt:t6:t7 0x330:rt:t6:t7 0x334:rd:t7:t6 0x338:rs:t7:t6 0x348:rt:t6:t7 0x34c:rs:t3:v1 0x34c:rt:v1:t3 0x360:rs:t6:t7 0x360:rt:t6:t7 0x368:rd:v0:t2 0x368:rs:t6:t7 0x36c:rs:v0:t2 0x378:rt:t2:t4 0x37c:rs:t2:t4 0x37c:rt:t2:t4 0x380:rt:t3:t5 0x384:rs:t2:t4 0x384:rt:t4:t6 0x388:rs:t4:t6 0x38c:rd:t5:t8 0x390:rd:t9:t3 0x398:rt:t9:t3 0x39c:rt:t7:t9 0x3a4:rt:t7:t9 0x3ac:rt:t5:t8 0x3b0:rt:t3:t5 */
/* Address: 0x80148774 */
/* Size: 952 bytes (238 insns) */
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
extern void RotMatrixZ(s32, void *);
extern void RotMatrixX(s32, void *);
extern void RotMatrixY(s32, void *);

extern Mtx GsIDMATRIX;
extern Rec taco_coord[];
extern s16 taco_angle_tbl[][4];
extern u8 *taco_actor_rec;
extern s32 taco_obj[][4];
extern s32 step_counter;

void taco_actor_face_set(s32 a0, s32 a1, s16 *a2, u8 *a3, u8 *arg4)
{
    Mtx m;
    SV in;
    SV out;
    Rec *r;
    s16 *pv;
    u8 *q;
    s32 st;
    s32 idx;
    s32 off;
    s32 dx;
    s32 dy;
    u8 *ac;

    st = taco_actor_rec[2314];
    idx = a1 + 16;

    if (st == 1) {
        pv = a2 + a1 * 4;
        dx = pv[0];
        dy = pv[1];
        q = a3 + a1 * 2;
        taco_actor_rec[idx * 136 + 132] = q[0];
        if (a0 == 28) {
            if (taco_actor_rec[2310] == 1) {
                taco_actor_rec[idx * 136 + 132] = q[1];
            }
        }
        if (a0 == 28 || a0 == 27 || a0 == 21) {
            if (taco_actor_rec[2990] == 1) {
                taco_actor_rec[idx * 136 + 132] = q[1];
            }
        }
    } else if (st == 29) {
        pv = a2 + a1 * 4;
        dx = pv[2];
        dy = pv[3];
        q = a3 + a1 * 2;
        taco_actor_rec[idx * 136 + 132] = q[1];
        if (a0 == 27 || a0 == 28) {
            if (taco_actor_rec[3126] != 0) {
                if (taco_actor_rec[3262] == 0) {
                    taco_actor_rec[idx * 136 + 132] = 0;
                }
            }
        }
    }

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
            ac[133] = arg4[a1 * 2];
            ac = taco_actor_rec + off;
            *(s16 *)(ac + 130) = *(s16 *)(ac + 130) + 1;
        }
    }

    ac = taco_actor_rec + off;
    if (ac[133] != 0) {
        return;
    }

    if ((step_counter & 1) == 0) {
        taco_obj[a0][0] = 0x40000000;
    } else {
        taco_obj[a0][0] = (s32)0x80000000;
    }
}
