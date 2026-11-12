#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

extern u8 *tt_step_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_db_buf;

void tt_fill_disp(void)
{
    u8 *base;
    PTag *ot;
    u8 *res;
    u8 *p;

    res = tt_step_wk;
    base = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    p = *(u8 **)(base + 4);

    *(u8 **)(base + 4) += 16;
    p[3] = 3; p[7] = 97;
    p[4] = res[19];
    p[5] = res[20];
    p[6] = res[21];
    ot = (PTag *)(base + 0x8000);
    *(s16 *)(p + 8) = -128; *(s16 *)(p + 10) = 8;
    *(s16 *)(p + 12) = 256; *(s16 *)(p + 14) = 232;
    ((PTag *)p)->addr = ot[17].addr;
    ot[17].addr = (u32)p;
}
