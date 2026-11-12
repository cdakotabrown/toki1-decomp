/* @SymbolSize: 884 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/tt_box.h"

typedef struct { s16 x; s16 y; } Pt;
typedef struct { s16 a; s16 b; u8 c; u8 d; } Blk6;

typedef struct {
    u8  on;
    u8  pad1;
    s16 a;
    s16 b;
    u8  pad6[14];
} Rec20;

typedef struct {
    u8    active;
    u8    pad1[19];
    s16   x;
    s16   y;
    u8    pad24[42];
    Rec20 rec[24];
    u8    pad546[38];
    s16   ox[24];
    s16   oy[24];
    u8    pad680[104];
    u8    flag[50];
    u8    b834;
    u8    b835;
    u8    b836;
} Stage;

typedef struct {
    u8  on;
    u8  pad1[3];
    u16 state;
    s16 f6;
    s16 f8;
    s16 fA;
    u8  padC;
    u8  d;
    u8  e;
    u8  f;
    u8  pad10[4];
    u8  hit[62];
    u8  b82;
    u8  pad83;
} Obj;

extern Stage *tt_boss_wk;
extern Obj *tt_player_wk;
extern Obj *tt_player_shot_buf;
extern u8 tt_hit_box_tbl[];
extern TT_BOX tt_cell_box_tbl[];

void tt_unit_hit_step()
{
    Stage *p;
    Obj *q;
    s32 pad0[9];
    Pt tmp;
    Blk6 blk;
    Obj *r;
    s32 i;
    s32 j;

    p = tt_boss_wk;
    q = tt_player_wk;
    if (p->active == 0) {
        return;
    }
    for (i = 0; i < 24; i++) {
        p->rec[i].on = 0;
    }
    if (p->b834 == 0 && tt_hit_check() != 0) {
        tt_cmd_once(1283);
    }
    for (i = 16; i < 19; i++) {
        if (p->flag[i] != 0) {
            p->rec[i].on = 0;
        }
    }
    if (p->b835 == 0) {
        for (i = 16; i < 19; i++) {
            if (p->flag[i] != 0) {
                tmp.x = p->ox[i] + p->x;
                tmp.y = p->oy[i] + p->y;
                if (q->b82 == 0 && q->on != 0 &&
                    tt_box_overlap_check(q->hit, tt_hit_box_tbl, &tmp, ((u8 *)&tt_cell_box_tbl[34].x)) != 0) {
                    tt_widget_pair_init();
                    q->state = 96;
                    q->fA = 0;
                    q->f8 = 0;
                    q->f6 = 0;
                    q->f = 0;
                    q->e = 0;
                    q->d = 0;
                }
                r = tt_player_shot_buf;
                for (j = 0; j < 12; j++, r++) {
                    if (r->on == 0) {
                        continue;
                    }
                    if (r->state != 64) {
                        continue;
                    }
                    if (tt_box_overlap_check(r->hit, ((u8 *)&((TT_BOX *)tt_hit_box_tbl)[1].x), &tmp, ((u8 *)&tt_cell_box_tbl[34].x)) == 0) {
                        continue;
                    }
                    r->state = 128;
                    r->fA = 0;
                    r->f8 = 0;
                    r->f6 = 0;
                    r->f = 0;
                    r->e = 0;
                    r->d = 0;
                }
            }
        }
    }
    if (p->b836 == 0) {
        p->rec[23].on = 0;
        if (q->on != 0) {
            tmp.x = p->ox[23] + p->x;
            tmp.y = p->oy[23] + p->y;
            blk.a = 0;
            blk.b = 0;
            blk.c = p->rec[23].a * 48 / 4096;
            blk.d = p->rec[23].b * 48 / 4096;
            if (q->b82 == 0 && q->on != 0 &&
                tt_box_overlap_check(q->hit, tt_hit_box_tbl, &tmp, &blk) != 0) {
                tt_widget_pair_init();
                q->state = 96;
                q->fA = 0;
                q->f8 = 0;
                q->f6 = 0;
                q->f = 0;
                q->e = 0;
                q->d = 0;
            }
        }
    }
}
