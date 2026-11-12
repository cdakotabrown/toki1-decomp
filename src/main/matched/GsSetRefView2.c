/* Function: GsSetRefView2  --  3D listener/view matrix + doppler-style */
/* Address: 0x8009A530 */
/* Size: 1012 bytes (253 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --expand-div --defer-div --mflo-noreorder-nops --konami-lib-sched */
/* @SymbolStart: 0x8009A530 */
/* @SymbolSize: 1012 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
typedef signed short s16;
typedef signed int s32;

typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;

typedef struct {
    s32 vx, vy, vz, pad;
} VECTOR;

typedef struct {
    s32 vpx, vpy, vpz;
    s32 vrx, vry, vrz;
    s32 rz;
    void *super;
} GsRVIEW2;

extern MATRIX GsIDMATRIX2;
extern MATRIX GsWSMATRIX;
extern MATRIX GsWSMATRIX_ORG;

extern void gte_rotate_z_matrix(MATRIX *m, s32 rz);
extern void scale_view_param(s32 *arg0, s32 *arg1);
extern s32 SquareRoot0(s32 x);
extern void Gssub_make_matrix(MATRIX *m, s16 sin, s16 cos, s32 axis);
extern void MulMatrix(MATRIX *m0, MATRIX *m1);
extern void ApplyMatrixLV(MATRIX *m, VECTOR *in, VECTOR *out);
extern void GsGetLw(void *chain, MATRIX *m);
extern void TransposeMatrix(MATRIX *in, MATRIX *out);
extern void GsMulCoord2(s32 *a0, s32 *a1);

s32 GsSetRefView2(GsRVIEW2 *view)
{
    s32 local[8];
    MATRIX m;
    MATRIX m2;
    MATRIX unused;
    VECTOR v;
    s32 l;
    s32 r;
    s32 sn;

    GsWSMATRIX = GsIDMATRIX2;
    gte_rotate_z_matrix(&GsWSMATRIX, -view->rz);

    scale_view_param((s32 *)view, local);

    l = SquareRoot0((local[3] - local[0]) * (local[3] - local[0])
                     + (local[4] - local[1]) * (local[4] - local[1])
                     + (local[5] - local[2]) * (local[5] - local[2]));
    if (l == 0)
        return 1;

    r = local[1] - local[4];
    sn = (r << 12) / l;
    sn = -sn;
    r = SquareRoot0((local[3] - local[0]) * (local[3] - local[0])
                     + (local[5] - local[2]) * (local[5] - local[2]));
    Gssub_make_matrix(&m, sn, (r << 12) / l, 120);
    MulMatrix(&GsWSMATRIX, &m);

    if (r != 0) {
        l = r;
        r = local[3] - local[0];
        sn = (r << 12) / l;
        r = local[5] - local[2];
        Gssub_make_matrix(&m, -sn, (r << 12) / l, 121);
        MulMatrix(&GsWSMATRIX, &m);
    }

    v.vx = -view->vpx;
    v.vy = -view->vpy;
    v.vz = -view->vpz;
    ApplyMatrixLV(&GsWSMATRIX, &v, (VECTOR *)GsWSMATRIX.t);

    if (view->super) {
        GsGetLw(view->super, &m);
        TransposeMatrix(&m, &m2);
        ApplyMatrixLV(&m2, (VECTOR *)m.t, &v);
        m2.t[0] = -v.vx;
        m2.t[1] = -v.vy;
        m2.t[2] = -v.vz;
        GsMulCoord2((s32 *)&GsWSMATRIX, (s32 *)&m2);
        GsWSMATRIX = m2;
    }

    GsWSMATRIX_ORG = GsWSMATRIX;
    return 0;
}
