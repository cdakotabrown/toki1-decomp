/* Address: 0x8005680C */
/* Size: 240 bytes (60 instructions) */
/* Region: Game (IDO 5.2) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    s16 x, y, w, h;
} MvRect;

typedef struct {
    u32 *vlcbuf[2];
    s32 vlcid;
    u16 *imgbuf;
    MvRect rect[2];
    s32 rectid;
    MvRect slice;
    s32 isdone;
} MvDec;

typedef struct {
    u8 head[0x10044];
    u16 imgbuf[0x2D00 / 2];
    u32 vlcbuf0[0x36000 / 4];
    u32 vlcbuf1[0x36000 / 4];
} MvWork;

#define setRECT(r, _x, _y, _w, _h) \
    (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)

extern MvWork *movie_state;
extern s32 movie_main_tbl1;

void strSetDefDecEnv(MvDec *dec, s32 x0, s32 y0, s32 x1, s32 y1)
{
    dec->vlcbuf[0] = movie_state->vlcbuf0;
    dec->vlcbuf[1] = movie_state->vlcbuf1;
    dec->vlcid = 0;
    dec->imgbuf = movie_state->imgbuf;
    dec->rectid = 0;
    dec->isdone = 0;
    if (movie_main_tbl1 & 1) {
        setRECT(&dec->rect[0], x0, y0, 480, 240);
        setRECT(&dec->rect[1], x1, y1, 480, 240);
        setRECT(&dec->slice, x0, y0, 24, 240);
    } else {
        setRECT(&dec->rect[0], x0, y0, 320, 240);
        setRECT(&dec->rect[1], x1, y1, 320, 240);
        setRECT(&dec->slice, x0, y0, 16, 240);
    }
}
