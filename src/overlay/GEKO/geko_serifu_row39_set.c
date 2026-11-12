/* geko_serifu_row39_set -- GEKO overlay, Address: 0x80133FE8, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s16 geko_now_serifu;

void geko_serifu_row39_set(void)
{
    geko_now_serifu = 39;
    sub_sub_step_up();
}
