/* Function: GsSetRefView  --  view-matrix setup (libgs `GsSetRefView2` shape) */
/* Address: 0x8009EBB8 */
/* Size: 996 bytes (249 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --expand-div --defer-div --mflo-noreorder-nops --konami-lib-sched */
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
extern MATRIX GsIDMATRIX;
extern char GsDSTACK[];

extern void gte_rotate_z_matrix(MATRIX *m, s32 rz);
extern s32 SquareRoot0(s32 x);
extern void Gssub_make_matrix(MATRIX *m, s16 sin, s16 cos, s32 axis);
extern void MulMatrix(MATRIX *m0, MATRIX *m1);
extern void ApplyMatrixLV(MATRIX *m, VECTOR *in, VECTOR *out);
extern s32 get_dstack2(void *p);
extern void mul_rdmat(MATRIX *m, char *p);

s32 GsSetRefView(GsRVIEW2 *view)
{
    MATRIX m;
    VECTOR v;
    s32 l;
    s32 r;
    s32 sn;
    s32 cs;
    s32 n;
    s32 i;

    GsWSMATRIX = GsIDMATRIX2;
    gte_rotate_z_matrix(&GsWSMATRIX, -view->rz);

    l = SquareRoot0((view->vrx - view->vpx) * (view->vrx - view->vpx)
                     + (view->vry - view->vpy) * (view->vry - view->vpy)
                     + (view->vrz - view->vpz) * (view->vrz - view->vpz));
    if (l == 0)
        return 1;

    r = view->vpy - view->vry;
    sn = (r << 12) / l;
    sn = -sn;
    r = SquareRoot0((view->vrx - view->vpx) * (view->vrx - view->vpx)
                     + (view->vrz - view->vpz) * (view->vrz - view->vpz));
    Gssub_make_matrix(&m, sn, (r << 12) / l, 120);
    MulMatrix(&GsWSMATRIX, &m);

    if (r != 0) {
        l = r;
        r = view->vrx - view->vpx;
        sn = (r << 12) / l;
        r = view->vrz - view->vpz;
        Gssub_make_matrix(&m, -sn, (r << 12) / l, 121);
        MulMatrix(&GsWSMATRIX, &m);
    }

    v.vx = -view->vpx;
    v.vy = -view->vpy;
    v.vz = -view->vpz;
    ApplyMatrixLV(&GsWSMATRIX, &v, (VECTOR *)GsWSMATRIX.t);

    n = get_dstack2(view->super);
    if (n == -1)
        return 0;

    m = GsIDMATRIX;
    for (i = 0; i <= n; i++)
        mul_rdmat(&m, &GsDSTACK[i * 40]);

    ApplyMatrixLV(&GsWSMATRIX, (VECTOR *)m.t, &v);
    GsWSMATRIX.t[0] += v.vx;
    GsWSMATRIX.t[1] += v.vy;
    GsWSMATRIX.t[2] += v.vz;
    MulMatrix(&GsWSMATRIX, &m);
    return 0;
}
