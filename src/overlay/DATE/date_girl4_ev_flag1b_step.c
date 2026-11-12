/* @SymbolStart: 0x80159154 */
/* @SymbolSize: 72 */
/* Function: date_girl4_ev_flag1b_step */
/* Overlay: DATE */
/* Address: 0x80159154 (cross-prologue head) / 0x8015915C (body) */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern u8 ev_flag1b;
extern u16 now_serifuev;
extern u16 now_serifuev_in;

extern void sub_sub_step_up(void);

void date_girl4_ev_flag1b_step(void)
{
    if (ev_flag1b != 0) {
        now_serifuev = now_serifuev + 1;
        now_serifuev_in = 0;
    }
    sub_sub_step_up();
}
