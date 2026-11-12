/* geko_serifu_sel_set20 -- GEKO overlay, Address: 0x80143BC4, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s16 geko_now_serifu3;

void geko_serifu_sel_set20(void)
{
    geko_now_serifu3 = 20;
    sub_sub_step_up();
}
