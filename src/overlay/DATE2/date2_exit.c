#include "types.h"

void goto_step(s32 arg0);
void goto_sub_step(s32 arg0);
void goto_sub_sub_step(s32 arg0);

void date2_exit(void)
{
    goto_step(80);
    goto_sub_step(1);
    goto_sub_sub_step(17);
}
