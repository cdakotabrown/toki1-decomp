/* @SymbolSize: 984 */
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
} Sprt16;

typedef struct {
    Tag tag;
    u8  r0, g0, b0, code;
    s16 x0, y0;
    u8  r1, g1, b1, pad1;
    s16 x1, y1;
    u8  r2, g2, b2, pad2;
    s16 x2, y2;
    u8  r3, g3, b3, pad3;
    s16 x3, y3;
} PolyG4;

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
    u8  a, b, c;
} Ent;

typedef struct {
    u8  *map;
    Ent *ent;
    u8  *pal;
} Tex;

typedef struct {
    u8  pad0[24];
    Tex *tex;
} TTState;

typedef struct {
    u8  pad0[8];
    s32 buf;
} TTDisp;

extern TTDisp  *tt_disp_wk;
extern TTState *tt_step_wk;
extern DB      *tt_db_buf;

#define setlen(p, n) ((p)->tag.len = (n))
#define setcode(p, c) ((p)->code = (c))
#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3) \
    (p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x1), (p)->y1 = (_y1), \
    (p)->x2 = (_x2), (p)->y2 = (_y2), (p)->x3 = (_x3), (p)->y3 = (_y3)
#define setRGB0(p, _r, _g, _b) (p)->r0 = (_r), (p)->g0 = (_g), (p)->b0 = (_b)
#define setRGB1(p, _r, _g, _b) (p)->r1 = (_r), (p)->g1 = (_g), (p)->b1 = (_b)
#define setRGB2(p, _r, _g, _b) (p)->r2 = (_r), (p)->g2 = (_g), (p)->b2 = (_b)
#define setRGB3(p, _r, _g, _b) (p)->r3 = (_r), (p)->g3 = (_g), (p)->b3 = (_b)
#define addPrim(ot, p) ((p)->tag.addr = (ot)->addr, (ot)->addr = (u32)(p))

void tt_board_disp()
{
    s32 unused0;
    s32 unused1;
    DB *db;
    Tag *ot;
    TTState *st;
    Sprt16 *p;
    DrMode *dm;
    PolyG4 *g4;
    Ent *e;
    s32 i;
    s32 y;
    u32 v;

    st = tt_step_wk;
    db = &tt_db_buf[tt_disp_wk->buf];
    ot = db->ot;
    for (y = 0; y < 128; y += 16) {
        for (i = 0; i < 16; i++) {
            p = (Sprt16 *)db->prim;
            db->prim += sizeof(Sprt16);
            p->code = 125;
            p->tag.len = 3;
            p->x0 = (i << 4) - 128;
            p->y0 = y + 112;
            e = &st->tex->ent[st->tex->map[(y + i) & 0x3FF]];
            p->u0 = (st->tex->pal[e->a] & 0x80) + ((e->b & 7) << 4);
            p->v0 = ((st->tex->pal[e->a] & 0x40) << 1) + ((e->b >> 3) << 4);
            v = e->c;
            p->clut = (v & 15) + (((v >> 4) + 240) << 6);
            addPrim(&ot[15], p);
        }
    }
    dm = (DrMode *)db->prim;
    db->prim += sizeof(DrMode);
    SetDrawMode(dm, 0, 0, 24, 0);
    addPrim(&ot[15], dm);
    dm = (DrMode *)db->prim;
    db->prim += sizeof(DrMode);
    SetDrawMode(dm, 0, 0, 24, 0);
    addPrim(&ot[17], dm);
    g4 = (PolyG4 *)db->prim;
    db->prim += sizeof(PolyG4);
    setlen(g4, 8); setcode(g4, 57);
    setXY4(g4, -128, 0, 128, 0, -128, 168, 128, 168);
    setRGB0(g4, 0, 104, 240);
    setRGB1(g4, 0, 104, 240);
    setRGB2(g4, 176, 240, 216);
    setRGB3(g4, 176, 240, 216);
    addPrim(&ot[17], g4);
    dm = (DrMode *)db->prim;
    db->prim += sizeof(DrMode);
    SetDrawMode(dm, 0, 1, 24, 0);
    addPrim(&ot[17], dm);
}
