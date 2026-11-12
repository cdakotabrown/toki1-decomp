/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  *b0;
    u8  b4;
    u8  b5;
    u8  b6;
    u8  pad07;
    u8  b8;
    u8  pad09;
    u8  b10;
    u8  pad0B;
    s16 h12;
    s16 h14;
    u8  b16;
    u8  pad11[3];
} Unit;

typedef struct {
    u8  b0;
    u8  pad001[3];
    u16 h4;
    u16 h6;
    u16 h8;
    u8  pad00A[46];
    Unit unit[24];
    s32 w536;
    u8  pad21C[44];
    s16 h584[24];
    s16 h632[24];
    s16 h680[24];
    s16 h728[4];
    u8  pad2E0[44];
    s16 h780;
    s16 h782;
    u8  b784[24];
    u8  pad328[28];
    u8  b836;
    u8  pad345[31];
    u16 h868;
    u8  pad366[110];
    u16 h980;
} Serve;

extern Serve *tt_boss_wk;
extern u8     tt_anim_tbl[];
extern s16    tt_serve_intro_scr[];
extern u8     tt_flash_seq_tbl[];
extern u8     tt_puff_seq_tbl[];

void tt_serve_intro()
{
    Serve *p;
    s32 i;
    s32 n;
    s32 t;

    p = tt_boss_wk;
    switch (p->h8) {
    case 0:
        tt_cmd_once(1281);
        p->b836 = 1;
        tt_shape_clear();
        p->h868 = 0;
        p->h980 = 0;
        for (i = 8; i < 16; i++) {
            p->unit[i].b8 = 128;
        }
        for (i = 16; i < 19; i++) {
            p->unit[i].b0 = tt_anim_tbl;
            p->unit[i].b4 = 0;
            p->unit[i].b5 = 4;
            p->unit[i].b10 = 0;
            p->unit[i].b16 = 0;
            p->b784[i] = 0;
            p->h680[i] = 7;
            p->h584[i] = 0;
            p->h632[i] = 0;
        }
        p->h8 = 64;
        break;
    case 64:
        t = tt_serve_intro_scr[tt_boss_wk->h980];
        if (t != -1 && t == tt_boss_wk->h868) {
            tt_cmd_once(1281);
            n = tt_serve_intro_scr[p->h980 + 4];
            p->b784[n] = 1;
            p->unit[n].b8 = 128;
            p->h584[n] = tt_serve_intro_scr[p->h980 + 1];
            p->h632[n] = tt_serve_intro_scr[p->h980 + 2];
            p->h680[n] = tt_serve_intro_scr[p->h980 + 3];
            p->h980 += 5;
        }
        for (i = 8; i < 16; i++) {
            tt_seq_step(tt_flash_seq_tbl, i);
        }
        for (i = 16; i < 19; i++) {
            tt_seq_step(tt_puff_seq_tbl, i);
        }
        if (p->h868 == 133) {
            for (i = 0; i < 16; i++) {
                p->b784[i] = 0;
            }
        }
        if (p->h868 == 156) {
            for (i = 0; i < 24; i++) {
                p->b784[i] = 0;
            }
            p->h8 = 0;
            p->h4 = 160;
        }
        break;
    }
    p->h868++;
}
