/* @SymbolStart: 0x8013E5BC */
/* @SymbolSize: 56 */
/* geko_serifuev_add -- GEKO overlay, Address: 0x8013E5CC, Size: 40 bytes (10 insns) */
#include "types.h"
extern s32 serifu_no;

extern void sub_sub_step_up(void);

extern u16 now_serifuev;

void geko_serifuev_add(void)
{
    s32 t6;
    s32 t7;

    t6 = now_serifuev;
    t7 = serifu_no;

    now_serifuev = (s16)(t6 + t7);
    sub_sub_step_up();

}
