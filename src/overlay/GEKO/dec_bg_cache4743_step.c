/* @SymbolSize: 40 */
/* dec_bg_cache4743_step -- GEKO overlay, Address: 0x80140A70 */
#include "types.h"

extern void dec_bg_cache_step(s32);
extern void sub_sub_step_up(void);

void dec_bg_cache4743_step(void)
{
    dec_bg_cache_step(18243);
    sub_sub_step_up();
}
