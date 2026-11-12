/* @SymbolSize: 644 */
/* @PostProc: patch_stack_complete --old 88 --new 96 */
/* @PostProc: remap_stack_slots --from 36 --to 52 --from 40 --to 56 --from 76 --to 84 --from 80 --to 88 --from 84 --to 92 --from 56 --to 64 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: patch_insn_reg_field 0x20:rt:t7:t9 0x24:rs:t7:t9 0x24:rt:t7:t9 0x28:rs:t7:t9 0x2c:rs:t7:t9 0x2c:rt:t0:t2 0x30:rt:t6:t8 0x34:rs:t6:t8 0x38:rs:t6:t8 0x38:rt:t0:t2 0x3c:rs:t7:t9 0x3c:rt:t0:t2 0x40:rs:t7:t9 0x44:rs:t6:t8 0x44:rt:t0:t2 0x48:rs:t6:t8 0x4c:rs:t7:t9 0x50:rs:t7:t9 0x50:rt:t0:t2 0x54:rs:t6:t8 0x58:rs:t6:t8 0x58:rt:t0:t2 0x5c:rs:t7:t9 0x5c:rt:t0:t2 0x60:rs:t7:t9 0x64:rs:t6:t8 0x64:rt:t0:t2 0x68:rs:t6:t8 0x70:rd:t1:t6 0x74:rd:t1:t6 0x74:rs:t1:t6 0x7c:rd:t2:t7 0x7c:rt:t1:t6 0x88:rt:t2:t7 0x98:rt:t9:t7 0x9c:rt:t8:t6 0xa0:rt:t7:t8 0xa4:rd:t6:t1 0xa4:rs:t9:t6 0xa4:rt:t8:t7 0xa8:rt:t6:t1 0xb0:rt:t2:t3 0xb4:rd:t1:t9 0xb4:rs:t7:t0 0xb4:rt:t0:t8 0xb8:rt:t1:t9 0xbc:rt:t3:t2 0xe4:rd:t8:t7 0xe4:rs:t6:t1 0xe8:rt:t9:t6 0xf0:rd:t6:t1 0xf0:rs:t9:t6 0xf0:rt:t6:t1 0xf4:rs:t6:t1 0x110:rs:t6:t1 0x11c:rd:t7:t0 0x120:rd:t0:t8 0x120:rt:t8:t0 0x124:rt:t0:t8 0x128:rt:t1:t9 0x130:rs:t1:t9 0x160:rd:t9:t6 0x160:rs:t4:t2 0x160:rt:t2:t4 0x164:rt:t9:t6 0x168:rt:t6:t1 0x16c:rt:t8:t0 0x170:rs:t8:t0 0x170:rt:t8:t0 0x174:rt:t6:t1 0x178:rt:t8:t0 0x17c:rt:t0:t8 0x188:rd:t1:t9 0x188:rs:t0:t8 0x18c:rt:t1:t9 0x1bc:rt:t9:t6 0x1c0:rt:t2:t4 0x1c8:rd:t6:t1 0x1c8:rs:t2:t4 0x1c8:rt:t9:t6 0x1cc:rt:t6:t1 0x1f8:rt:t0:t8 0x200:rd:t1:t9 0x200:rs:t0:t8 0x204:rt:t1:t9 0x224:rt:t2:t4 0x22c:rs:t2:t4 0x238:rs:t2:t4 0x238:rt:t6:t1 0x240:rt:t6:t1 0x244:rs:t2:t4 0x250:rs:t2:t4 0x250:rt:t6:t1 0x258:rt:t6:t1 0x25c:rs:t2:t4 0x268:rs:t2:t4 0x268:rt:t6:t1 0x270:rt:t6:t1 0x274:rs:t2:t4 0x280:rs:t2:t4 0x280:rt:t6:t1 0x290:rt:t6:t1 */
/* Address: 0x8014764C */
/* Size: 644 bytes (161 insns) */
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

void taco_pose_keyframe_ease(s32 a0, s16 *p, s32 a2, s32 a3, s32 arg4)
{
    Mtx m;
    Rec *v1;
    s16 *v0;
    u8 *q;

    m = GsIDMATRIX;
    p += a2 * 6;
    v1 = &taco_coord[a0];
    m.t[0] = *p++ + v1->mat.t[0];
    m.t[1] = *p++ + v1->mat.t[1];
    m.t[2] = *p++ + v1->mat.t[2];

    if (a3 != 0) {
        if (arg4 != 0) {
            q = taco_actor_rec;
            m.t[0] = m.t[0] + (*(s16 *)(q + 116) - m.t[0]) / a3;
            m.t[1] = m.t[1] + (*(s16 *)(q + 118) - m.t[1]) / arg4;
        }
    }

    v0 = taco_angle_tbl[a0];
    v0[0] += *p++;
    RotMatrixX(((s32)v0[0] << 12) / 360, &m);

    v0[2] += *p++;
    RotMatrixZ(((s32)v0[2] << 12) / 360, &m);

    v0[1] += *p;
    RotMatrixY(((s32)v0[1] << 12) / 360, &m);

    v1->mat = m;
    v1->flag = 0;
}
