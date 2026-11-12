/* tt_tile_disp - TT overlay, Address: 0x8014CBF4, Size: 440 bytes. */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

extern u8 *tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_db_buf;

void tt_tile_disp(u8 *prm)
{
    u8 *base;
    u8 *p;
    s32 r;
    s32 g;
    s32 b;

    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    p = *(u8 **)(base + 4);
    *(u8 **)(base + 4) = p + 16;
    p[3] = 3;
    p[7] = prm[59];
    if (*(u16 *)(prm + 68) & 8) {
        r = (u32)(prm[56] * tt_step_wk[29]) >> 7;
        if (r >= 256) {
            r = 255;
        }
        g = (u32)(prm[57] * tt_step_wk[30]) >> 7;
        if (g >= 256) {
            g = 255;
        }
        b = (u32)(prm[58] * tt_step_wk[31]) >> 7;
        if (b >= 256) {
            b = 255;
        }
        p[4] = (u8)r;
        p[5] = (u8)g;
        p[6] = (u8)b;
    } else {
        p[4] = prm[56];
        p[5] = prm[57];
        p[6] = prm[58];
    }
    *(s16 *)(p + 8) = *(s16 *)(prm + 20);
    *(s16 *)(p + 10) = *(s16 *)(prm + 22);
    *(s16 *)(p + 12) = *(s16 *)(prm + 60);
    *(s16 *)(p + 14) = *(s16 *)(prm + 62);
    ((PTag *)p)->addr = ((PTag *)(base + *(u16 *)(prm + 16) * 4 + (0, 0x8000) + 8))->addr;
    ((PTag *)(base + *(u16 *)(prm + 16) * 4 + 0x8000 + 8))->addr = (u32)p;
}
