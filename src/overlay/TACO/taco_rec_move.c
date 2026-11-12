/* @PostProc: patch_stack_complete --old 80 --new 88 */
/* @PostProc: remap_stack_slots --from 28 --to 44 --from 32 --to 48 --from 48 --to 56 --from 68 --to 76 --from 72 --to 80 --from 76 --to 84 */
/* @PostProc: patch_insn_reg_field 0x100:rs:t5:t3 0x100:rt:t3:t5 0x140:rs:t2:t6 0x140:rt:t6:t2 0x14c:rt:t3:t5 0x150:rs:t3:t5 0x150:rt:t3:t5 0x154:rd:t5:t3 0x158:rs:t5:t3 0x158:rt:t3:t5 0x19c:rt:t6:t2 0x1a4:rs:t6:t2 0x1b4:rd:t5:t3 0x1b8:rs:t5:t3 0x1fc:rt:t6:t2 0x204:rs:t6:t2 0x210:rs:t6:t2 0x21c:rs:t6:t2 0x228:rs:t6:t2 0x234:rs:t6:t2 0x240:rs:t6:t2 0x24c:rs:t6:t2 0x258:rs:t6:t2 */
/* Address: 0x80146F24 */
/* Size: 604 bytes (151 insns) */
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

extern void RotMatrixZ(s32, void *);
extern void RotMatrixX(s32, void *);
extern void RotMatrixY(s32, void *);

extern Mtx GsIDMATRIX;
extern Rec taco_coord[];
extern s16 taco_angle_tbl[][4];
extern u8 *taco_actor_rec;

void taco_rec_move(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8)
{
    Mtx m;
    Rec *v1;
    s16 *v0;
    u8 *q;

    m = GsIDMATRIX;
    v1 = &taco_coord[a0];
    m.t[0] = v1->mat.t[0] + a1;
    m.t[1] = v1->mat.t[1] + a2;
    m.t[2] = v1->mat.t[2] + a3;

    if (arg7 != 0) {
        if (arg8 != 0) {
            q = taco_actor_rec;
            m.t[0] = m.t[0] + (*(s16 *)(q + 116) - m.t[0]) / arg7;
            m.t[1] = m.t[1] + (*(s16 *)(q + 118) - m.t[1]) / arg8;
        }
    }

    v0 = taco_angle_tbl[a0];
    v0[0] += arg4;
    RotMatrixX(((s32)v0[0] << 12) / 360, &m);

    v0[2] += arg6;
    RotMatrixZ(((s32)v0[2] << 12) / 360, &m);

    v0[1] += arg5;
    RotMatrixY(((s32)v0[1] << 12) / 360, &m);

    v1->mat = m;
    v1->flag = 0;
}
