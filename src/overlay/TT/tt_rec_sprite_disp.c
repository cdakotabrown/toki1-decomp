/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} Tag;

typedef struct {
    Tag tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  u0, v0;
    u16 clut;
    s16 w, h;
} Sprt;

typedef struct {
    Tag tag;
    u32 code[2];
} DrMode;

typedef struct {
    u8  pad0[4];
    u8  *prim;
    u8  pad8[0x7FF8];
    Tag ot[46];
} DB;

typedef struct {
    u8  pad0[8];
    s32 buf;
} TTDisp;

typedef struct {
    u8  pad00[16];
    u16 pri;
    u8  pad12[2];
    s16 x;
    s16 y;
    u8  pad18[32];
    u8  r;
    u8  g;
    u8  b;
    u8  code;
    s16 w;
    s16 h;
    u16 tpage;
    u16 clut;
    u16 bits;
    u8  u;
    u8  v;
} Rec;

extern u8     *tt_step_wk;
extern TTDisp *tt_disp_wk;
extern DB     *tt_db_buf;

#define addPrim(ot, p) ((p)->tag.addr = (ot)->addr, (ot)->addr = (u32)(p))

void tt_rec_sprite_disp(r)
Rec *r;
{
    s32 unused0;
    DB *db;
    Sprt *p;
    DrMode *dm;
    u8 *pal;

    db = &tt_db_buf[tt_disp_wk->buf];
    p = (Sprt *)db->prim;
    db->prim += sizeof(Sprt);
    p->tag.len = 4;
    if (r->bits & 8) {
        pal = tt_step_wk;
        p->code = r->code & ~1;
        p->r0 = pal[29];
        p->g0 = pal[30];
        p->b0 = pal[31];
    } else {
        p->code = r->code;
        p->r0 = r->r;
        p->g0 = r->g;
        p->b0 = r->b;
    }
    p->x0 = r->x;
    p->y0 = r->y;
    p->w = r->w;
    p->h = r->h;
    p->u0 = r->u;
    p->v0 = r->v;
    p->clut = r->clut;
    addPrim(&db->ot[r->pri + 2], p);
    dm = (DrMode *)db->prim;
    db->prim += sizeof(DrMode);
    SetDrawMode(dm, 0, 0, r->tpage, 0);
    addPrim(&db->ot[r->pri + 2], dm);
}
