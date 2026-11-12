/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=4,101 */
/* @SymbolSize: 528 */
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
    u8  flag;
    u8  pad1[15];
    u16 pri;
    u8  pad12[2];
    s16 x;
    s16 y;
    u8  pad18[38];
    u16 tpage;
    u16 clut;
    u8  u;
    u8  v;
    u8  pad44[20];
} Rec;

extern TTDisp *tt_disp_wk;
extern Rec    *tt_recs_buf;
extern DB     *tt_db_buf;

#define addPrim(ot, p) ((p)->tag.addr = (ot)->addr, (ot)->addr = (u32)(p))

void tt_recs_disp()
{
    DB *db;
    Rec *r;
    Sprt *p;
    DrMode *dm;
    s32 i;

    db = &tt_db_buf[tt_disp_wk->buf];
    r = tt_recs_buf;
    for (i = 0; i < 8; i++) {
        if (r->flag) {
            p = (Sprt *)db->prim;
            db->prim += sizeof(Sprt);
            p->tag.len = 4, p->code = 101;
            p->x0 = r->x - 8;
            p->y0 = r->y - 8;
            p->h = 16;
            p->w = 16;
            p->u0 = r->u;
            p->v0 = r->v;
            p->clut = r->clut;
            addPrim(&db->ot[r->pri + 2], p);
            dm = (DrMode *)db->prim;
            db->prim += sizeof(DrMode);
            SetDrawMode(dm, 0, 0, r->tpage, 0);
            addPrim(&db->ot[r->pri + 2], dm);
        }
        r++;
    }
}
