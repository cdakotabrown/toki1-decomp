/* @CompileFlags: -cckr */
#include "types.h"

typedef struct {
    u8  pad000[6];
    u16 h6;
    u16 h8;
    u8  pad00A[10];
    s16 x;
    s16 y;
    u8  pad018[238];
    u8  b262;
    u8  pad107[607];
    u16 h870;
} Serve;

typedef struct {
    u8  pad00[20];
    s16 x;
    s16 y;
} Player;

typedef struct {
    s32 pad[3];
    s32 x;
    s32 y;
    s32 z;
    s32 w[2];
} Pos;

extern Serve  *tt_boss_wk;
extern Player *tt_player_wk;
extern s32 tt_spark_ofs_tbl[];
extern s32 tt_spark_ofs_tbl_1_0[];
extern s32 tt_spark2_ofs_tbl[];
extern s32 tt_spark2_ofs_tbl_1_0[];

void tt_serve_effect_step()
{
    Serve *p;
    Player *q;
    s32 a;
    Pos pos;
    s32 r;
    s32 i;

    p = tt_boss_wk;
    q = tt_player_wk;
    switch (p->h8) {
    case 0:
        p->h870 = 0;
        p->h8 = 16;
        break;
    case 16:
        if (tt_boss_wk->h870 >= 60) {
            p->h8 = 64;
        }
        break;
    case 64:
        if (tt_boss_wk->h870 < 128) {
            if (tt_player_wk->y < 120) {
                p->h870 = 128;
            }
            if ((p->h870 & 0x1F) == 31) {
                tt_cmd_once(1280);
                if (p->b262 == 0) {
                    for (i = 0; i < 5; i++) {
                        sparkle_make(p->x - tt_spark_ofs_tbl[i], tt_spark_ofs_tbl_1_0[i] + p->y);
                        sparkle_make(tt_spark_ofs_tbl[i] + p->x, tt_spark_ofs_tbl_1_0[i] + p->y);
                    }
                } else {
                    for (i = 0; i < 4; i++) {
                        sparkle_make(p->x - tt_spark2_ofs_tbl[i], tt_spark2_ofs_tbl_1_0[i] + p->y);
                        sparkle_make(tt_spark2_ofs_tbl[i] + p->x, tt_spark2_ofs_tbl_1_0[i] + p->y);
                    }
                }
            }
        } else if (p->h870 < 256) {
            if ((p->h870 & 1) == 1) {
                if ((p->h870 & 3) == 3) {
                    tt_cmd_once(1293);
                }
                tt_shot_heading_make(p->x, p->y + 8, 48, ((p->h870 & 0x7F) << 6) - 1024, 161);
            }
        } else if ((p->h870 & 0x3F) == 63) {
            tt_cmd_once(1291);
            pos.x = p->x << 16;
            pos.y = (p->y + 8) << 16;
            r = tt_angle_between(&pos, &q->x);
            a = r & 0x7F;
            for (i = -256; i < 2304; i += 128) {
                tt_shot_heading_make(p->x, p->y + 8, 72, a + i, 160);
            }
        }
        if (p->h870 >= 384) {
            p->h8 = 160;
        }
        break;
    case 160:
        p->h6 = 1;
        p->h8 = 0;
        break;
    }
    p->h870++;
}
