/* geko_serifuev_set12 -- GEKO overlay, Address: 0x80141F20, Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern s16 now_serifuev;

void geko_serifuev_set12(void)
{
    now_serifuev = 12;
    sub_sub_step_up();
}
