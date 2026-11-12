/* Address: 0x8004BAD8 (NEW symbol: nothing `jal`s it; the recorded symbols */
/* Size: 1072 bytes (268 instructions); the flag-set arm returns at */
/* Region: Game (IDO 5.2 -O2) */
/* @SymbolStart: 0x8004BAD8 */
/* @SymbolSize: 1072 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:a3,5:v1,8:a2,14:t0,265:t0 IDO52_NO_LOOP_TEST_REPLACEMENT=occ:3 */
/* @CompileFlags: -cckr -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u16 a : 5;
    u16 b : 5;
    u16 c : 5;
    u16 pad : 1;
} Packed;

typedef struct {
    Packed r0;
    Packed r1;
} PackedPair;

void make_color_bar(tbl, flag)
Packed tbl[8][32];
s32 flag;
{
    s32 j;
    s32 k;
    s32 n;
    PackedPair *pr;
    s32 f;

    f = flag;

    for (j = 0, k = -1; k < 31; k += 2, j += 2) {
        tbl[0][j + 1].a = j;
        tbl[1][j + 1].a = j;
        tbl[2][j + 1].a = 0;
        tbl[3][j + 1].a = 0;
        tbl[4][j + 1].a = j;
        tbl[5][j + 1].a = j;
        tbl[6][j + 1].a = 0;
        tbl[7][j + 1].a = 0;
        tbl[0][j].a = k;
        tbl[1][j].a = k;
        tbl[2][j].a = 0;
        tbl[3][j].a = 0;
        tbl[4][j].a = k;
        tbl[5][j].a = k;
        tbl[6][j].a = 0;
        tbl[7][j].a = 0;
    }
    for (j = 0, k = -1; k < 31; k += 2, j += 2) {
        tbl[0][j + 1].b = j;
        tbl[1][j + 1].b = j;
        tbl[2][j + 1].b = j;
        tbl[3][j + 1].b = j;
        tbl[4][j + 1].b = 0;
        tbl[5][j + 1].b = 0;
        tbl[6][j + 1].b = 0;
        tbl[7][j + 1].b = 0;
        tbl[0][j].b = k;
        tbl[1][j].b = k;
        tbl[2][j].b = k;
        tbl[3][j].b = k;
        tbl[4][j].b = 0;
        tbl[5][j].b = 0;
        tbl[6][j].b = 0;
        tbl[7][j].b = 0;
    }
    for (j = 0, k = -1; k < 31; k += 2, j += 2) {
        tbl[0][j + 1].c = j;
        tbl[1][j + 1].c = 0;
        tbl[2][j + 1].c = j;
        tbl[3][j + 1].c = 0;
        tbl[4][j + 1].c = j;
        tbl[5][j + 1].c = 0;
        tbl[6][j + 1].c = j;
        tbl[7][j + 1].c = 0;
        tbl[0][j].c = k;
        tbl[1][j].c = 0;
        tbl[2][j].c = k;
        tbl[3][j].c = 0;
        tbl[4][j].c = k;
        tbl[5][j].c = 0;
        tbl[6][j].c = k;
        tbl[7][j].c = 0;
    }
    if (f) {
        pr = (PackedPair *)tbl;
        for (n = 0; n < 128; n++) {
            pr[n].r1.pad = 1;
            pr[n].r0.pad = 1;
        }
    } else {
        pr = (PackedPair *)tbl;
        for (n = 0; n < 128; n++) {
            pr[n].r1.pad = 0;
            pr[n].r0.pad = 0;
        }
    }
}
