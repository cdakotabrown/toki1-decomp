/* Likely Symbol: fujisaki_wink (from o.bin, offset +116 bytes from 0x80072D9C) */
/* Address: 0x80072D28 */
/* Size: 56 bytes (14 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void parameter_show_init(void);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void sub_step_up(void);

void byouki_init(void)
{
    parameter_show_init();
    hizuke_init();
    message_window_init();
    sub_step_up();
}
