/* @CompileFlags: -Wc,-pic0 */
/* @SymbolSize: 672 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x130 */
/* Address: 0x801443E0 */
#include "types.h"
#include "tokimeki/libgs_types.h"

typedef struct { s32 w[8]; } Mtx;
typedef struct { s16 vx, vy; } Dvec;
typedef struct { s16 vx, vy, vz, pad; } Svec;

typedef struct Spr {
    u32 tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  u0, v0;
    s16 clut;
    s16 x1, y1;
    u8  u1, v1;
    s16 tpage;
    s16 x2, y2;
    u8  u2, v2;
    s16 pad30;
    s16 x3, y3;
    u8  u3, v3;
    s16 pad38;
} Spr;

extern Mtx GsWSMATRIX;
extern s32 activeBuff;
extern u8  ot_buff0[];
extern LIBGS_RVIEW2 ref_view;
extern s16 taco_grid_ofs[];

extern Spr *prim_alloc(s32, s32);
extern void SetPolyFT4(Spr *);
extern s16  GetTPage(s32, s32, s32, s32);
extern s16  GetClut(s32, s32);
extern void AddPrim(u8 *, Spr *);
extern void SetTransMatrix(Mtx *);
extern void SetRotMatrix(Mtx *);
extern void RotTransPers(Svec *, Dvec *, s32 *, s32 *);

void
taco_mesh_disp(void)
{
    s32   unused0[2];
    Mtx   a;
    Svec  v;
    Dvec  sxy[5][2];
    s32   unused1[4];
    s32   p;
    s32   flg;
    s32   unused2;
    s32   ang;
    s32   j;
    s32   i;
    s32   x;
    s32   c;
    Spr  *s;

    a = GsWSMATRIX;
    SetTransMatrix(&a);
    SetRotMatrix(&a);

    v.vz = -32767;
    j = 0;
    ang = 0;
    while (j < 2) {
        v.vy = (s16)(taco_grid_ofs[1] + (ang / 4) + 128);
        for (i = 0, x = 16384; i < 5; i++) {
            v.vx = (s16)(x + ref_view.vrx);
            RotTransPers(&v, &sxy[i][j], &p, &flg);
            x -= 8192;
        }
        ang -= 4096;
        j++;
    }

    for (j = 0; j < 1; j++) {
        for (i = 0; i < 4; i++) {
            s = prim_alloc(40, activeBuff);
            SetPolyFT4(s);
            s->r0 = 128;
            s->g0 = 128;
            s->b0 = 128;
            s->tpage = GetTPage(1, 0, 512, 256);
            c = (i & 1) << 6;
            s->clut = GetClut(0, 494);
            s->v0 = (u8)(c + 63);
            s->v2 = (u8)(c + 63);
            s->v1 = (u8)c;
            s->v3 = (u8)c;
            s->u0 = 0;
            s->u1 = 0;
            s->u2 = 255;
            s->u3 = 255;
            s->x0 = sxy[i][j].vx;
            s->y0 = sxy[i][j].vy;
            s->x1 = sxy[i][j + 1].vx;
            s->y1 = sxy[i][j + 1].vy;
            s->x2 = sxy[i + 1][j].vx;
            s->y2 = sxy[i + 1][j].vy;
            s->x3 = sxy[i + 1][j + 1].vx;
            s->y3 = sxy[i + 1][j + 1].vy;
            AddPrim(&ot_buff0[(activeBuff << 10) + 1012], s);
        }
    }
}
