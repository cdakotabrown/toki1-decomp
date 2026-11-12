/* @SymbolStart: 0x8013820C */
/* @SymbolSize: 80 */
/* Address: 0x80138220 */
#include "types.h"

extern void goto_sub_step(s32);

extern s16 taco_time_left;
extern s32 taco_score;
extern u8 *taco_actor_rec;

void taco_main8_main2(void)
{
    u8 *p;

    taco_time_left = 18000;
    p = taco_actor_rec;
    taco_score = 0;
    p[133] = 255;
    taco_actor_rec[1221] = 255;
    goto_sub_step(11);
}
