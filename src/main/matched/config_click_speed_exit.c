/* Likely Symbol: config_menu_mode_init (from o.bin, offset +144 bytes from 0x8005F4FC) */
/* Address: 0x8005F46C */
/* Size: 64 bytes */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 pad_trig;
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 arg0);

void config_click_speed_exit(void) {
    k_disp_inc();
    if (pad_trig & 0x20) {
        goto_sub_sub_step(48);
    }
}
