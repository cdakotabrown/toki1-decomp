/* @SymbolStart: 0x80133F94 */
/* @SymbolSize: 84 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* serifu_state_row_set -- GEKO overlay, Address: 0x80133F94 (prologue 0x80133FB4), */
/* Size: 84 bytes (21 insns).  geko_now_serifu += geko_serifu_state - 1; when geko_serifu_state */
#include "types.h"

extern void sub_sub_step_up(void);

extern u8 geko_serifu_state;
extern s16 geko_now_serifu;

void serifu_state_row_set(void)
{
    geko_now_serifu = geko_now_serifu + geko_serifu_state - 1;
    if (geko_serifu_state == 100) {
        geko_now_serifu = 40;
    }
    sub_sub_step_up();
}
