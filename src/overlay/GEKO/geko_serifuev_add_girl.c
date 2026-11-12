/* @SymbolStart: 0x80141EE8 */
/* @SymbolSize: 56 */
/* geko_serifuev_add_girl -- GEKO overlay, Address: 0x80141EF8, Size: 40 bytes (10 insns) */
#include "types.h"
extern u8 now_girl;

extern void sub_sub_step_up(void);
extern u16 now_serifuev;

void geko_serifuev_add_girl(void)
{
    s32 hidden_t6;
    s32 hidden_t7;

    hidden_t6 = now_serifuev;
    hidden_t7 = now_girl;

    now_serifuev = (s16)(hidden_t6 + hidden_t7);
    sub_sub_step_up();

}
