/* Address: 0x8005A2C0 */
/* Size: 60 bytes */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack 24 40 */
#include "types.h"

extern u8 now_sub_step;

extern void boot_phase_switch(void);
extern void goto_step(s32);

void boot_main(void)
{

    if (now_sub_step == 0) {
        boot_phase_switch();
        return;
    }
    goto_step(18);
}
