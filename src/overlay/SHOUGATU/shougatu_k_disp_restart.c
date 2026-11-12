/* @SymbolSize: 56 */
/* /* shougatu_k_disp_restart - SHOUGATU overlay, Address: 0x80141CDC, Size: 64 bytes (16 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void k_disp_start();
extern void k_speed_set();
extern u8 shougatu_k_speed_wk;

void shougatu_k_disp_restart(void)
{
    k_speed_set(shougatu_k_speed_wk);
    k_disp_start(1);
    sub_sub_step_up();
}
