/* @CompileEnv: IDO52_COPYPROP_VAR=-32 */
/* @CompileFlags: -cckr */
/* @SymbolSize: 932 */
/* racket_whip_step - TT overlay, Address: 0x80141090, Size: 932 bytes (233 insns) */
/* `@SymbolSize: 932` (the needed .sym size; 12 bytes are alignment pad), */
#include "types.h"

typedef struct {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s32 minLen;
    s32 maxLen;
    u8 pad10[2];
    s16 aim;
    u8 pad14[4];
    s16 step;
    u8 n;
    u8 hist;
    s32 X[32];
    s32 Y[32];
    s32 len[32];
    s32 B[32][16];
    s16 ang[32];
    s16 A[32][16];
    s16 dl[32];
} Whip;

extern s32 rsin(s32 a);
extern s32 rcos(s32 a);
extern s32 SquareRoot0(s32 v);
extern s32 get_angle(s32 dy, s32 dx);

void racket_whip_step(Whip *q)
{
    s32 n;
    s32 dx;
    s32 dy;
    s32 tgt;
    s32 d;
    s32 ad;
    s32 lim;
    s32 st;
    s32 i;
    s32 k;
    s32 m;
    s32 t;

    n = q->n;
    dx = q->x0 - q->x1;
    dy = q->y0 - q->y1;
    q->aim = get_angle(dy, dx);
    q->aim = q->aim & 0xFFF;
    q->ang[n] = q->ang[n] & 0xFFF;
    tgt = q->aim;
    d = q->ang[n] - tgt;
    if (d < 0) {
        ad = -d;
    } else {
        ad = d;
    }
    lim = q->step;
    if (lim >= ad) {
        q->ang[n] = tgt;
    } else if (tgt - q->ang[n] < 0) {
        st = (s16)-lim;
        if ((tgt >= 3073 && q->ang[n] < 1024) || (tgt < 1024 && q->ang[n] >= 3073)) {
            q->ang[n] = q->ang[n] + (s16)-st;
            return;
        }
        q->ang[n] = q->ang[n] + st;
    } else {
        st = (s16)lim;
        if ((tgt >= 3073 && q->ang[n] < 1024) || (tgt < 1024 && q->ang[n] >= 3073)) {
            q->ang[n] = q->ang[n] + (s16)-st;
            return;
        }
        q->ang[n] = q->ang[n] + st;
    }
    q->ang[n] = q->ang[n] & 0xFFF;
    q->X[0] = q->x0 << 16;
    q->Y[0] = q->y0 << 16;
    q->len[n] = (SquareRoot0(dx * dx + dy * dy) << 16) / 4096;
    if (q->len[n] < q->minLen) {
        q->len[n] = q->minLen;
    }
    if (q->maxLen < q->len[n]) {
        q->len[n] = q->maxLen;
    }
    for (i = 1; i <= n; i++) {
        q->A[i][q->hist] = q->ang[i];
        q->B[i][q->hist] = q->len[i];
        for (k = 0; k < q->hist; k++) {
            q->A[i][k] = q->A[i][k + 1];
            q->B[i][k] = q->B[i][k + 1];
        }
        q->ang[i - 1] = q->A[i][0];
        q->len[i - 1] = q->B[i][0];
    }
    m = n - 1;
    for (i = 1; i < n; i++) {
        q->dl[i] = q->ang[i] - q->ang[i - 1];
        t = rcos(q->ang[i]);
        q->X[i] = q->X[i - 1] - q->len[i] * t / m;
        t = rsin(q->ang[i]);
        q->Y[i] = q->Y[i - 1] - q->len[i] * t / m;
    }
}
