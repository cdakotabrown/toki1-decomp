/* geko_serifuev_next3 -- GEKO overlay, Address: 0x80141978, Size: 64 bytes (16 insns), Matched */
#include "types.h"

extern void geko_serifuev_run(void);
extern void sub_sub_step_up(void);

extern u16 now_serifuev;
extern s16 now_serifuev_in;

void geko_serifuev_next3(void)
{
    geko_serifuev_run();
    now_serifuev = (now_serifuev + 1);
    now_serifuev_in = 0;
    sub_sub_step_up();
}
