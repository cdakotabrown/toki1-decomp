/* @SymbolStart: 0x8013EF8C */
/* @SymbolSize: 140 */
/* geko_serifuev_ev1b_add -- GEKO overlay, Address: 0x8013EF94, Size: 132 bytes (33 insns) */
#include "types.h"

extern void sub_sub_step_up(void);

extern u16 now_serifuev;
extern s16 now_serifuev_in;
extern volatile u8 ev_flag1b;
extern u8 ev_flag1c;

void geko_serifuev_ev1b_add(void)
{
    s32 t6 = ev_flag1c;

    if (t6 == 1 && ev_flag1b == 1) {
        now_serifuev = now_serifuev + 4;
        now_serifuev_in = 0;
    } else if (ev_flag1b == 2) {
        now_serifuev = now_serifuev + 2;
        now_serifuev_in = 0;
    }
    sub_sub_step_up();
}
