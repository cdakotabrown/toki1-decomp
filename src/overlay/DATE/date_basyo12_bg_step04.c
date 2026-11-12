/* Function: date_basyo12_bg_step04 */
/* Overlay: DATE */
/* Address: 0x80137D60 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern void dec_bg_cache_step();

s32 date_basyo12_bg_step04(void)
{
    dec_bg_cache_step(17205);
    return sub_sub_step_up();
}
