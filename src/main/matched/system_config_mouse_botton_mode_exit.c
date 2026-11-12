/* Debug Symbol: system_config_mouse_botton_mode_exit (from o.bin, high match) */
/* Address: 0x8005EEB8 */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 pad_trig;
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 arg0);

void system_config_mouse_botton_mode_exit(void) {
    k_disp_inc();
    if (pad_trig & 0x20) {
        goto_sub_sub_step(48);
    }
}
