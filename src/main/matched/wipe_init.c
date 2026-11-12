/* Address: 0x8007C908 */
/* Size: 44 bytes (cross-prologue, 4 bytes before body at 0x8007C90C) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern s32 fade_flag;
extern u8 fade_bri;

extern void sub_sub_step_up(void);

void wipe_init(void)
{

    fade_flag = 0;
    fade_bri = 0;

    sub_sub_step_up();
}
