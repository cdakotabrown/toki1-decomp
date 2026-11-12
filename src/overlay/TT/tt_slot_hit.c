/* @CompileFlags: -cckr */
/* tt_slot_hit - TT overlay, Address: 0x80137654, Size: 548 bytes (137 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

typedef struct {
    u8  on;
    u8  pad1;
    s16 a;
    s16 b;
    u8  pad6[13];
    u8  f19;
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
    u8    pad680[44];
    s16   hp[30];
    u8    flag[50];
    u8    b834;
    u8    b835;
    u8    b836;
    u8    pad837[12];
    u8    act[6];
} Stage;

typedef struct {
    u8  on;
    u8  pad1;
    u16 kind;
    u16 state;
    s16 f6;
    s16 f8;
    s16 fA;
    u8  padC;
    u8  d;
    u8  e;
    u8  f;
    u8  pad10[4];
    s16 hx;
    s16 hy;
    u8  pad24[58];
    u8  b82;
    u8  pad83;
} Obj;

extern Stage *tt_boss_wk;
extern u8 *tt_step_wk;
extern Obj *tt_player_shot_buf;
extern TT_BOX tt_hit_box_tbl[];
extern TT_BOX tt_target_box_tbl[];

void tt_slot_hit()
{
    u8 *hit;
    Stage *p;
    s32 padA[8];
    s16 pos[2];
    Obj *obj;
    s32 k;
    s32 j;

    p = tt_boss_wk;
    hit = tt_step_wk;
    for (k = 0; k < 4; k++) {
        if (p->act[k + 2] != 0) {
            p->rec[k + 2].on = 0;
            pos[0] = p->ox[k + 2] + p->x;
            pos[1] = p->oy[k + 2] + p->y;
            obj = tt_player_shot_buf;
            for (j = 0; j < 12; j++, obj++) {
                if (obj->on == 0) {
                    continue;
                }
                if (obj->state != 64) {
                    continue;
                }
                if (tt_box_overlap_check(&obj->hx, ((u8 *)&tt_hit_box_tbl[1].x), pos, ((u8 *)&tt_target_box_tbl[3].x)) == 0) {
                    continue;
                }
                hit[37]++;
                if (hit[36] != 0 && (hit[37] & 3) == 0) {
                    tt_shot_aimed_make(obj->hx, obj->hy, 32, 160);
                }
                tt_cmd_once(1287);
                if (p->act[k + 2] != 0) {
                    if (j >= 6) {
                        p->hp[k + 2] -= 1;
                    } else if (obj->kind == 9) {
                        p->hp[k + 2] -= 4;
                    } else {
                        p->hp[k + 2] -= 2;
                    }
                    if (p->hp[k + 2] < 0) {
                        tt_cmd_once(1285);
                        p->act[k + 2] = 0;
                        p->rec[k + 1].f19 = 0;
                    }
                }
                p->rec[k + 2].on = 1;
                obj->state = 128;
                obj->d = 0;
                obj->e = 0;
                obj->f = 0;
                obj->f6 = 0;
                obj->f8 = 0;
                obj->fA = 0;
            }
        }
    }
}
