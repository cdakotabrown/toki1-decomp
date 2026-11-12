/* Address: 0x8005E64C */
/* Size: 72 bytes */
/* Region: GCC (0x80040000-0x80080000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 pad_trig_w;

extern u8 now_icon_b;

extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32 a0);

void system_config_moji_speed_exit(void) {
    k_disp_inc();
    if (pad_trig_w & 0x20) {
        goto_sub_sub_step(0);
        now_icon_b = 0;
    }
}
