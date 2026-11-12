/* @SymbolStart: 0x8013AE3C */
/* @SymbolSize: 676 */
/* @RodataBase: 0x8014D5B0 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad000[6];
    u16 h6;
    u8  pad008[24];
    s32 w32;
    s32 w36;
    u8  pad028[496];
    s32 w536;
    u8  pad21C[188];
    s16 h728[4];
    u8  pad2E0[44];
    s16 h780;
    u16 h782;
    u8  b784[24];
    u8  pad328[27];
    u8  b835[4];
    u8  pad347[29];
    s16 h868;
    u8  pad366[126];
    s32 w996;
} Serve;

extern Serve *tt_boss_wk;
extern s16    tt_match_pos;
extern u8     tt_cell_box_tbl[];

void tt_match_move0()
{
    Serve *p;
    s32 v;
    s32 i;

    p = tt_boss_wk;
    switch (p->h6) {
    case 0:
        p->w32 = rsin(p->w996) * 1280;
        p->w36 = 0x400000;
        p->w996 += 8;
        if (p->w996 >= 8192) {
            p->w996 = 0;
            p->h6 = 1;
        }
        break;
    case 1:
        p->w36 -= 0x8000;
        if (p->w36 <= 0x300000) {
            p->w36 = 0x300000;
            p->h6 = 2;
        }
        break;
    case 2:
        p->w32 = rsin(p->w996) * 1280;
        p->w36 = 0x300000;
        p->w996 += 16;
        if (p->w996 >= 8192) {
            p->w996 = 0;
            p->h6 = 3;
        }
        break;
    case 3:
        p->w36 += 0x8000;
        if (p->w36 >= 0x400000) {
            p->w36 = 0x400000;
            p->h6 = 0;
        }
        break;
    case 4:
        if (tt_boss_wk->w36 < 0x400000) {
            p->w36 += 0x8000;
        } else {
            p->w36 = 0x400000;
            p->h6 = 5;
        }
        break;
    case 5:
        v = tt_boss_wk->w32;
        if ((v < 0 ? -v : v) < 0x10000) {
            p->w32 = 0;
            p->h6 = 6;
        } else if (v < 0) {
            p->w32 += 0x10000;
        } else {
            p->w32 -= 0x10000;
        }
        break;
    case 6:
        tt_cmd_once(1290);
        p->h782 = 1;
        p->h6 = 0;
        tt_match_pos = 16080;
        p->w536 = (s32)tt_cell_box_tbl;
        for (i = 0; i < 4; i++) {
            if (p->b784[i * 2 + 1] != 0) {
                p->b835[i] = 1;
                p->h728[i] = -1;
            }
        }
        p->w996 = 0;
        p->h868 = 128;
        break;
    }
    if (p->h780 < 0) {
        p->w536 = 0;
        p->h780 = 160;
        p->h6 = 4;
    }
}
