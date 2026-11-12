/* date2_serifu_set23 - DATE2 overlay, Address: 0x80136CEC, Size: 40 bytes (10 insns) */
#include "types.h"

extern s16 date2_now_s;

void sub_sub_step_up(void);

void date2_serifu_set23(void)
{
    date2_now_s = 23;
    sub_sub_step_up();
}
