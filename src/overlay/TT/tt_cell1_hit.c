/* @SymbolStart: 0x8013A710 */
/* @SymbolSize: 256 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad000[20];
    s16 h20;
    s16 h22;
    u8  pad018[76];
    u8  b100;
    u8  pad065;
    u8  b102;
    u8  pad067[485];
    s16 h588[24];
    s16 h636[24];
    u8  pad2AC[250];
    u16 h934;
} Serve;

typedef struct { s16 x; s16 y; } Pt;
typedef struct { u8 b[6]; } TT_CELL;

extern Serve *tt_boss_wk;
extern TT_CELL   tt_cell_box_tbl[];

void tt_cell1_hit()
{
    Serve *p;
    s32 idx;
    s32 unused[7];
    Pt pt;
    s32 unused2;
    s32 rem;

    p = tt_boss_wk;
    idx = p->b102;
    rem = p->h934 % 3;
    if (p->b100 == 2 && p->b102 != 3) {
        idx += 3;
    }
    pt.x = p->h588[0] + p->h20;
    pt.y = p->h636[0] + p->h22;
    tt_cell_hit_scan(&pt, &tt_cell_box_tbl[idx], 1);
    pt.x = p->h588[1] + p->h20;
    pt.y = p->h636[1] + p->h22;
    tt_cell_hit_scan(&pt, &tt_cell_box_tbl[rem + 11], -1);
}
