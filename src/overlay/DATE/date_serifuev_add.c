/* @SymbolStart: 0x8014DC3C */
/* @SymbolSize: 56 */
/* Function: date_serifuev_add */
/* Overlay: DATE */
/* @SymbolSize: 56, so the head bytes are unowned there). */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern u16 now_serifuev;
extern s32 serifu_no;

s32 date_serifuev_add(void)
{
    now_serifuev += serifu_no;
    return sub_sub_step_up();
}
