/* @CompileEnv: IDO52_FORCE_WEB_COLOR=45:v1,46:a1,42:t0,229:t1,223:t3,225:t4,227:t5,67:a1,82:a1,112:t2,30:a3,32:a2,9:t2 IDO52_AS1_TIE_REJECT=427,654 */
/* @SymbolStart: 0x80147CB0 */
/* @SymbolSize: 1020 */
/* @CompileFlags: -cckr -Wc,-pic0 */
#include "types.h"

typedef union {
    s32 l;
    struct {
        u16 lo;
        s16 hi;
    } w;
} Fix;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 pad4[20];
    s16 h24;
    u8 pad1A[10];
    Fix x;
    Fix y;
    u8 pad2C[24];
} Actor;

typedef struct {
    s32 w0;
    s32 w4;
    u8 pad8[18];
    s16 h26;
} Rec28;

typedef struct { s16 h[10]; } Blk20;
typedef struct { s16 h[8]; } Blk16;

extern Blk20 taiiku_racer_pos_set2_tbl0;
extern Blk20 taiiku_racer_pos_set2_tbl1;
extern Blk16 taiiku_racer_pos_set2_tbl2;
extern Blk16 taiiku_racer_pos_set2_tbl3;

extern Actor class_w[];
extern Rec28 taiiku_spoon_cpu[];
extern Rec28 taiiku_spoon_bal[];
extern s16 taiiku_spoon_scrl_hi;
extern u16 taiiku_spoon_pose_n[];
extern u16 taiiku_spoon_pose_r[];
extern u16 taiiku_spoon_pose_l[];

void taiiku_racer_pos_set2(idx)
    s32 idx;
{
    s32 j;
    Fix x;
    Fix y;
    Blk20 b1;
    Blk20 b2;
    Blk16 b3;
    Blk16 b4;
    s32 i;
    Fix *xp;
    Fix *yp;
    s32 k;
    s32 key;
    Rec28 *rec;

    b1 = taiiku_racer_pos_set2_tbl0;
    b2 = taiiku_racer_pos_set2_tbl1;
    b3 = taiiku_racer_pos_set2_tbl2;
    b4 = taiiku_racer_pos_set2_tbl3;
    yp = (Fix *)&rec;
    k = class_w[idx + 96].x.l;
    rec = &taiiku_spoon_bal[idx];
    k += rec->w0;
    y.l = class_w[idx + 96].y.l;
    y.l += rec->w4;
    x.l = k;
    k = idx * 19;
    key = class_w[idx + 96].h24;
    yp = &y;
    xp = &x;
    for (i = 0; i < 8; i++) {
        if (key == taiiku_spoon_pose_n[i] + k) {
            yp->w.hi += b1.h[i];
            xp->w.hi += b2.h[i];
        }
    }
    for (i = 2; i < 8; i++) {
        if (key == taiiku_spoon_pose_r[i] + k) {
            xp->w.hi += 22;
            yp->w.hi -= 4;
            yp->w.hi += b3.h[i];
        }
    }
    for (i = 2; i < 8; i++) {
        if (key == taiiku_spoon_pose_l[i] + k) {
            xp->w.hi -= 12;
            yp->w.hi -= 4;
            yp->w.hi += b4.h[i];
        }
    }
    class_w[idx + 100].x.w.hi = xp->w.hi;
    class_w[idx + 100].y.w.hi = yp->w.hi;
    goto L1;
L1:
    rec = taiiku_spoon_cpu;
    for (j = 0; j < 3; j++) {
        if (rec[j].h26 - taiiku_spoon_scrl_hi > 480 || rec[j].h26 - taiiku_spoon_scrl_hi < -160) {
            class_w[j + 101].b3 = 0;
        } else {
            class_w[j + 101].b3 = 164;
        }
    }
}
