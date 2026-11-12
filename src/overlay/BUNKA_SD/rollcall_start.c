/* @SymbolSize: 52 */
/* rollcall_start - BUNKA_SD overlay, Address: 0x801325E8, Size: 60 bytes (15 insns) */
#include "types.h"

extern void rollcall_init(void);
extern void k_speed_set(s32);
extern void sub_sub_step_up(void);

extern s32 bunka_sd_line_top;

void rollcall_start(void)
{
    rollcall_init();
    k_speed_set(5);
    bunka_sd_line_top = 0;
    sub_sub_step_up();
}
