/* Address: 0x80083D04 - 0x80083D2C (40 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void hizuke_disp_switch(s32 arg0);
extern void sub_sub_step_up(void);

void hizuke_show_step(void) {
    hizuke_disp_switch(1);
    sub_sub_step_up();
}
