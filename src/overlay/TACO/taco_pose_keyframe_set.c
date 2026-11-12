/* @PostProc: patch_stack_complete --old 72 --new 88 */
/* @PostProc: remap_stack_slots --from 24 --to 44 --from 28 --to 48 --from 40 --to 56 --from 60 --to 76 --from 64 --to 80 --from 68 --to 84 --from 76 --to 92 */
/* @PostProc: patch_insn_reg_field 0x14:rt:t6:t8 0x20:rs:t6:t8 0x20:rt:t6:t8 0x24:rs:t6:t8 0x34:rs:t6:t8 0x34:rt:t9:t1 0x38:rd:t0:t6 0x3c:rt:t9:t1 0x40:rs:t6:t8 0x44:rd:t0:t6 0x44:rs:t0:t6 0x4c:rs:t6:t8 0x4c:rt:t9:t1 0x54:rt:t9:t1 0x58:rs:t6:t8 0x5c:rd:t1:t7 0x5c:rt:t0:t6 0x64:rs:t6:t8 0x64:rt:t9:t1 0x6c:rt:t9:t1 0x70:rs:t6:t8 0x74:rt:t1:t7 0x7c:rs:t6:t8 0x7c:rt:t9:t1 0x84:rt:t9:t1 0x8c:rt:t8:t5 0xa0:rt:t5:t6 0xa4:rt:t6:t9 0xa8:rt:t8:t6 0xb0:rt:t9:t0 0xb4:rt:t1:t3 0xb8:rd:t0:t8 0xb8:rs:t6:t0 0xbc:rt:t0:t8 0xc0:rt:t3:t1 0xc4:rt:t8:t6 0xc8:rs:t8:t6 0xc8:rt:t8:t6 0xd8:rt:t8:t6 0xe0:rt:t6:t0 0xe8:rt:t6:t0 0xf8:rd:t0:t8 0xfc:rs:t0:t8 0x160:rt:t6:t0 0x168:rt:t6:t0 0x178:rd:t0:t8 0x17c:rs:t0:t8 */
/* Address: 0x80147460 */
/* Size: 492 bytes (123 insns) */
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

void taco_pose_keyframe_set(s32 a0, s16 *p, s32 a2)
{
    Mtx m;
    Rec *v1;
    s16 *v0;

    m = GsIDMATRIX;
    p += a2 * 6;
    v1 = &taco_coord[a0];
    m.t[0] = p[0] + v1->mat.t[0];
    m.t[1] = p[1] + v1->mat.t[1];
    m.t[2] = p[2] + v1->mat.t[2];

    v0 = taco_angle_tbl[a0];
    v0[0] += p[3];
    p += 4;
    RotMatrixX(((s32)v0[0] << 12) / 360, &m);

    v0[2] += p[0];
    p += 1;
    RotMatrixZ(((s32)v0[2] << 12) / 360, &m);

    v0[1] += p[0];
    RotMatrixY(((s32)v0[1] << 12) / 360, &m);

    v1->mat = m;
    v1->flag = 0;
}
