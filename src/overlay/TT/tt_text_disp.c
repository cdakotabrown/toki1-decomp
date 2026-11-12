/* @CompileFlags: -cckr */
/* @SymbolSize: 292 */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} Tag;

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
    u8  pad1[5];
    u16 bits;
    u8  pad8[12];
} Ent;

extern TTDisp *tt_disp_wk;
extern Ent    *tt_text_buf;
extern DB     *tt_db_buf;

#define addPrim(ot, p) ((p)->tag.addr = (ot)->addr, (ot)->addr = (u32)(p))

void tt_text_disp()
{
    s32 unused0;
    s32 unused1;
    DB *db;
    Ent *e;
    DrMode *dm;
    s32 i;

    db = &tt_db_buf[tt_disp_wk->buf];
    e = tt_text_buf;
    for (i = 0; i < 128; i++) {
        if (e->flag && !(e->bits & 1)) {
            tt_qstr_disp(e);
        }
        e++;
    }
    dm = (DrMode *)db->prim;
    db->prim += sizeof(DrMode);
    SetDrawMode(dm, 0, 0, 10, 0);
    addPrim(&db->ot[2], dm);
}
