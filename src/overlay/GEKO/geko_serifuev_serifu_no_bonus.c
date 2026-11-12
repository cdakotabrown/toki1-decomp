/* @SymbolStart: 0x8013BF00 */
/* @SymbolSize: 64 */
/* geko_serifuev_serifu_no_bonus -- GEKO overlay, Address: 0x8013BF00, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u16 now_serifuev;
extern s32 serifu_no;

void geko_serifuev_serifu_no_bonus(void)
{
    s32 t6 = serifu_no;

    if (t6 == 0) {
        goto label0;
    }
    now_serifuev += 5;

label0:
    sub_sub_step_up();
}
