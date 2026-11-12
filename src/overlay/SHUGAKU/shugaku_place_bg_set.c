/* @SymbolStart: 0x801380F4 */
/* @SymbolSize: 104 */
/* shugaku_place_bg_set - SHUGAKU overlay, Size: 104 bytes (26 insns) */
#include "types.h"

extern u8 shugaku_place;

extern void dec_bg_cache_step(s32 a0);
extern void sub_sub_step_up(void);

void shugaku_place_bg_set(void)
{
    switch (shugaku_place) {
    case 0:
        dec_bg_cache_step(16951);
        break;
    case 1:
        dec_bg_cache_step(16942);
        break;
    default:
        dec_bg_cache_step(16961);
        break;
    }
    sub_sub_step_up();
}
