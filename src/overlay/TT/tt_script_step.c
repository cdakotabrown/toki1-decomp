/* @CompileFlags: -cckr */
/* tt_script_step - TT overlay, Address: 0x8013FBC0, Size: 836 bytes (209 insns) */
#include "types.h"

typedef struct {
    s32 *ptr;
    u8 b4;
    u8 b5;
    u8 pad6[2];
    u8 b8;
    u8 pad9;
    u8 b10;
    u8 padB[5];
    u8 b16;
    u8 pad11[3];
} Rec20;

typedef struct {
    u8 pad0[4];
    s16 f4;
    u8 pad6[2];
    u16 f8;
    u8 padA[46];
    Rec20 rec[26];
    u8 pad248[8];
    s16 a[24];
    s16 b[24];
    s16 c[24];
    u8 pad2D8[56];
    u8 flag[80];
    u8 pad360[4];
    u16 f868;
    u8 pad86A[110];
    u16 f980;
} TTWork;

extern TTWork *tt_boss_wk;
extern s32 tt_anim_tbl;
extern s32 tt_script_seq_tbl;
extern s32 tt_puff_seq_tbl;
extern s16 tt_script_scr[];

tt_script_step()
{
    TTWork *p;
    s32 i;
    s32 k;
    s32 j;

    p = tt_boss_wk;
    switch (p->f8) {
    case 0:
        tt_cmd_once(1281);
        p->f868 = 0;
        p->f980 = 0;
        for (i = 3; i < 9; i++) {
            p->rec[i].ptr = &tt_anim_tbl;
            p->rec[i].b4 = 0;
            p->rec[i].b5 = 4;
            p->rec[i].b8 = 128;
            p->rec[i].b10 = 0;
            p->rec[i].b16 = 0;
            p->c[i] = 6;
        }
        p->a[3] = -38;
        p->b[3] = 15;
        p->a[4] = 38;
        p->b[4] = 15;
        p->a[5] = -44;
        p->b[5] = 10;
        p->a[6] = 44;
        p->b[6] = 10;
        p->a[7] = -48;
        p->b[7] = 0;
        p->a[8] = 48;
        p->b[8] = 0;
        for (i = 3; i < 9; i++) {
            tt_seq_step(&tt_script_seq_tbl, i);
        }
        for (i = 16; i != 19; i++) {
            p->rec[i].ptr = &tt_anim_tbl;
            p->rec[i].b4 = 0;
            p->rec[i].b5 = 4;
            p->rec[i].b10 = 0;
            p->rec[i].b16 = 0;
            p->flag[i] = 0;
            p->c[i] = 8;
            p->a[i] = 0;
            p->b[i] = 0;
        }
        p->f8 = 64;
        break;
    case 64:
        k = tt_script_scr[tt_boss_wk->f980];
        if (k != -1 && k == tt_boss_wk->f868) {
            tt_cmd_once(1281);
            j = tt_script_scr[p->f980 + 4];
            p->flag[j] = 1;
            p->rec[j].b8 = 128;
            p->a[j] = tt_script_scr[p->f980 + 1];
            p->b[j] = tt_script_scr[p->f980 + 2];
            p->c[j] = tt_script_scr[p->f980 + 3];
            p->f980 += 5;
        }
        for (i = 3; i < 9; i++) {
            tt_seq_step(&tt_script_seq_tbl, i);
        }
        for (i = 16; i < 19; i++) {
            tt_seq_step(&tt_puff_seq_tbl, i);
        }
        if (p->f868 == 143) {
            for (i = 0; i != 16; i++) {
                p->flag[i] = 0;
            }
        }
        if (p->f868 == 166) {
            for (i = 0; i != 24; i++) {
                p->flag[i] = 0;
            }
            p->f8 = 0;
            p->f4 = 160;
        }
        break;
    }
    p->f868++;
}
