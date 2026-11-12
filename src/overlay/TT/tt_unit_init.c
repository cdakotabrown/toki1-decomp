/* @CompileFlags: -cckr */
#include "types.h"

struct Rec {
    char *img;
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char pad63;
    unsigned char d;
    unsigned char e;
    unsigned char pad66;
    unsigned char f;
    unsigned char pad68[4];
    unsigned char g;
    unsigned char pad73[3];
};

struct Obj {
    unsigned char pad0[56];
    struct Rec rec[26];
    unsigned char pad576[8];
    short x[24];
    short y[24];
    short h[24];
    unsigned char pad728[56];
    unsigned char flag[8];
};

extern struct Obj *tt_boss_wk;
extern char tt_anim_tbl[];
extern short tt_unit_y_tbl[];

void tt_unit_init()
{
    struct Obj *p;
    int k;

    p = tt_boss_wk;
    for (k = 2; k < 7; k++) {
        p->rec[k].img = tt_anim_tbl;
        p->rec[k].a = 0;
        p->rec[k].b = 6;
        if (k == 6) {
            p->rec[k].c = 20;
        } else {
            p->rec[k].c = 31;
        }
        p->rec[k].d = 96;
        p->rec[k].e = 0;
        p->rec[k].f = 15;
        p->rec[k].g = 0;
        p->flag[k] = 1;
        p->h[k] = 9;
        if (k == 6) {
            p->x[k] = 0;
        } else {
            p->x[k] = k * 16 - 56;
        }
        p->y[k] = tt_unit_y_tbl[k - 2];
    }
}
