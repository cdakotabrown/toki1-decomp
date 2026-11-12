/* Address: 0x80133CE0 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern s16 shougatu_meet_now_serifu;

void shougatu_serifu15_step(void)
{
    shougatu_meet_now_serifu = 15;
    sub_sub_step_up();
}
