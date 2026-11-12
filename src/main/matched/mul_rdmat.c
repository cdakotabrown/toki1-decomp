/* Address: 0x8009F354, Size: 392 bytes (98 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-schedule-insns -fno-schedule-insns2 --fill-epilogue --expand-div --mflo-nops-first */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0x4 0x8 */
/* @PostProc: swap_instructions 0x8 0x14 */
/* @PostProc: swap_instructions 0xc 0x14 */
/* @PostProc: swap_instructions 0x10 0x14 */
/* @PostProc: swap_instructions 0x14 0x18 */
/* @PostProc: swap_instructions 0x18 0x2c */
/* @PostProc: swap_instructions 0x1c 0x20 */
/* @PostProc: swap_instructions 0x20 0x24 */
/* @PostProc: swap_instructions 0x24 0x30 */
/* @PostProc: swap_instructions 0x30 0x34 */
/* @PostProc: swap_instructions 0x38 0x3c */
/* @PostProc: swap_instructions 0x3c 0x40 */
/* @PostProc: swap_instructions 0x40 0x44 */
/* @PostProc: swap_instructions 0x44 0x48 */
/* @PostProc: swap_instructions 0x48 0x4c */
/* @PostProc: swap_instructions 0x50 0x54 */
/* @PostProc: swap_instructions 0x54 0x58 */
/* @PostProc: swap_instructions 0x60 0x64 */
/* @PostProc: swap_instructions 0x94 0xa0 */
/* @PostProc: swap_instructions 0x9c 0xa0 */
/* @PostProc: swap_instructions 0xa4 0xb8 */
/* @PostProc: swap_instructions 0xa8 0xb8 */
/* @PostProc: swap_instructions 0xac 0xb8 */
/* @PostProc: swap_instructions 0xb0 0xb8 */
/* @PostProc: swap_instructions 0xb4 0xb8 */
/* @PostProc: swap_instructions 0xbc 0xc4 */
/* @PostProc: swap_instructions 0xc0 0xc4 */
/* @PostProc: swap_instructions 0xcc 0xd0 */
/* @PostProc: swap_instructions 0xe0 0xe8 */
/* @PostProc: swap_instructions 0xf8 0x100 */
/* @PostProc: swap_instructions 0x110 0x118 */
/* @PostProc: swap_instructions 0x12c 0x134 */
/* @PostProc: swap_instructions 0x14c 0x150 */
/* @PostProc: swap_instructions 0x160 0x164 */
/* @PostProc: swap_instructions 0x178 0x17c */
#include "types.h"

typedef struct { s16 m[3][3]; s32 t[3]; } MATRIX_;
typedef struct { s32 vx, vy, vz, pad; } VECTOR_;

typedef struct {
    s32 rx;
    s32 ry;
    s32 rz;
    s16 sx;
    s16 sy;
    s16 sz;
    s16 pad;
    VECTOR_ pos;
} ObjRec_;

typedef struct {
    s32 unk0;
    ObjRec_ *rec;
} Obj_;

extern void gte_scale_matrix(MATRIX_ *m, s16 sx, s16 sy, s16 sz);
extern void gte_rotate_z_matrix(MATRIX_ *m, s32 ang);
extern void gte_rotate_x_matrix(MATRIX_ *m, s32 ang);
extern void gte_rotate_y_matrix(MATRIX_ *m, s32 ang);
extern void ApplyMatrixLV(MATRIX_ *m, VECTOR_ *in, VECTOR_ *out);

void mul_rdmat(MATRIX_ *m, Obj_ *o)
{
    VECTOR_ out;
    s32 a, b, c;

    a = o->rec->sx;
    b = o->rec->sy;
    c = o->rec->sz;
    gte_scale_matrix(m, 0x1000000 / a, 0x1000000 / b, 0x1000000 / c);
    gte_rotate_z_matrix(m, -o->rec->rz);
    gte_rotate_x_matrix(m, -o->rec->rx);
    gte_rotate_y_matrix(m, -o->rec->ry);
    ApplyMatrixLV(m, &o->rec->pos, &out);
    m->t[0] -= out.vx;
    m->t[1] -= out.vy;
    m->t[2] -= out.vz;
}
