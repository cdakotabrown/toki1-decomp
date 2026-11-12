/* @SymbolStart: 0x8014629C */
/* @SymbolSize: 156 */
#include "types.h"

typedef struct { u8 pad[6]; s16 f6; u8 pad2[12]; } S20;
typedef struct { u8 pad[68]; s16 f68; u8 pad2[30]; } S100;

extern u8 *tt_step_wk;
extern S20 *tt_text_buf;
extern S100 *tt_anim_buf;

extern void tt_board_scale_step(void);

void tt_fade_in20(void)
{
    u8 *p;
    s32 i;
    s32 v;
    S20 *q;
    S100 *r;

    p = tt_step_wk;
    v = (u8)(p[29] += 4);
    p[31] = v;
    p[30] = v;
    if ((u32)v >= 128U) {
        q = tt_text_buf;
        for (i = 0; i < 128; i++) {
            q++;
            q[-1].f6 = 0;
        }
        r = tt_anim_buf;
        for (i = 0; i < 128; i++) {
            r->f68 = 0;
            r++;
        }
        *(s16 *)(p + 2) = 64;
    }
    tt_board_scale_step();
}
