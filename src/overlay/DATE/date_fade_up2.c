/* Function: date_fade_up2 */
/* Overlay: DATE */
#include "types.h"

extern s32 fade_up_main();
extern u8 reserved_date_b3;

s32 date_fade_up2(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 v0;

    v0 = fade_up_main(a0, a1, a2, a3);
    reserved_date_b3 &= ~0x20;
    return v0;
}
