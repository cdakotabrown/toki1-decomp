/* Likely Symbol: wipe_disp (from o.bin, offset +84 bytes from 0x8007C988) */
/* Address: 0x8007C934 */
/* Size: 52 bytes (13 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 wipe_disp(void);
extern void sub_sub_step_up(void);

void wipe_main(void)
{
    if (wipe_disp() == 1) {
        sub_sub_step_up();
    }
}
