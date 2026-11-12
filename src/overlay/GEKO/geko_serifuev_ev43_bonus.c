/* @SymbolStart: 0x8013F68C */
/* @SymbolSize: 68 */
/* geko_serifuev_ev43_bonus -- GEKO overlay, Address: 0x8013F694, Size: 60 bytes (15 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u16 now_serifuev;
extern u8 ev_flag43;

void geko_serifuev_ev43_bonus(void)
{
    s32 t6 = ev_flag43;

    if ((u32)t6 >= 2) {
        now_serifuev += 5;
    }
    sub_sub_step_up();
}
