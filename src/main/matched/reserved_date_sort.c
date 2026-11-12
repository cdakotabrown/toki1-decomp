/* Address: 0x8006B174 */
/* Size: 1268 bytes (317 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=4 IDO52_FORCE_WEB_COLOR=47:a0,7:a1,3:a2,59:a3,254:t0,148:t1,66:v1 IDO52_AS1_XBB_SKIP_CAND=3,5 IDO52_UGEN_TEMP_REG=62:t6,65:t6,70:t8,77:t6,84:t7,23:t8,24:t9,27:t6,52:t6,56:t9,109:t7,110:t6,113:t9,91:t8,93:t6,101:t9,99:t8 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x124 --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x424 --offset2 0x428 */
/* @PostProc: swap_instructions --offset1 0x42c --offset2 0x430 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x21c */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x220 */
#include "types.h"

typedef struct {
    u32 lo : 12;
    u32 kind : 4;
    u32 level : 5;
    u32 mid : 8;
    u32 active : 1;
    u32 top : 2;
} Club;

extern Club screen_w[];

#define CLUB(i) screen_w[(i) + 339]
extern u8 now_month;
extern void bzero();

reserved_date_sort()
{
    s32 i;
    s32 j;
    s32 n;
    s32 best;
    Club tmp;
    s32 score[8];
    s32 bestIdx;
    s32 pad[10];

    n = 0;
    for (j = 0; j < 8; j++) {
        if (CLUB(j).active) {
            CLUB(n) = CLUB(j);
            n++;
        }
    }
    for (j = n; j < 8; j++) {
        bzero(&CLUB(j), 4);
    }
    if (now_month == 12) {
        for (j = 0; j < n; j++) {
            if (CLUB(j).kind == 1) {
                CLUB(j).kind = 13;
            }
        }
    }
    for (i = 0; i < n; i++) {
        best = CLUB(i).kind * 100 + CLUB(i).level;
        bestIdx = i;
        for (j = i + 1; j < n; j++) {
            score[j] = CLUB(j).kind * 100 + CLUB(j).level;
            if ((s32)((u32 *)score)[j] < best) {
                best = score[j];
                bestIdx = j;
            }
        }
        tmp = CLUB(i);
        CLUB(i) = CLUB(bestIdx);
        CLUB(bestIdx) = tmp;
    }
    for (j = 0; j < n; j++) {
        if (CLUB(j).kind == 13) {
            CLUB(j).kind = 1;
        }
    }
}
