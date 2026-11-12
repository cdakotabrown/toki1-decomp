/* Function: grp_table_calc */
/* Address: 0x80053374 */
/* Size: 1148 bytes */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80053364 */
/* @SymbolSize: 1164 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=4:a2,46:a3,51:t4,53:t3,102:t0,101:a3 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 pad0;
    s16 val;
    u8 pad4[8];
    u8 flags[36];
    u8 pad30[8];
} Rec;

typedef struct {
    u8 pad0[452];
    Rec recs[11];
} Big;

extern Big screen_w;
typedef struct {
    s16 x;
    s16 y;
} Pair;

extern Pair grp_tbl[];
extern s16 para_str[];
extern s8 grp_table_calc_tbl0[];
extern u8 grp_table_calc_tbl1[];
extern u8 grp_table_calc_tbl2[];
extern s8 grp_table_calc_tbl3[];
extern s8 grp_table_calc_tbl4[];

void grp_table_calc(input, out)
u8 *input;
s16 *out;
{
    s32 i;
    s32 j;
    s32 k;
    s32 v;
    s32 sum;
    u32 bitsA;
    u32 bitsB;
    s32 shift;
    s16 buf[11];
    s8 *w;

    for (k = 1; k < 11; k++) {
        buf[k] = input[k] << 2;
        w = &grp_table_calc_tbl0[k];
        buf[k] += screen_w.recs[k].val * *w;
    }
    i = para_str[0];
    if (i > 140) {
        i = 140;
    }
    buf[2] += grp_table_calc_tbl4[i / 10];
    for (j = 1; j < 11; j++) {
        bitsA = grp_table_calc_tbl1[j];
        bitsB = grp_table_calc_tbl2[j];
        for (i = 0; i < 7; i++) {
            shift = 7 - i;
            if ((bitsA >> shift) & 1) {
                v = grp_tbl[0 - i + 63].y;
                if (i == 7) {
                    v = v / 20;
                } else {
                    v = v / 10;
                }
                if (v > 14) {
                    v = 14;
                }
                buf[j] += grp_table_calc_tbl3[v];
            }
            if ((bitsB >> shift) & 1) {
                v = grp_tbl[0 - i + 63].y;
                if (i == 7) {
                    v = v / 20;
                } else {
                    v = v / 10;
                }
                if (v > 14) {
                    v = 14;
                }
                buf[j] += grp_table_calc_tbl3[v];
            }
        }
    }
    for (i = 1; i < 11; i++) {
        sum = 0;
        for (j = 1; j < 36; j++) {
            sum += screen_w.recs[i].flags[j];
        }
        if (sum >= 4) {
            buf[i] += sum * 10;
        } else {
            buf[i] = 0;
        }
    }
    for (i = 1; i < 11; i++) {
        out[i] = buf[i];
    }
}
