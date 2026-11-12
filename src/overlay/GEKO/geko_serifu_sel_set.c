/* @SymbolStart: 0x80137098 */
/* @SymbolSize: 52 */
/* geko_serifu_sel_set -- GEKO overlay, Size: 52 bytes (13 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 geko_now_serifu2;
extern s32 serifu_no;

void geko_serifu_sel_set(void)
{
    s32 v = serifu_no;

    geko_now_serifu2 = (s16)((v << 1) + 13);
    sub_sub_step_up();
}
