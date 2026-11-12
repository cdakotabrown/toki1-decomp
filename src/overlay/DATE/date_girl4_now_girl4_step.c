/* @SymbolSize: 40 */
/* Function: date_girl4_now_girl4_step */
/* Overlay: DATE */
/* Address: 0x80159470 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 sub_sub_step_up();

s32 date_girl4_now_girl4_step(void)
{
    now_girl = 4;
    return sub_sub_step_up();
}
