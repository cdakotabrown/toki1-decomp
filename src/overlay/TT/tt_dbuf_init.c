/* tt_dbuf_init - TT overlay, Address: 0x8013E68C, Size: 388 bytes (97 insns) */
/* @SymbolSize: 388 */
#include "types.h"

typedef struct {
    s32 ot;
    u8  pad4[32836];
    s16 clipX;
    s16 clipY;
    s16 clipW;
    s16 clipH;
    s16 ofsX;
    s16 ofsY;
    u8  pad32852[10];
    u8  dtd;
    u8  dfe;
    u8  isbg;
    u8  pad32865[83];
    u8  isinter;
    u8  isrgb24;
    u8  pad32950[2];
} TTBuffer;

extern u8 *tt_game_wk;
extern u8 *tt_db_buf;

extern void SetDispMask(s32 a0);
extern void SetDefDrawEnv(u8 *db, s32 x, s32 y, s32 w, s32 h);
extern void SetDefDispEnv(u8 *db, s32 x, s32 y, s32 w, s32 h);

void tt_dbuf_init(void)
{
    u8 *q;
    TTBuffer *w;
    s32 x;
    s32 y;

    q = tt_game_wk;
    w = (TTBuffer *)tt_db_buf;
    SetDispMask(0);
    SetDefDrawEnv((u8 *)w + 32840, 0, 0, 320, 240);
    SetDefDrawEnv((u8 *)w + 65792, 320, 0, 320, 240);
    if (q[73] != 0) {
        SetDefDispEnv((u8 *)w + 32932, 352, 0, 256, 240);
        SetDefDispEnv((u8 *)w + 65884, 32, 0, 256, 240);
    } else {
        SetDefDispEnv((u8 *)w + 32932, 320, 0, 320, 240);
        SetDefDispEnv((u8 *)w + 65884, 0, 0, 320, 240);
    }
    y = 160;
    x = 32;
    do {
        *(s16 *)((u8 *)w + 32848) = y;
        w->clipX = x;
        w->clipY = 8;
        w->clipW = 256;
        w->clipH = 232;
        w->isbg = 0;
        w->dtd = 0;
        w->dfe = 1;
        w->isinter = 0;
        w->isrgb24 = 0;
        x += 320;
        w->ot = (s32)((u8 *)w + 8);
        w++;
        y += 320;
    } while (x != 672);
}
