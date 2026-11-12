/* Address: 0x8004FC90 */
/* Size: 276 bytes (69 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=7:t2 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8  w;
    u8  h;
    u8  pad6;
    u8  flag;
} SlotRec;

typedef struct {
    u8 pad0[3984];
    SlotRec recs[16][2];
    u8 count[2];
    u8 pad4242[3];
    s8 cur[2];
} Big;

extern Big screen_w;

s32 menu_set(slot, count, xs, ys, ws, hs)
s32 slot;
u8 count;
s16 *xs;
s16 *ys;
s16 *ws;
s16 *hs;
{
    s32 i;
    s16 *pw;
    s16 *ph;
    s16 *px;
    s16 *py;

    for (i = 0; i < 16; i++) {
        screen_w.recs[i][slot].flag = 0;
        screen_w.recs[i][slot].h = 0;
        screen_w.recs[i][slot].w = 0;
    }
    px = xs;
    py = ys;
    pw = ws;
    ph = hs;
    if (count > 16) {
        return -1;
    }
    screen_w.count[slot] = count;
    screen_w.cur[slot] = -1;
    if (slot >= 2) {
        return -1;
    }
    for (i = 0; i < count; i++) {
        screen_w.recs[i][slot].x = *px++;
        screen_w.recs[i][slot].y = *py++;
        if (*pw * *ph == 0) {
            screen_w.recs[i][slot].flag = 1;
        } else {
            screen_w.recs[i][slot].flag = 0;
        }
        screen_w.recs[i][slot].w = *pw++;
        screen_w.recs[i][slot].h = *ph++;
        screen_w.recs[i][slot].pad6 = 0;
    }
}
