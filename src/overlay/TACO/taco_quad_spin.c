/* Address: 0x80145600 */
/* Size: 1908 bytes (477 instructions) */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=20:t2,37:t6,54:t3 */
/* @PostProc: patch_stack_complete --old 120 --new 136 */
/* @PostProc: remap_stack_slots --map '80:124,82:126,84:128,76:116,78:118,88:84,108:104,112:108,116:112' */
#include "types.h"

typedef struct { s16 vx, vy, vz, pad; } SVECTOR;
typedef struct { s16 vx, vy; } DVECTOR;
typedef struct { s16 m[3][3]; s32 t[3]; } MATRIX;

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 x0, y0;
    s16 unk12, unk14;
    s16 x1, y1;
    s16 unk20, unk22;
    s16 x2, y2;
    s16 unk28, unk30;
    s16 x3, y3;
} Poly;

typedef struct {
    Poly *a;
    Poly *b;
    Poly *p;
} Obj;

extern MATRIX GsIDMATRIX;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void ApplyMatrixSV(MATRIX *, SVECTOR *, DVECTOR *);
extern void RotMatrixZ(s32, MATRIX *);

void taco_quad_spin(Obj *obj, s32 ang, s32 scale)
{
    MATRIX mtx;
    SVECTOR v;
    DVECTOR out;
    s32 cx;
    s32 cy;
    s32 a;
    s32 n;

    cx = obj->p->x0;
    cy = obj->p->y0;
    v.vx = 0;
    v.vy = 0;
    v.vz = 0;
    mtx = GsIDMATRIX;
    a = (ang << 12) / 360;
    mtx.t[0] = 0;
    mtx.t[1] = 0;
    mtx.t[2] = 0;
    RotMatrixZ(a, &mtx);
    ApplyMatrixSV(&mtx, &v, &out);
    obj->p->x0 = out.vx + cx;
    obj->p->y0 = out.vy + cy;

    v.vx = obj->p->x1 - cx;
    v.vy = obj->p->y1 - cy;
    v.vz = 0;
    mtx = GsIDMATRIX;
    mtx.t[0] = v.vx;
    mtx.t[1] = 0;
    mtx.t[2] = 0;
    RotMatrixZ(a, &mtx);
    ApplyMatrixSV(&mtx, &v, &out);
    obj->p->x1 = out.vx + cx;
    obj->p->y1 = out.vy + cy;

    v.vx = obj->p->x2 - cx;
    v.vy = obj->p->y2 - cy;
    v.vz = 0;
    mtx = GsIDMATRIX;
    mtx.t[0] = v.vx;
    mtx.t[1] = 0;
    mtx.t[2] = 0;
    RotMatrixZ(a, &mtx);
    ApplyMatrixSV(&mtx, &v, &out);
    obj->p->x2 = out.vx + cx;
    obj->p->y2 = out.vy + cy;

    v.vx = obj->p->x3 - cx;
    v.vy = obj->p->y3 - cy;
    v.vz = 0;
    mtx = GsIDMATRIX;
    mtx.t[0] = v.vx;
    mtx.t[1] = 0;
    mtx.t[2] = 0;
    RotMatrixZ(a, &mtx);
    ApplyMatrixSV(&mtx, &v, &out);
    obj->p->x3 = out.vx + cx;
    obj->p->y3 = out.vy + cy;

    n = 3;
    obj->a->x0 = rsin(a) * scale / n / 4096 + obj->p->x0;
    obj->a->y0 = obj->p->y0 - rcos(a) * scale / n / 4096;
    obj->a->x1 = rsin(a) * scale / n / 4096 + obj->p->x1;
    obj->a->y1 = obj->p->y1 - rcos(a) * scale / n / 4096;

    obj->a->x2 = obj->p->x0;
    obj->a->y2 = obj->p->y0;
    obj->a->x3 = obj->p->x1;
    obj->a->y3 = obj->p->y1;
    obj->b->x0 = obj->p->x2;
    obj->b->y0 = obj->p->y2;
    obj->b->x1 = obj->p->x3;
    obj->b->y1 = obj->p->y3;

    obj->b->x2 = obj->p->x2 - rsin(a) * scale / n / 4096;
    obj->b->y2 = rcos(a) * scale / n / 4096 + obj->p->y2;
    obj->b->x3 = obj->p->x3 - rsin(a) * scale / n / 4096;
    obj->b->y3 = rcos(a) * scale / n / 4096 + obj->p->y3;
}
