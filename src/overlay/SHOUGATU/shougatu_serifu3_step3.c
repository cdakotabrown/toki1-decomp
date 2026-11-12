/* Address: 0x801412CC */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern void sub_sub_step_up();
extern s16 shougatu_menu_now_serifu;

void shougatu_serifu3_step3(void)
{
    shougatu_menu_now_serifu = 3;
    sub_sub_step_up();
}
