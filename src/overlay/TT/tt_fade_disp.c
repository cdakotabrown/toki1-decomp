/* tt_fade_disp - TT overlay, Address: 0x8013D3C0, Size: 328 bytes. */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

extern u8 *tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_db_buf;

void tt_fade_disp(void)
{
    u8 *base;
    PTag *ot;
    u8 *res;
    u8 *p;
    s32 r;
    s32 g;
    s32 b;

    res = tt_step_wk;
    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    p = *(u8 **)(base + 4);
    *(u8 **)(base + 4) = *(u8 **)(base + 4) + 16;
    p[3] = 3; p[7] = 97;
    r = (u32)(res[19] * res[29]) >> 7;
    if (r >= 256) {
        r = 255;
    }
    g = (u32)(res[20] * res[30]) >> 7;
    if (g >= 256) {
        g = 255;
    }
    b = (u32)(res[21] * res[31]) >> 7;
    if (b >= 256) {
        b = 255;
    }
    ot = (PTag *)(base + 0x8000);
    p[4] = (u8)r;
    p[5] = (u8)g;
    p[6] = (u8)b;
    *(s16 *)(p + 8) = -128;
    *(s16 *)(p + 10) = 8;
    *(s16 *)(p + 12) = 256; *(s16 *)(p + 14) = 232;
    ((PTag *)p)->addr = ot[17].addr;
    ot[17].addr = (u32)p;
}
