/* @SymbolStart: 0x801441AC */
/* @SymbolSize: 1688 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  b0;
    u8  pad01[3];
    u16 h4;
    u16 h6;
    u16 h8;
    u16 h10;
    u8  pad0C;
    u8  b13;
    u8  b14;
    u8  b15;
    u8  pad10[4];
    u16 h20;
    u16 h22;
    u8  pad18[8];
    s32 w32;
    s32 w36;
    s32 w40;
    s32 w44;
    u8  pad30[8];
    u8  b56;
    u8  pad39[5];
    u8  b62;
    u8  pad3F;
    u8  b64;
    u8  pad41[15];
    u16 h80;
    u8  b82;
    u8  pad53[4];
    u8  b87;
    u8  b88;
    u8  b89;
    u8  b90;
    u8  b91;
    u8  b92;
    u8  b93;
    u8  b94;
    u8  b95;
    u8  pad60[2];
    u16 h98;
    u8  pad64[60];
} St;

typedef struct {
    u8  pad00[56];
    u8  b56;
    u8  pad39[5];
    u8  b62;
    u8  pad3F[95];
    u8  b158;
} Hd;

typedef struct {
    u16 h0;
    u16 h2;
    u16 h4;
    u16 h6;
    u8  pad08;
    u8  b9;
    u8  b10;
    u8  b11;
    u8  pad0C[6];
    u8  b18;
} A1;

typedef struct {
    u8  pad00[12];
    u8  b12;
} A0;

typedef struct {
    u8  b0;
    u8  b1;
    u8  pad02[59];
    u8  b61;
    u16 h62;
    u8  pad40[24];
} Ent;

extern St  *tt_player_wk;
extern A1  *tt_step_wk;
extern A0  *tt_scroll_wk;
extern Hd  *tt_cel_buf;
extern Ent *tt_rec_pool;
extern u8   tt_player_dur_tbl[];

void tt_player()
{
    s32 unused0;
    A1 *a1;
    A0 *a0;
    s32 unused1;
    St *p;
    Hd *hd;
    Ent *e;
    s32 i;
    s32 u2;
    s32 u3;

    p = tt_player_wk;
    hd = tt_cel_buf;
    a1 = tt_step_wk;
    a0 = tt_scroll_wk;
    switch (p->h4) {
    case 32:
        tt_board_step();
        if (p->b13 != 0) {
            p->b13 = 0;
            p->h98 = 0;
            tt_rec_clear();
            tt_recs_clear();
            p->h4 = 48;
            p->h6 = p->h8 = p->h10 = 0;
            p->b13 = p->b14 = p->b15 = 0;
        }
        break;
    case 48:
        tt_player_pad_step();
        if (p->h98 >= 90) {
            p->b82 = 0;
            p->h80 &= 0xFFFD;
            p->h4 = 64;
            p->h6 = p->h8 = p->h10 = 0;
            p->b13 = p->b14 = p->b15 = 0;
        } else {
            p->h98++;
        }
        break;
    case 64:
        tt_player_pad_step();
        break;
    case 96:
        p->w40 = 0;
        p->w44 = 0x10000;
        anim_cel_step(&p->b56);
        anim_cel_step(&hd->b56);
        if (p->b64 & 1) {
            p->w40 = 0;
            p->w44 = 0;
            p->h4 = 97;
        }
        break;
    case 97:
        if (tt_scroll_wk->b12 == 0) {
            a1->b18 = 0;
            a1->h0 = 5;
            a1->h4 = 0;
        } else {
            a0->b12--;
            tt_sprite_clear();
        }
        tt_scene_init();
        p->b0 = 0;
        p->w32 = 0;
        p->w36 = 0xF80000;
        p->w40 = 0;
        p->w44 = 0;
        p->h20 = 0;
        p->h22 = 248;
        p->h4 = 32;
        p->h6 = p->h8 = p->h10 = 0;
        p->b13 = p->b14 = p->b15 = 0;
        break;
    case 160:
        tt_panel_wipe();
        p->b62 = 0;
        hd->b62 = 5;
        hd->b158 = 10;
        if (p->b13 != 0) {
            a1->h0 = 4;
            a1->h2 = a1->h4 = a1->h6 = 0;
            a1->b9 = a1->b10 = a1->b11 = 0;
        }
        break;
    }

    tt_pos_integrate();
    if (p->h4 >= 96) {
        return;
    }
    if (p->b88 == 0 && p->b89 == 0) {
        p->b87 = 0;
    } else if (p->b88 >= 7) {
        p->b87 = 2;
    } else if (p->b88 >= 3) {
        p->b87 = 1;
    } else if (p->b89 >= 7) {
        p->b87 = 4;
    } else if (p->b89 >= 3) {
        p->b87 = 3;
    }

    p->b62 = p->b87;
    hd->b62 = p->b87 * 10 + p->b90 + 5;
    hd->b158 = p->b87 * 10 + p->b91 + 10;

    if (p->b92 != 0) {
        p->b94++;
        if (tt_player_dur_tbl[p->b90] < p->b94) {
            p->b94 = 0;
            p->b90++;
            if (p->b90 >= 5) {
                p->b90 = 0;
                p->b92 = 0;
            }
        }
        e = tt_rec_pool;
        for (i = 0; i < 8; i++, e++) {
            if (e->b0 != 0 && e->b61 == 0 && p->b90 == 4 && p->b94 == 1) {
                e->b1 = 0;
                e->h62 = 0;
            }
        }
    }

    if (p->b93 != 0) {
        p->b95++;
        if (tt_player_dur_tbl[p->b91] < p->b95) {
            p->b95 = 0;
            p->b91++;
            if (p->b91 >= 5) {
                p->b91 = 0;
                p->b93 = 0;
            }
        }
        e = tt_rec_pool;
        for (i = 0; i < 8; i++, e++) {
            if (e->b0 != 0 && e->b61 == 1 && p->b91 == 4 && p->b95 == 1) {
                e->b1 = 0;
                e->h62 = 0;
            }
        }
    }
}
