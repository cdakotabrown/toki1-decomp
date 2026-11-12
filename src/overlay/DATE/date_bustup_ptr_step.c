/* Function: date_bustup_ptr_step */
/* Overlay: DATE */
/* Address: 0x8013F480 */
/* Size: 40 bytes (10 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 bustup_ptr_init();

s32 date_bustup_ptr_step(void)
{
    bustup_ptr_init();
    return sub_sub_step_up();
}
