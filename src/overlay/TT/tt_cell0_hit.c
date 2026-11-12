/* @SymbolStart: 0x8013A610 */
/* @SymbolSize: 256 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad000[20];
    s16 h20;
    s16 h22;
    u8  pad018[36];
    u8  b60;
    u8  pad03D;
    u8  b62;
    u8  pad03F[521];
    s16 h584[24];
    s16 h632[24];
    u8  pad2A8[252];
    u16 h932;
} Serve;

typedef struct { s16 x; s16 y; } Pt;
typedef struct { u8 b[6]; } TT_CELL;

extern Serve *tt_boss_wk;
extern TT_CELL   tt_cell_box_tbl[];

void tt_cell0_hit()
{
    Serve *p;
    s32 idx;
    s32 unused[7];
    Pt pt;
    s32 unused2;
    s32 rem;

    p = tt_boss_wk;
    idx = p->b62;
    rem = p->h932 % 3;
    if (p->b60 == 2 && p->b62 != 3) {
        idx += 3;
    }
    pt.x = p->h584[0] + p->h20;
    pt.y = p->h632[0] + p->h22;
    tt_cell_hit_scan(&pt, &tt_cell_box_tbl[idx], 0);
    pt.x = p->h584[1] + p->h20;
    pt.y = p->h632[1] + p->h22;
    tt_cell_hit_scan(&pt, &tt_cell_box_tbl[rem + 14], -1);
}
