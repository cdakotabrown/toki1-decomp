/* Debug Symbol: wipe_main2 (from o.bin, high match) */
/* Address: 0x8007D9BC */
/* Size: 52 bytes (13 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 _wipe_main(void);
extern void sub_sub_step_up(void);

void wipe_main2(void)
{
    if (_wipe_main() == 1) {
        sub_sub_step_up();
    }
}
