/* Address: 0x8006033C */
/* Size: 76 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 pad_trig;
extern u8 now_icon;
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 arg0);

void voice_mode_exit(void) {
    k_disp_inc();
    if (pad_trig & 0x20) {
        goto_sub_sub_step(0);
        now_icon = 5;
    }
}
