/* Likely Symbol: config_click_mode_exit (from o.bin, offset +88 bytes from 0x8005F0EC) */
/* Address: 0x8005F144 */
/* Debug Symbol: config_click_mode_exit (REVERTED - offset 88, uncertain) (from o.bin, high match) */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 pad_trig;
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 arg0);

void config_click_mode_exit(void) {
    k_disp_inc();
    if (pad_trig & 0x20) {
        goto_sub_sub_step(48);
    }
}
